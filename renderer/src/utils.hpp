#pragma once
#include <GLFW/glfw3.h>
#include <fstream>
#include <glslang/Include/glslang_c_interface.h>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

namespace liby {
namespace renderer {
#ifdef NDEBUG
static const bool EnableValidationLayers = false;
#define LOG(arg)
#else
static const bool EnableValidationLayers = true;
#define LOG(arg) printf arg
#endif

// only using the debugging layer....for now
static const std::vector<const char *> ValidationLayers{
    "VK_LAYER_KHRONOS_validation"};

// list of required device extension
static const std::vector<const char *> DeviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

static bool endsWith(const std::string &s, const std::string &part) {
  return (strstr(s.c_str(), part.c_str()) - s.c_str()) ==
         (s.size() - part.size());
}

struct QueueFamilyIndices {
  // supports drawing commands
  std::optional<uint32_t> graphicsFamily;
  // supports presentation
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails { // basic surface capabilities (min/max number
                                 // of images in swap chain, min/max
  // width and height of images)
  VkSurfaceCapabilitiesKHR capabilities;
  // surface formats (pixel format, color space)
  std::vector<VkSurfaceFormatKHR> formats;
  // available presentation modes
  std::vector<VkPresentModeKHR> presentModes;
};

static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                                     VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  // basic surface capabilities
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                            VkSurfaceKHR surface) {
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;

  for (const auto &queueFamily : queueFamilies) {
    // at least one queue family that supports VK_QUEUE_GRAPHICS_BIT
    if (queueFamily.queueCount > 0 &&
        queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
    VkBool32 presentSupport = false;
    // look for queue family that has the capibility of presenting to the
    // window surface
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (queueFamily.queueCount > 0 && presentSupport) {
      indices.presentFamily = i;
    }

    if (indices.isComplete()) {
      break;
    }
    i++;
  }
  return indices;
}

static glslang_stage_t
glslangShaderStageFromFileName(const std::string &fileName) {
  if (endsWith(fileName, ".vert")) {
    std::cout << fileName << " vert\n";
    return GLSLANG_STAGE_VERTEX;
  }
  if (endsWith(fileName, ".frag")) {
    return GLSLANG_STAGE_FRAGMENT;
  }
  return GLSLANG_STAGE_VERTEX;
}

// prints all messages coming into the system console
static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  std::cerr << "validation layer: " << pCallbackData->pMessage << "\n";
  return VK_FALSE;
}

// provides information about an object thats causing an error or a warning
static VKAPI_ATTR VkBool32 VKAPI_CALL debugReportCallback(
    VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
    uint64_t object, size_t location, int32_t messageCode,
    const char *pLayerPrefix, const char *pMessage, void *userData) {
  if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    return VK_FALSE;
  }
  printf("Debug callback (%s): %s\n", pLayerPrefix, pMessage);
  return VK_FALSE;
}

// proxy function that handles looking upd the address for extension function
// vkCreateDebugUtilsMessengerEXT
static VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

// proxy function that handles looking up the address for extension function
// vkDestroyDebugUtilsMessengerEXT
static void
DestroyDebugUtilsMessengerEXT(VkInstance instance,
                              VkDebugUtilsMessengerEXT debugMessenger,
                              const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

static std::vector<char> readShaderFile(const std::string &fileName) {
  // start reading at the end of file
  std::ifstream file(fileName, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("failed to open file " + fileName);
  }

  size_t fileSize = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();
  return buffer;
}

static std::vector<const char *> requiredGlfwExtensions() {
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  if (!glfwExtensions) {
    throw std::runtime_error(
        "failed to get glfw required instance extensions!");
  }

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  if (EnableValidationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

static bool hasValidationLayerSupport(void) {
  uint32_t layerCount;
  // get the number of available layer properties
  if (vkEnumerateInstanceLayerProperties(&layerCount, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("failed to enumerate instance layer properties!");
  }

  std::vector<VkLayerProperties> availableLayers(layerCount);
  // get the layer properties
  if (vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to enumerate instance layer properties!");
  }

  for (auto &layerName : ValidationLayers) {
    bool layerFound = false;
    for (auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }
    if (!layerFound) {
      return false;
    }
  }
  return true;
}

static bool
hasGlfwRequiredExtensions(const std::vector<const char *> &glfwExtensions) {
  uint32_t extensionCount = 0;
  if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                             nullptr) != VK_SUCCESS) {
    throw std::runtime_error(
        "failed to enumerate instance extension properties!");
  }
  std::vector<VkExtensionProperties> extensions(extensionCount);
  if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                             extensions.data()) != VK_SUCCESS) {
    throw std::runtime_error(
        "failed to enumerate instance extension properties!");
  }

  LOG(("Available Extensions: "));
  std::unordered_set<std::string> available;
  for (const auto &extension : extensions) {
    LOG(("\t%s\n", extension.extensionName));
    available.insert(extension.extensionName);
  }

  LOG(("Required Extensions: "));
  for (const auto &required : glfwExtensions) {
    LOG(("\t%s\n", required));
    if (available.find(required) == available.end()) {
      return false;
    }
  }
  return true;
}

static bool checkDeviceExtensionsSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(DeviceExtensions.begin(),
                                           DeviceExtensions.end());
  for (auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }
  return requiredExtensions.empty();
}

static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  auto indices = findQueueFamilies(device, surface);
  bool swapChainAdequate = false;
  bool extensionsSupported = checkDeviceExtensionsSupport(device);
  if (extensionsSupported) {
    auto swapChainSupport = querySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  // query device property
  // VkPhysicalDeviceProperties deviceProperties;
  // vkGetPhysicalDeviceProperties(device, &deviceProperties);
  // query optional features like texture compression, 64 bit floats, multi
  // viewport rendering
  // VkPhysicalDeviceFeatures deviceFeatures;
  // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  // pick graphic card that only support geometry shaders
  // return deviceProperties.deviceType ==
  //         VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
  //    deviceFeatures.geometryShader;
  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
  return indices.isComplete() && extensionsSupported && swapChainAdequate &&
         supportedFeatures.samplerAnisotropy;
}

static VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      LOG(("Present mode: Mailbox\n"));
      return availablePresentMode;
    }
  }
  LOG(("Present mode: V-Sync\n"));
  return VK_PRESENT_MODE_FIFO_KHR;
}

static VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      LOG(("swapchain format: B8G8R8A8\n"));
      return availableFormat;
    }
  }

  return availableFormats[0];
}

} // namespace renderer

} // namespace liby
