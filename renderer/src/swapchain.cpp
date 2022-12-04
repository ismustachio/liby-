#include "swapchain.hpp"

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace liby {
namespace renderer {
Swapchain::Swapchain(Device &device, VkExtent2D extent)
    : device_{device}, windowExtent_{extent} {
  init();
}

Swapchain::Swapchain(Device &device, VkExtent2D extent,
                     std::shared_ptr<Swapchain> previous)
    : device_{device}, windowExtent_{extent}, oldSwapchain_{previous} {
  init();
  oldSwapchain_ = nullptr;
}

void Swapchain::init() {
  createSwapchain();
  createImageViews();
  createRenderPass();
  createDepthResources();
  createFramebuffers();
  createSyncObjects();
}

Swapchain::~Swapchain() {
  for (auto imageView : imageViews_) {
    vkDestroyImageView(device_.device(), imageView, nullptr);
  }
  imageViews_.clear();

  if (swapchain_ != nullptr) {
    vkDestroySwapchainKHR(device_.device(), swapchain_, nullptr);
    swapchain_ = nullptr;
  }

  for (int i = 0; i < depthImages_.size(); i++) {
    vkDestroyImageView(device_.device(), depthImageViews_[i], nullptr);
    vkDestroyImage(device_.device(), depthImages_[i], nullptr);
    vkFreeMemory(device_.device(), depthImageMemorys_[i], nullptr);
  }

  for (auto framebuffer : framebuffers_) {
    vkDestroyFramebuffer(device_.device(), framebuffer, nullptr);
  }

  vkDestroyRenderPass(device_.device(), renderPass_, nullptr);

  // cleanup synchronization objects
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(device_.device(), renderFinishedSemaphores_[i], nullptr);
    vkDestroySemaphore(device_.device(), imageAvailableSemaphores_[i], nullptr);
    vkDestroyFence(device_.device(), inFlightFences_[i], nullptr);
  }
}

uint32_t Swapchain::width() const { return extent_.width; }
uint32_t Swapchain::height() const { return extent_.height; }
VkRenderPass Swapchain::renderPass() const { return renderPass_; }
VkImageView Swapchain::imageView(size_t index) const {
  return imageViews_[index];
}
size_t Swapchain::imageCount() const { return images_.size(); }
VkFormat Swapchain::imageFormat() const { return imageFormat_; }
VkExtent2D Swapchain::extent() const { return extent_; }
VkFramebuffer Swapchain::frameBuffer(size_t index) const {
  return framebuffers_[index];
}

float Swapchain::extentAspectRatio() const {
  return static_cast<float>(extent_.width) / static_cast<float>(extent_.height);
}

VkResult Swapchain::acquireNextImage(uint32_t *imageIndex) {
  // wait until the previous frame has finished
  if (vkWaitForFences(device_.device(), 1, &inFlightFences_[currentFrame_],
                      VK_TRUE,
                      std::numeric_limits<uint64_t>::max()) != VK_SUCCESS) {
    throw std::runtime_error("failed to wait for fences to become signaled!");
  }

  return vkAcquireNextImageKHR(
      device_.device(), swapchain_, std::numeric_limits<uint64_t>::max(),
      imageAvailableSemaphores_[currentFrame_], VK_NULL_HANDLE, imageIndex);
}

VkResult Swapchain::submitCommandBuffers(const VkCommandBuffer *buffers,
                                         uint32_t *imageIndex) {
  if (imagesInFlight_[*imageIndex] != VK_NULL_HANDLE) {
    vkWaitForFences(device_.device(), 1, &imagesInFlight_[*imageIndex], VK_TRUE,
                    UINT64_MAX);
  }
  imagesInFlight_[*imageIndex] = inFlightFences_[currentFrame_];

  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores_[currentFrame_]};
  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores_[currentFrame_]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  VkSubmitInfo submitInfo{
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = waitSemaphores,
      .pWaitDstStageMask = waitStages,
      .commandBufferCount = 1,
      .pCommandBuffers = buffers,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores = signalSemaphores,
  };

  // reset prevoius frame fence to unsignaled state
  if (vkResetFences(device_.device(), 1, &inFlightFences_[currentFrame_]) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to reset fences!");
  }

  if (vkQueueSubmit(device_.graphicsQueue(), 1, &submitInfo,
                    inFlightFences_[currentFrame_]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkSwapchainKHR swapchains[] = {swapchain_};
  VkPresentInfoKHR presentInfo{
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = signalSemaphores,
      .swapchainCount = 1,
      .pSwapchains = swapchains,
      .pImageIndices = imageIndex,
  };

  currentFrame_ = (currentFrame_ + 1) % MAX_FRAMES_IN_FLIGHT;

  return vkQueuePresentKHR(device_.presentQueue(), &presentInfo);
}

void Swapchain::createSwapchain() {
  auto swapChainSupport =
      querySwapChainSupport(device_.physicalDevice(), device_.surface());

  VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(*this, swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .surface = device_.surface(),
      .minImageCount = imageCount,
      .imageFormat = surfaceFormat.format,
      .imageColorSpace = surfaceFormat.colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  };

  auto indices = findQueueFamilies(device_.physicalDevice(), device_.surface());
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                   indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;     // Optional
    createInfo.pQueueFamilyIndices = nullptr; // Optional
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain =
      oldSwapchain_ == nullptr ? VK_NULL_HANDLE : oldSwapchain_->swapchain_;

  if (vkCreateSwapchainKHR(device_.device(), &createInfo, nullptr,
                           &swapchain_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  if (vkGetSwapchainImagesKHR(device_.device(), swapchain_, &imageCount,
                              nullptr) != VK_SUCCESS) {
    throw std::runtime_error("failed to get swapchain images!");
  }
  images_.resize(imageCount);
  if (vkGetSwapchainImagesKHR(device_.device(), swapchain_, &imageCount,
                              images_.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to get swapchain images!");
  }

  imageFormat_ = surfaceFormat.format;
  extent_ = extent;
}

void Swapchain::createImageViews() {
  imageViews_.resize(images_.size());
  for (size_t i = 0; i < images_.size(); i++) {
    VkImageViewCreateInfo viewInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = images_[i],
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = imageFormat_,
        .subresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };
    if (vkCreateImageView(device_.device(), &viewInfo, nullptr,
                          &imageViews_[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture image view!");
    }
  }
}

void Swapchain::createRenderPass() {
  VkAttachmentDescription depthAttachment{
      .format = findDepthFormat(*this),
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentReference depthAttachmentRef{
      .attachment = 1,
      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentDescription colorAttachment{
      .format = imageFormat_,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  };

  VkAttachmentReference colorAttachmentRef{
      .attachment = 0,
      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  VkSubpassDescription subpass{
      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
      .colorAttachmentCount = 1,
      .pColorAttachments = &colorAttachmentRef,
      .pDepthStencilAttachment = &depthAttachmentRef,
  };

  VkSubpassDependency dependency{
      .srcSubpass = VK_SUBPASS_EXTERNAL,
      .dstSubpass = 0,
      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
      .srcAccessMask = 0,
      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                       VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
  };

  std::array<VkAttachmentDescription, 2> attachments = {colorAttachment,
                                                        depthAttachment};
  VkRenderPassCreateInfo renderPassInfo{
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .attachmentCount = static_cast<uint32_t>(attachments.size()),
      .pAttachments = attachments.data(),
      .subpassCount = 1,
      .pSubpasses = &subpass,
      .dependencyCount = 1,
      .pDependencies = &dependency,
  };

  if (vkCreateRenderPass(device_.device(), &renderPassInfo, nullptr,
                         &renderPass_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  }
}

void Swapchain::createFramebuffers() {
  framebuffers_.resize(imageCount());
  for (size_t i = 0; i < imageCount(); i++) {
    std::array<VkImageView, 2> attachments = {imageViews_[i],
                                              depthImageViews_[i]};

    VkFramebufferCreateInfo framebufferInfo{
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = renderPass_,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .width = extent_.width,
        .height = extent_.height,
        .layers = 1,
    };

    if (vkCreateFramebuffer(device_.device(), &framebufferInfo, nullptr,
                            &framebuffers_[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }
}

void Swapchain::createDepthResources() {
  VkFormat depthFormat = findDepthFormat(*this);
  depthImages_.resize(imageCount());
  depthImageMemorys_.resize(imageCount());
  depthImageViews_.resize(imageCount());

  for (int i = 0; i < depthImages_.size(); i++) {
    VkImageCreateInfo imageInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = depthFormat,
        .extent{
            .width = extent_.width,
            .height = extent_.height,
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    device_.createImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                depthImages_[i], depthImageMemorys_[i]);

    VkImageViewCreateInfo viewInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = depthImages_[i],
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = depthFormat,
        .subresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    if (vkCreateImageView(device_.device(), &viewInfo, nullptr,
                          &depthImageViews_[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create texture image view!");
    }
  }
}

void Swapchain::createSyncObjects() {
  imageAvailableSemaphores_.resize(MAX_FRAMES_IN_FLIGHT);
  renderFinishedSemaphores_.resize(MAX_FRAMES_IN_FLIGHT);
  inFlightFences_.resize(MAX_FRAMES_IN_FLIGHT);
  imagesInFlight_.resize(imageCount(), VK_NULL_HANDLE);

  VkSemaphoreCreateInfo semaphoreInfo{
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
  };

  VkFenceCreateInfo fenceInfo{
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
      // workaround for first frame (first frame to be drawn) so call to wait
      // does not block indefinitely
      .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(device_.device(), &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores_[i]) != VK_SUCCESS ||
        vkCreateSemaphore(device_.device(), &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores_[i]) != VK_SUCCESS ||
        vkCreateFence(device_.device(), &fenceInfo, nullptr,
                      &inFlightFences_[i]) != VK_SUCCESS) {
      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}

VkExtent2D chooseSwapExtent(const Swapchain &swapchain,
                            const VkSurfaceCapabilitiesKHR &capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    VkExtent2D actualExtent = swapchain.windowExtent_;
    actualExtent.width = std::max(
        capabilities.minImageExtent.width,
        std::min(capabilities.maxImageExtent.width, actualExtent.width));
    actualExtent.height = std::max(
        capabilities.minImageExtent.height,
        std::min(capabilities.maxImageExtent.height, actualExtent.height));

    return actualExtent;
  }
}

VkFormat findDepthFormat(const Swapchain &swapchain) {
  return findSupportedFormat(
      swapchain.device_,
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
} // namespace renderer
} // namespace liby
