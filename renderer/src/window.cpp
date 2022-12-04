#include "window.hpp"
#include <GLFW/glfw3.h>

namespace liby {
namespace renderer {
Window::Window(uint32_t width, uint32_t height, const std::string &name)
    : width_(width), height_(height), windowName_(name) {
  initWindow();
}
Window::~Window() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void Window::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window_ =
      glfwCreateWindow(width_, height_, windowName_.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, frameBufferResizeCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if (glfwCreateWindowSurface(instance, window_, nullptr, surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

void Window::frameBufferResizeCallback(GLFWwindow *glfwWindow, int width,
                                       int height) {
  auto window =
      reinterpret_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
  window->frameBufferResized_ = true;
  window->width_ = width;
  window->height_ = height;
}

const VkExtent2D Window::getExtent() const { return {width_, height_}; }

bool Window::shouldClose(void) const { return glfwWindowShouldClose(window_); }

bool Window::wasWindowResized(void) const { return frameBufferResized_; }

void Window::resetWindowResizedFlag() { frameBufferResized_ = false; }
} // namespace renderer
} // namespace liby
