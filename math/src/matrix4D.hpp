#pragma once

#include "plane.hpp"
#include "vector3D.hpp"
#include "vector4D.hpp"

namespace liby {
namespace math {
class Matrix4D {
public:
  Matrix4D() = default;
  Matrix4D(float a, float b, float c, float d, float e, float f, float g,
           float h, float i, float j, float k, float l, float m, float n,
           float o, float p);
  Matrix4D(const Vector4D &v, const Vector4D &q, const Vector4D &w,
           const Vector4D &j);
  Matrix4D(const Matrix4D &m) = default;
  Matrix4D &operator=(const Matrix4D &m) = default;
  Vector4D &operator[](int j);
  const Vector4D &operator[](int j) const;
  float &operator()(int i, int j);
  const float &operator()(int i, int j) const;
  Matrix4D &operator*=(const Matrix4D &);
  Matrix4D &operator*=(const float);

  friend Matrix4D operator*(const Matrix4D &, const Matrix4D &);
  friend Vector4D operator*(const Matrix4D &, const Vector3D &);
  friend Vector4D operator*(const Matrix4D &, const Point3D &);
  friend float determinant(const Matrix4D &);
  friend Matrix4D transpose(const Matrix4D &);
  friend Matrix4D inverse(const Matrix4D &);

  static Matrix4D identity();

protected:
  float n[4][4];
};

} // namespace math
} // namespace liby
