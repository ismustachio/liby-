#include "vector3D.hpp"

namespace liby {
namespace math {
Vector3D::Vector3D(float x, float y, float z) : x_(x), y_(y), z_(z) {}

Vector3D &Vector3D::operator=(const Vector3D &v) {
  x_ = v.x_;
  y_ = v.y_;
  z_ = v.z_;
  return *this;
}

float &Vector3D::operator[](int i) {
  if (i >= 3) {
    throw std::runtime_error("Index out of bound");
  }
  return ((&x_)[i]);
}

const float &Vector3D::operator[](int i) const {
  if (i >= 3) {
    throw std::runtime_error("Index out of bound");
  }
  return ((&x_)[i]);
}

const float &Vector3D::x() const { return x_; }
const float &Vector3D::y() const { return y_; }
const float &Vector3D::z() const { return z_; }

Vector3D &Vector3D::operator-() {
  x_ = -x_;
  y_ = -y_;
  z_ = -z_;
  return (*this);
}

const Vector3D Vector3D::operator-() const { return Vector3D(-x_, -y_, -z_); }

Vector3D &Vector3D::operator+=(const Vector3D &v) {
  x_ += v.x_;
  y_ += v.y_;
  z_ += v.z_;
  return (*this);
}

Vector3D &Vector3D::operator-=(const Vector3D &v) {
  x_ -= v.x_;
  y_ -= v.y_;
  z_ -= v.z_;
  return (*this);
}

Vector3D &Vector3D::operator*=(const Vector3D &v) {
  x_ *= v.x_;
  y_ *= v.y_;
  z_ *= v.z_;
  return (*this);
}

Vector3D &Vector3D::operator/=(const Vector3D &v) {
  x_ /= v.x_;
  y_ /= v.y_;
  z_ /= v.z_;
  return (*this);
}

Vector3D &Vector3D::operator*=(const float s) {
  x_ *= s;
  y_ *= s;
  z_ *= s;
  return (*this);
}

Vector3D &Vector3D::operator/=(const float s) { return (*this) *= (1.0 / s); }

Vector3D cross(const Vector3D &v, const Vector3D &q) {
  return Vector3D(v.y_ * q.z_ - v.z_ * q.y_, v.z_ * q.x_ - v.x_ * q.z_,
                  v.x_ * q.y_ - v.y_ * q.x_);
}

Vector3D normalize(const Vector3D &v) { return v / magnitude(v); }

float magnitude(const Vector3D &v) {
  return sqrt(v.x_ * v.x_ + v.y_ * v.y_ + v.z_ * v.z_);
}

Vector3D operator+(const Vector3D &v, const Vector3D &q) {
  return Vector3D(v.x_ + q.x_, v.y_ + q.y_, v.z_ + q.z_);
}

float dot(const Vector3D &v, const Vector3D &q) {
  return v.x_ * q.x_ + v.y_ * q.y_ + v.z_ * q.z_;
}

Vector3D operator-(const Vector3D &v, const Vector3D &q) {
  return Vector3D(v.x_ - q.x_, v.y_ - q.y_, v.z_ - q.z_);
}

Vector3D operator*(const Vector3D &v, const Vector3D &q) {
  return Vector3D(v.x_ * q.x_, v.y_ * q.y_, v.z_ * q.z_);
}

Vector3D operator/(const Vector3D &v, const Vector3D &q) {
  return Vector3D(v.x_ / q.x_, v.y_ / q.y_, v.z_ / q.z_);
}

Vector3D operator*(const float s, const Vector3D &v) {
  return Vector3D(v.x_ * s, v.y_ * s, v.z_ * s);
}

Vector3D operator*(const Vector3D &v, const float s) { return s * v; }

Vector3D operator/(const Vector3D &v, const float s) { return s / v; }

Vector3D operator/(const float s, const Vector3D &v) { return (1.0 / s) * v; }

Point3D::Point3D(float x, float y, float z) : Vector3D(x, y, z) {}

Point3D &Point3D::operator=(const Vector3D &v) {
  x_ = v[0];
  y_ = v[1];
  z_ = v[2];
  return *this;
}

Point3D operator+(const Point3D &p, const Vector3D &v) {
  return Point3D(p.x_ + v.x(), p.y_ + v.y(), p.z_ + v.z());
}

Vector3D operator-(const Point3D &p, const Point3D &pp) {
  return Vector3D(p.x_ + pp.x_, p.y_ + pp.y_, p.z_ + pp.z_);
}

Point3D operator-(const Point3D &p, const Vector3D &v) {
  return Point3D(p.x_ - v.x(), p.y_ - v.y(), p.z_ - v.z());
}

Vector3D project(const Vector3D &v, const Vector3D &q) {
  return q * (dot(v, q) / dot(q, q));
}

Vector3D reject(const Vector3D &v, const Vector3D &q) {
  return v - (q * (dot(v, q) / dot(q, q)));
}

Vector3D reflect(const Vector3D &v, const Vector3D &q) {
  return v - (q * (2.0 * dot(v, q)));
}

float DistancePointLine(const Point3D &q, const Point3D &p, const Vector3D &v) {
  auto a = cross(q - p, v);
  return (sqrt(dot(a, a) / dot(v, v)));
}

float DistanceLineLine(const Point3D &p1, const Vector3D &v1, const Point3D &p2,
                       const Vector3D &v2) {
  auto dp = p2 - p1;
  auto v12 = dot(v1, v1);
  auto v22 = dot(v2, v2);
  auto v1v2 = dot(v1, v2);

  auto det = v1v2 * v1v2 - v12 * v22;
  if (fabs(det) > 0.00001) {
    det = 1.0F / det;
    auto dpv1 = dot(dp, v1);
    auto dpv2 = dot(dp, v2);
    auto t1 = (v1v2 * dpv2 - v22 * dpv1) * det;
    auto t2 = (v12 * dpv2 - v1v2 * dpv1) * det;
    return (magnitude(dp + v2 * t2 - v1 * t1));
  }
  auto a = cross(dp, v1);
  return (sqrt(dot(a, a) / v12));
}
} // namespace math
} // namespace liby
