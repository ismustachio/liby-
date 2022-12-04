#pragma once

#include "utils.hpp"
#include "window.hpp"
#include <iostream>
#include <optional>
#include <set>
#include <vector>

namespace liby {
namespace renderer {
class Device {
public:
  Device(Window &);
  ~Device();
  Device(const Device &) = delete;
  Device &operator=(const Device &) = delete;
  Device(Device &&) = delete;
  Device &operator=(Device &&) = delete;

  VkCommandPool commandPool(void) const;
  VkDevice device(void) const;
  VkSurfaceKHR surface(void) const;
  VkQueue graphicsQueue(void) const;
  VkQueue presentQueue(void) const;
  VkPhysicalDevice physicalDevice(void) const;

  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);

  void createImageWithInfo(const VkImageCreateInfo &imageInfo,
                           VkMemoryPropertyFlags properties, VkImage &image,
                           VkDeviceMemory &imageMemory);

  friend void copyBuffer(Device &device, VkBuffer srcBuffer, VkBuffer dstBuffer,
                         VkDeviceSize size);
  friend void endSingleTimeCommands(Device &device,
                                    VkCommandBuffer commandBuffer);
  friend void copyBufferToImage(Device &device, VkBuffer buffer, VkImage image,
                                uint32_t width, uint32_t height,
                                uint32_t layerCount);
  friend VkCommandBuffer beginSingleTimeCommands(Device &device);
  friend uint32_t findMemoryType(const Device &device, uint32_t typeFilter,
                                 VkMemoryPropertyFlags properties);

  friend VkFormat findSupportedFormat(const Device &device,
                                      const std::vector<VkFormat> &candidates,
                                      VkImageTiling tiling,
                                      VkFormatFeatureFlags flags);

  VkPhysicalDeviceProperties properties_;

private:
  void createInstance(void);
  void setupDebugMessenger(void);
  void createSurface(void);
  void pickPhysicalDevice(void);
  void createLogicalDevice(void);
  void createCommandPool(void);
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);

  // connection between selected device and vulkan library
  VkInstance instance_;

  // debug callback handle
  VkDebugUtilsMessengerEXT debugMessenger_;

  // an abstract type surface that establish a connection with WSI (window
  // system integration) to present results on the screen
  VkSurfaceKHR surface_;

  // graphic card that we select
  VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;

  // logical device that interfaces with the physical device
  VkDevice device_;

  //
  VkCommandPool commandPool_;

  // device lifetime depends on the window we are using
  Window &window_;

  // handle to graphics queue
  VkQueue graphicsQueue_;

  // handle to presentation queue
  VkQueue presentQueue_;
};
} // namespace renderer
} // namespace liby
