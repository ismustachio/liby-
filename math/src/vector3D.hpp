#pragma once

#include <cmath>
#include <stdexcept>

namespace liby {
namespace math {
class Vector3D {
public:
  Vector3D() = default;
  Vector3D(const Vector3D &) = default;
  Vector3D(float x, float y, float z);
  Vector3D &operator=(const Vector3D &);
  float &operator[](int i);
  const float &operator[](int i) const;
  Vector3D &operator+=(const Vector3D &);
  Vector3D &operator-=(const Vector3D &);
  Vector3D &operator-();
  const Vector3D operator-() const;
  Vector3D &operator*=(const Vector3D &);
  Vector3D &operator/=(const Vector3D &);
  Vector3D &operator*=(const float);
  Vector3D &operator/=(const float);
  const float &x() const;
  const float &y() const;
  const float &z() const;

  friend Vector3D operator+(const Vector3D &, const Vector3D &);
  friend Vector3D operator-(const Vector3D &, const Vector3D &);
  friend Vector3D operator/(const Vector3D &, const Vector3D &);
  friend Vector3D operator*(const Vector3D &, const Vector3D &);
  friend Vector3D operator*(const Vector3D &, const float);
  friend Vector3D operator*(const float, const Vector3D &);
  friend Vector3D operator/(const Vector3D &, const float);
  friend Vector3D operator/(const float, const Vector3D &);
  friend Vector3D project(const Vector3D &, const Vector3D &);
  friend Vector3D reject(const Vector3D &, const Vector3D &);
  friend Vector3D reflect(const Vector3D &, const Vector3D &);
  friend Vector3D normalize(const Vector3D &);
  friend float magnitude(const Vector3D &);
  friend Vector3D cross(const Vector3D &, const Vector3D &);
  friend float dot(const Vector3D &, const Vector3D &);

protected:
  float x_;
  float y_;
  float z_;
};

class Point3D : public Vector3D {
public:
  Point3D() = default;
  Point3D(float x, float y, float z);
  Point3D &operator=(const Vector3D &);

  friend Point3D operator+(const Point3D &, const Vector3D &);
  friend Point3D operator-(const Point3D &, const Vector3D &);
  friend Vector3D operator-(const Point3D &, const Point3D &);
};

/**
 * @brief Calculates the distance between the p and the line determined by the
 * point p and the direction v.
 *
 * @param q Point3D
 * @param p Point3D
 * @param v Vector3D
 *
 * @return float
 */
float DistancePointLine(const Point3D &q, const Point3D &p, const Vector3D &v);

/**
 * @brief Calculates the distance between two lines determined by the points p1
 * and p2 and directions v1 and v2.
 *
 * @param p1
 * @param v1
 * @param p2
 * @param v2
 *
 * @return
 */
float DistanceLineLine(const Point3D &p1, const Vector3D &v1, const Point3D &p2,
                       const Vector3D &v2);
} // namespace math
} // namespace liby
