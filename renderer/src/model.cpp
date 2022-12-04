#include "model.hpp"
#include <cassert>
#include <cstddef>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace liby {
namespace renderer {
Model::Model(Device &device, const std::vector<Vertex> &vertices)
    : device_{device} {
  createVertexBuffer(vertices);
}
Model::~Model() {
  vkDestroyBuffer(device_.device(), vertexBuffer_, nullptr);
  vkFreeMemory(device_.device(), vertexBufferMemory_, nullptr);
}

void Model::createVertexBuffer(const std::vector<Vertex> &vertices) {
  vertexCount_ = static_cast<uint32_t>(vertices.size());
  assert(vertexCount_ >= 3 && "Vertex count must be at least 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount_;

  device_.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       vertexBuffer_, vertexBufferMemory_);

  void *data;
  vkMapMemory(device_.device(), vertexBufferMemory_, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(device_.device(), vertexBufferMemory_);
}

void Model::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer_};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void Model::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount_, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription>
Model::Vertex::getBindingDescriptions(void) {
  std::vector<VkVertexInputBindingDescription> bindingDesription(1);
  bindingDesription[0].binding = 0;
  bindingDesription[0].stride = sizeof(Vertex);
  bindingDesription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDesription;
}

std::vector<VkVertexInputAttributeDescription>
Model::Vertex::getAttributeDescriptions(void) {
  std::vector<VkVertexInputAttributeDescription> attributeDesription(2);
  // position
  attributeDesription[0].binding = 0;
  attributeDesription[0].location = 0;
  attributeDesription[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDesription[0].offset = offsetof(Vertex, position_);

  // color
  attributeDesription[1].binding = 0;
  attributeDesription[1].location = 1;
  attributeDesription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDesription[1].offset = offsetof(Vertex, color_);
  return attributeDesription;
}
} // namespace renderer
} // namespace liby
