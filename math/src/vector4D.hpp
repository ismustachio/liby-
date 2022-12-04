#pragma once

#include <cmath>
#include <stdexcept>

namespace liby {
namespace math {
class Vector4D {
public:
  Vector4D() = default;
  Vector4D(const Vector4D &) = default;
  Vector4D(float x, float y, float z, float w);
  Vector4D &operator=(const Vector4D &);
  float &operator[](int i);
  const float &operator[](int i) const;
  Vector4D &operator+=(const Vector4D &);
  Vector4D &operator-=(const Vector4D &);
  Vector4D &operator-();
  const Vector4D operator-() const;
  Vector4D &operator*=(const Vector4D &);
  Vector4D &operator/=(const Vector4D &);
  Vector4D &operator*=(const float);
  Vector4D &operator/=(const float);
  const float &x() const;
  const float &y() const;
  const float &z() const;
  const float &w() const;

  friend Vector4D operator+(const Vector4D &, const Vector4D &);
  friend Vector4D operator-(const Vector4D &, const Vector4D &);
  friend Vector4D operator/(const Vector4D &, const Vector4D &);
  friend Vector4D operator*(const Vector4D &, const Vector4D &);
  friend Vector4D operator*(const Vector4D &, const float);
  friend Vector4D operator*(const float, const Vector4D &);
  friend Vector4D operator/(const Vector4D &, const float);
  friend Vector4D operator/(const float, const Vector4D &);
  friend Vector4D project(const Vector4D &, const Vector4D &);
  friend Vector4D reject(const Vector4D &, const Vector4D &);
  friend Vector4D reflect(const Vector4D &, const Vector4D &);
  friend Vector4D normalize(const Vector4D &);
  friend float magnitude(const Vector4D &);
  friend float dot(const Vector4D &, const Vector4D &);

private:
  float x_;
  float y_;
  float z_;
  float w_;
};
} // namespace math
} // namespace liby
