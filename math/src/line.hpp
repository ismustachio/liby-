#pragma once

#include "matrix3D.hpp"
#include "matrix4D.hpp"
#include "transform4D.hpp"
#include "vector3D.hpp"

namespace liby {
namespace math {
class Line {
public:
  Line() = default;
  Line(const Vector3D &, const Vector3D &);
  Line(float vx, float vy, float vz, float mx, float my, float mz);

  friend Line transform(const Line &, const Transform4D &);

private:
  Vector3D direction;
  Vector3D moment;
};
} // namespace math
} // namespace liby
