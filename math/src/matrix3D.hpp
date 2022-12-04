#pragma once

#include "vector3D.hpp"

namespace liby {
namespace math {
class Matrix3D {
public:
  Matrix3D() = default;
  Matrix3D(float a, float b, float c, float d, float e, float f, float g,
           float h, float i);
  Matrix3D(const Vector3D &v, const Vector3D &q, const Vector3D &w);
  Matrix3D(const Matrix3D &m) = default;
  Matrix3D &operator=(const Matrix3D &m) = default;
  Vector3D &operator[](int i);
  const Vector3D &operator[](int i) const;
  float &operator()(int i, int j);
  const float &operator()(int i, int j) const;
  Matrix3D &operator*=(const Matrix3D &);
  Matrix3D &operator*=(const float);

  friend Matrix3D operator*(const Matrix3D &, const Matrix3D &);
  friend Vector3D operator*(const Matrix3D &, const Vector3D &);
  friend Matrix3D operator*(const Matrix3D &, float s);
  friend Matrix3D operator/(const Matrix3D &, float s);
  friend Matrix3D operator*(float s, const Matrix3D &);
  friend Matrix3D operator/(float s, const Matrix3D &);
  friend float determinant(const Matrix3D &);
  friend Matrix3D transpose(const Matrix3D &);
  friend Matrix3D inverse(const Matrix3D &);

  static Matrix3D makeReflection(const Vector3D &);
  static Matrix3D makeRotation(float, const Vector3D &);
  static Matrix3D makeRotationX(float x);
  static Matrix3D makeRotationY(float y);
  static Matrix3D makeRotationZ(float z);
  static Matrix3D makeScale(float s, const Vector3D &);
  static Matrix3D makeScale(float sx, float sy, float sz);
  static Matrix3D makeSkew(float t, const Vector3D &, const Vector3D &);
  static Matrix3D identity();

private:
  float n[3][3];
};
} // namespace math
} // namespace liby
