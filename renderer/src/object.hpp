#pragma once

#include "model.hpp"

#include <glm/fwd.hpp>
#include <memory>
namespace liby {
namespace renderer {
struct Transform2DComponent {
  glm::vec2 translation{}; // position
  glm::vec2 scale{1.0f, 1.0f};
  float rotation;

  glm::mat2 mat2() {
    const float s = glm::sin(rotation);
    const float c = glm::cos(rotation);
    glm::mat2 rotMatrix{{c, s}, {-s, c}};
    glm::mat2 scaleMat{{scale.x, 0.0f}, {0.0f, scale.y}};
    return rotMatrix * scaleMat;
  }
};

class GameObject {
public:
  using id_t = unsigned int;

  GameObject(const GameObject &) = delete;
  GameObject &operator=(const GameObject &) = delete;
  GameObject(GameObject &&) = default;
  GameObject &operator=(GameObject &&) = default;

  static GameObject createGameObject() {
    static id_t currentId = 0;
    return GameObject(currentId++);
  }

  id_t getId() const { return id_; }

  std::shared_ptr<Model> model_{};
  glm::vec3 color_;
  Transform2DComponent transform2D_;

private:
  GameObject(id_t objectId) : id_{objectId} {}

  id_t id_;
};
} // namespace renderer
} // namespace liby
