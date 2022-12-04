#pragma once

#include "device.hpp"
#include "object.hpp"
#include "pipeline.hpp"
#include "swapchain.hpp"
#include "window.hpp"
#include <array>
#include <glm/fwd.hpp>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace liby {
namespace renderer {
class Engine {
public:
  Engine();
  ~Engine();
  static constexpr size_t WIDTH = 800;
  static constexpr size_t HEIGHT = 600;

  void run(void);

private:
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
  void renderGameObjects(VkCommandBuffer commandBuffer);
  void drawFrame();

  void sierpinski(std::vector<Model::Vertex> &vertices, int depth,
                  glm::vec2 left, glm::vec2 right, glm::vec2 top);

  Window window_{WIDTH, HEIGHT, "Liby <Placeholder>"};
  Device device_{window_};
  std::unique_ptr<Swapchain> swapchain_;
  VkPipelineLayout pipelineLayout_;
  std::vector<VkCommandBuffer> commandBuffers_;
  std::unique_ptr<Pipeline> pipeline_;
  std::vector<GameObject> gameObjects_;
};
} // namespace renderer
} // namespace liby
