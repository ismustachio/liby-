#define GLFW_INCLUDE_VULKAN
#include "engine.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  liby::renderer::Engine engine{};
  try {
    engine.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
