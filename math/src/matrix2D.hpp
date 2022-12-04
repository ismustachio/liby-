#pragma once

#include "vector2D.hpp"

namespace liby {
namespace math {
class Matrix2D {
public:
  Matrix2D() = default;
  Matrix2D(float a, float b, float c, float d);
  Matrix2D(const Vector2D &v, const Vector2D &q);
  Matrix2D(const Matrix2D &m) = default;
  Matrix2D &operator=(const Matrix2D &m) = default;
  Vector2D &operator[](int j);
  const Vector2D &operator[](int j) const;
  float &operator()(int i, int j);
  const float &operator()(int i, int j) const;
  Matrix2D &operator*=(const Matrix2D &);
  Matrix2D &operator*=(const float);

  friend Matrix2D operator*(const Matrix2D &, const Matrix2D &);
  friend Vector2D operator*(const Matrix2D &, const Vector2D &);
  friend Matrix2D operator*(const Matrix2D &, float s);
  friend Matrix2D operator/(const Matrix2D &, float s);
  friend Matrix2D operator*(float s, const Matrix2D &);
  friend Matrix2D operator/(float s, const Matrix2D &);
  friend float determinant(const Matrix2D &);
  friend Matrix2D transpose(const Matrix2D &);
  friend Matrix2D inverse(const Matrix2D &);

  static Matrix2D identity();
  static Matrix2D makeScale(float sx, float sy);
  static Matrix2D makeScaleX(float sx);
  static Matrix2D makeScaleY(float sy);
  static Matrix2D makeRotation(float r);

private:
  float n[2][2];
};
} // namespace math
} // namespace liby
