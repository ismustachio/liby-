#pragma once
#include "device.hpp"
#include <glm/fwd.hpp>
#include <sys/endian.h>
#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace liby {
namespace renderer {
class Model {
public:
  struct Vertex {
    glm::vec2 position_;
    glm::vec3 color_;
    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions(void);
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions(void);
  };

  Model(Device &device, const std::vector<Vertex> &vertices);
  ~Model();

  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffer(const std::vector<Vertex> &vertices);

  Device &device_;
  VkBuffer vertexBuffer_;
  VkDeviceMemory vertexBufferMemory_;
  uint32_t vertexCount_;
};
} // namespace renderer
} // namespace liby
