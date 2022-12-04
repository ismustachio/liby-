#pragma once

#include <cmath>
#include <stdexcept>

namespace liby {
namespace math {
class Vector2D {
public:
  Vector2D() = default;
  Vector2D(const Vector2D &) = default;
  Vector2D(float x, float y);
  Vector2D &operator=(const Vector2D &);
  float &operator[](int i);
  const float &operator[](int i) const;
  const float &x() const;
  const float &y() const;
  Vector2D &operator+=(const Vector2D &);
  Vector2D &operator-=(const Vector2D &);
  Vector2D &operator-();
  const Vector2D operator-() const;
  Vector2D &operator*=(const Vector2D &);
  Vector2D &operator/=(const Vector2D &);
  Vector2D &operator*=(const float);
  Vector2D &operator/=(const float);

  friend Vector2D operator+(const Vector2D &, const Vector2D &);
  friend Vector2D operator-(const Vector2D &, const Vector2D &);
  friend Vector2D operator/(const Vector2D &, const Vector2D &);
  friend Vector2D operator*(const Vector2D &, const Vector2D &);
  friend Vector2D operator*(const Vector2D &, const float);
  friend Vector2D operator*(const float, const Vector2D &);
  friend Vector2D operator/(const Vector2D &, const float);
  friend Vector2D operator/(const float, const Vector2D &);
  friend Vector2D project(const Vector2D &, const Vector2D &);
  friend Vector2D reject(const Vector2D &, const Vector2D &);
  friend Vector2D reflect(const Vector2D &, const Vector2D &);
  friend Vector2D normalize(const Vector2D &);
  friend float magnitude(const Vector2D &);
  friend float dot(const Vector2D &, const Vector2D &);

private:
  float x_;
  float y_;
};

class Point2D : public Vector2D {
  Point2D() = default;
  Point2D(float x, float y);

  friend Point2D operator+(const Point2D &, const Vector2D &);
  friend Vector2D operator-(const Point2D &, const Point2D &);
};
} // namespace math
} // namespace liby
