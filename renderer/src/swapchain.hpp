#pragma once

#include "device.hpp"

// vulkan headers
#include <memory>
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>

namespace liby {
namespace renderer {
class Swapchain {
public:
  static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

  Swapchain(Device &device, VkExtent2D windowExtent);
  Swapchain(Device &device, VkExtent2D windowExtent,
            std::shared_ptr<Swapchain> previous);
  ~Swapchain();
  Swapchain(const Swapchain &) = delete;
  Swapchain &operator=(const Swapchain &) = delete;

  VkFramebuffer frameBuffer(size_t index) const;
  VkRenderPass renderPass() const;
  VkImageView imageView(size_t index) const;
  size_t imageCount() const;
  VkFormat imageFormat() const;
  VkExtent2D extent() const;
  uint32_t width() const;
  uint32_t height() const;
  float extentAspectRatio() const;

  friend VkFormat findDepthFormat(const Swapchain &swapchain);
  friend VkExtent2D
  chooseSwapExtent(const Swapchain &swapchain,
                   const VkSurfaceCapabilitiesKHR &capabilities);

  VkResult acquireNextImage(uint32_t *imageIndex);
  VkResult submitCommandBuffers(const VkCommandBuffer *buffers,
                                uint32_t *imageIndex);

private:
  void init();
  void createSwapchain();
  void createImageViews();
  void createDepthResources();
  void createRenderPass();
  void createFramebuffers();
  void createSyncObjects();

  VkFormat imageFormat_;
  VkExtent2D extent_;
  std::vector<VkFramebuffer> framebuffers_;
  VkRenderPass renderPass_;
  std::vector<VkImage> depthImages_;
  std::vector<VkDeviceMemory> depthImageMemorys_;
  std::vector<VkImageView> depthImageViews_;
  std::vector<VkImage> images_;
  std::vector<VkImageView> imageViews_;
  Device &device_;
  VkExtent2D windowExtent_;
  VkSwapchainKHR swapchain_;
  std::shared_ptr<Swapchain> oldSwapchain_;
  std::vector<VkSemaphore> imageAvailableSemaphores_;
  std::vector<VkSemaphore> renderFinishedSemaphores_;
  std::vector<VkFence> inFlightFences_;
  std::vector<VkFence> imagesInFlight_;
  size_t currentFrame_ = 0;
};
} // namespace renderer
} // namespace liby
