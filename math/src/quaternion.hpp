#pragma once

#include "matrix3D.hpp"
#include "vector3D.hpp"

namespace liby {
namespace math {
class Quaternion {
public:
  Quaternion() = default;
  Quaternion(float x, float y, float z, float w);
  Quaternion(const Vector3D &v, float w);
  float &operator[](int i);
  const float &operator[](int i) const;

  const float x(void) const;
  float &x(void);
  const float y(void) const;
  float &y(void);
  const float z(void) const;
  float &z(void);
  const float w(void) const;
  float &w(void);

  const Vector3D &getVectorPart(void) const;
  Matrix3D getRotationMatrix(void);
  void setRotationMatrix(const Matrix3D &m);

  friend Quaternion operator*(const Quaternion &q1, const Quaternion &q2);

  /**
   * @brief Rotates the vector v using the quaternion q by calculating the
   * sandwich product.
   *
   * @param q Quaternion
   * @param v Vector3D
   *
   * @return Vector3D
   */
  friend Vector3D transform(const Quaternion &q, const Vector3D &v);

private:
  float x_;
  float y_;
  float z_;
  float w_;
};
} // namespace math
} // namespace liby
