#pragma once

#include "rgba.hpp"
#include "vector3D.hpp"
#include <memory>

namespace raytracer {
class PatternManager;
class Shape;
class Pattern {
public:
  Pattern();
  virtual ~Pattern();
  Pattern(renderer::RGBA a, renderer::RGBA b,
          std::unique_ptr<PatternManager> manager);
  virtual renderer::RGBA at(const mathengine::Point3D &) const;
  virtual renderer::RGBA atShape(const Shape &,
                                 const mathengine::Point3D &) const;

protected:
  renderer::RGBA a_;
  renderer::RGBA b_;
  std::unique_ptr<PatternManager> manager_;
};

class PatternManager {
public:
  PatternManager();
  ~PatternManager();
  virtual renderer::RGBA at(const Pattern &, const mathengine::Point3D &) const;
  virtual renderer::RGBA atShape(const Pattern &, const Shape &,
                                 const mathengine::Point3D &) const;
};
} // namespace raytracer
