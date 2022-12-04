#pragma once

#include "pattern.hpp"
#include "rgba.hpp"
#include "vector3D.hpp"
#include <memory>

namespace raytracer {
class PointLight;
class Material {
public:
  Material();
  ~Material();
  Material(float ambient, float diffuse, float specular, float shininess,
           float reflective, float transparency, float refractiveIndex,
           renderer::RGBA color, std::unique_ptr<Pattern> pattern);

  friend renderer::RGBA
  lighting(const Material &material, const PointLight &pointLight,
           const mathengine::Point3D &point, const mathengine::Vector3D &eye,
           const mathengine::Vector3D &normal, bool inShadow);

protected:
  float ambient_;
  float diffuse_;
  float specular_;
  float shininess_;
  float reflective_;
  float transparency_;
  float refractiveIndex_;
  renderer::RGBA color_;
  std::unique_ptr<Pattern> pattern_;
};
} // namespace raytracer
