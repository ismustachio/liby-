#pragma once

#include "vector3D.hpp"

namespace liby {
namespace math {
class Plane {
public:
  Plane() = default;
  Plane(const Plane &) = default;
  Plane(float nx, float ny, float nz, float d);
  Plane(const Vector3D &, float);
  Plane &operator=(const Plane &) = default;
  float &operator[](int i);
  const float &operator[](int i) const;
  const float &x() const;
  const float &y() const;
  const float &z() const;
  const float &w() const;

  const Vector3D &getNormal(void) const;

  friend float dot(const Plane &, const Vector3D &);
  friend float dot(const Plane &, const Point3D &);

  /**
   * @brief Calculates the point q at which the line determined by p and v
   * intersects the plane f and returns true if such a point exits and false if
   * v is parallel to the plane.
   *
   * @param
   * @param
   * @param
   * @param
   *
   * @return
   */
  friend bool intersectLine(const Plane &, const Point3D &, const Vector3D &,
                            Point3D *);

  /**
   * @brief Calculates the point p at which three planes, f1,f2, and f3
   * intersect and returns true if such a point exists. If the normal vectors
   * are not linearly independent, then function returns false.
   *
   * @param
   * @param
   * @param
   * @param p
   *
   * @return
   */
  friend bool intersectThreePlanes(const Plane &, const Plane &, const Plane &,
                                   Point3D *p);

  friend bool intersectTwoPlanes(const Plane &, const Plane &, Point3D *,
                                 Vector3D *v);

private:
  float x_;
  float y_;
  float z_;
  float w_;
};
} // namespace math
} // namespace liby
