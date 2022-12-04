#include "engine.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glm/fwd.hpp>
#include <memory>
#include <sys/endian.h>
#include <vector>
#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace liby {
namespace renderer {
struct SimplePushConstantData {
  glm::mat2 transform{1.f};
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

Engine::Engine() {
  loadGameObjects();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

Engine::~Engine() {
  vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
}
void Engine::run(void) {
  while (!window_.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
  if (vkDeviceWaitIdle(device_.device()) != VK_SUCCESS) {
    throw std::runtime_error("failed to wait for device to idle!");
  }
}
void Engine::createPipelineLayout() {
  VkPushConstantRange pushConstantRange{
      .stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      .offset = 0,
      .size = sizeof(SimplePushConstantData),
  };

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = 0,
      .pSetLayouts = nullptr,
      .pushConstantRangeCount = 1,
      .pPushConstantRanges = &pushConstantRange,
  };
  if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}
void Engine::createPipeline() {
  auto pipelineConfig = PipelineConfigInfo::defaultPipelineConfig();
  pipelineConfig.renderPass = swapchain_->renderPass();
  pipelineConfig.pipelineLayout = pipelineLayout_;
  pipeline_ = std::make_unique<Pipeline>("shaders/shader.vert.spv",
                                         "shaders/shader.frag.spv", device_,
                                         pipelineConfig);
}

void Engine::recreateSwapChain() {
  auto extent = window_.getExtent();
  // handle window minimization or one dimension that is currently sizeless
  while (extent.width == 0 || extent.height == 0) {
    extent = window_.getExtent();
    glfwWaitEvents();
  }

  // wait for current swap chain to no longer be in use
  if (vkDeviceWaitIdle(device_.device()) != VK_SUCCESS) {
    throw std::runtime_error("failed to wait for device to idle!");
  }

  if (swapchain_ == nullptr) {
    swapchain_ = std::make_unique<Swapchain>(device_, extent);
  } else {
    swapchain_ =
        std::make_unique<Swapchain>(device_, extent, std::move(swapchain_));
    if (swapchain_->imageCount() != commandBuffers_.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  // recreate the pipeline since it depends on the swap chain
  createPipeline();
}

void Engine::createCommandBuffers() {
  commandBuffers_.resize(swapchain_->imageCount());

  VkCommandBufferAllocateInfo allocInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = device_.commandPool(),
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = static_cast<uint32_t>(commandBuffers_.size()),
  };

  if (vkAllocateCommandBuffers(device_.device(), &allocInfo,
                               commandBuffers_.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void Engine::recordCommandBuffer(int imageIndex) {

  VkCommandBufferBeginInfo beginInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
  };

  if (vkBeginCommandBuffer(commandBuffers_[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      // bind the framebuffer for the swapchain image we want to draw to
      .renderPass = swapchain_->renderPass(),
      .framebuffer = swapchain_->frameBuffer(imageIndex),
      // size of the render area where the shader loads and stores will take
      // place
      .renderArea{
          .offset = {0, 0},
          .extent = swapchain_->extent(),
      },
  };

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffers_[imageIndex], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  auto extent = swapchain_->extent();
  viewport.width = static_cast<float>(extent.width);
  viewport.height = static_cast<float>(extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, extent};
  vkCmdSetViewport(commandBuffers_[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers_[imageIndex], 0, 1, &scissor);

  renderGameObjects(commandBuffers_[imageIndex]);

  vkCmdEndRenderPass(commandBuffers_[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers_[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}
void Engine::freeCommandBuffers() {
  vkFreeCommandBuffers(device_.device(), device_.commandPool(),
                       static_cast<uint32_t>(commandBuffers_.size()),
                       commandBuffers_.data());
  commandBuffers_.clear();
}

void Engine::sierpinski(std::vector<Model::Vertex> &vertices, int depth,
                        glm::vec2 left, glm::vec2 right, glm::vec2 top) {
  if (depth <= 0) {
    vertices.push_back({top});
    vertices.push_back({right});
    vertices.push_back({left});
  } else {
    auto leftTop = 0.5f * (left + top);
    auto rightTop = 0.5f * (right + top);
    auto leftRight = 0.5f * (left + right);
    sierpinski(vertices, depth - 1, left, leftRight, leftTop);
    sierpinski(vertices, depth - 1, leftRight, right, rightTop);
    sierpinski(vertices, depth - 1, leftTop, rightTop, top);
  }
}

void Engine::loadGameObjects() {
  std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  // std::vector<Model::Vertex> vertices;
  // sierpinski(vertices, 5, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
  auto libyModel = std::make_shared<Model>(device_, vertices);

  auto triangle = GameObject::createGameObject();
  triangle.model_ = libyModel;
  triangle.color_ = {0.0f, .8f, .1f};
  triangle.transform2D_.translation.x = .2f;
  triangle.transform2D_.scale = {2.0f, 0.5f};
  triangle.transform2D_.rotation = 0.25f * glm::two_pi<float>();
  gameObjects_.push_back(std::move(triangle));
}

void Engine::renderGameObjects(VkCommandBuffer commandBuffer) {

  // update
  int i = 0;
  for (auto &obj : gameObjects_) {
    i += 1;
    obj.transform2D_.rotation = glm::mod<float>(
        obj.transform2D_.rotation + 0.001f * i, 2.0f * glm::pi<float>());
  }

  pipeline_->bind(commandBuffer);

  for (auto &obj : gameObjects_) {
    // obj.transform2D_.rotation =
    //     glm::mod(obj.transform2D_.rotation + 0.01f, glm::two_pi<float>());
    SimplePushConstantData push{};
    push.offset = obj.transform2D_.translation;
    push.color = obj.color_;
    push.transform = obj.transform2D_.mat2();

    vkCmdPushConstants(commandBuffer, pipelineLayout_,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);
    obj.model_->bind(commandBuffer);
    obj.model_->draw(commandBuffer);
  }
}

void Engine::drawFrame() {
  uint32_t imageIndex;
  auto result = swapchain_->acquireNextImage(&imageIndex);

  // check if swapchain has been resized
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result = swapchain_->submitCommandBuffers(&commandBuffers_[imageIndex],
                                            &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window_.wasWindowResized()) {
    window_.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}
} // namespace renderer
} // namespace liby
