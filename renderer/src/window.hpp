#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace liby {
namespace renderer {
class Window {
public:
  Window(uint32_t width, uint32_t height, const std::string &name);
  ~Window();
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  const VkExtent2D getExtent() const;
  bool shouldClose(void) const;
  bool wasWindowResized(void) const;
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
  void resetWindowResizedFlag();

private:
  void initWindow();
  static void frameBufferResizeCallback(GLFWwindow *glfwWindow, int width,
                                        int height);

  bool frameBufferResized_ = false;
  uint32_t width_;
  uint32_t height_;
  std::string windowName_;
  GLFWwindow *window_;
};
} // namespace renderer
} // namespace liby
