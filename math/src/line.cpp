#include "line.hpp"

namespace liby {
namespace math {
Line::Line(const Vector3D &d, const Vector3D &m) : direction(d), moment(m) {}
Line::Line(float vx, float vy, float vz, float mx, float my, float mz)
    : direction(Vector3D(vx, vy, vz)), moment(Vector3D(mx, my, mz)) {}

Line transform(const Line &l, const Transform4D &h) {
  auto adj = Matrix3D(cross(h[1], h[2]), cross(h[2], h[0]), cross(h[0], h[1]));
  auto t = h.getTranslation();

  auto v = h * l.direction;
  auto m = adj * l.moment + cross(t, v);
  return Line(v, m);
}
} // namespace math
} // namespace liby
