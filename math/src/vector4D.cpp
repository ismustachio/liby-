#include "vector4D.hpp"

namespace liby {
namespace math {
Vector4D::Vector4D(float x, float y, float z, float w)
    : x_(x), y_(y), z_(z), w_(w) {}

Vector4D &Vector4D::operator=(const Vector4D &v) {
  x_ = v.x_;
  y_ = v.y_;
  z_ = v.z_;
  w_ = v.w_;
  return *this;
}

float &Vector4D::operator[](int i) {
  if (i >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return ((&x_)[i]);
}

const float &Vector4D::operator[](int i) const {
  if (i >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return ((&x_)[i]);
}

const float &Vector4D::x() const { return x_; }
const float &Vector4D::y() const { return y_; }
const float &Vector4D::z() const { return z_; }
const float &Vector4D::w() const { return w_; }

Vector4D &Vector4D::operator-() {
  x_ = -x_;
  y_ = -y_;
  z_ = -z_;
  w_ = -w_;
  return (*this);
}

const Vector4D Vector4D::operator-() const {
  return Vector4D(-x_, -y_, -z_, -w_);
}

Vector4D &Vector4D::operator+=(const Vector4D &v) {
  x_ += v.x_;
  y_ += v.y_;
  z_ += v.z_;
  w_ += v.w_;
  return (*this);
}

Vector4D &Vector4D::operator-=(const Vector4D &v) {
  x_ -= v.x_;
  y_ -= v.y_;
  z_ -= v.z_;
  w_ -= v.w_;
  return (*this);
}

Vector4D &Vector4D::operator*=(const Vector4D &v) {
  x_ *= v.x_;
  y_ *= v.y_;
  z_ *= v.z_;
  w_ *= v.w_;
  return (*this);
}

Vector4D &Vector4D::operator/=(const Vector4D &v) {
  x_ /= v.x_;
  y_ /= v.y_;
  z_ /= v.z_;
  w_ /= v.w_;
  return (*this);
}

Vector4D &Vector4D::operator*=(const float s) {
  x_ *= s;
  y_ *= s;
  z_ *= s;
  w_ *= s;
  return (*this);
}

Vector4D &Vector4D::operator/=(const float s) { return (*this) *= (1.0 / s); }

Vector4D normalize(const Vector4D &v) { return v / magnitude(v); }

float magnitude(const Vector4D &v) {
  return sqrt(v.x_ * v.x_ + v.y_ * v.y_ + v.z_ * v.z_);
}

Vector4D operator+(const Vector4D &v, const Vector4D &q) {
  return Vector4D(v.x_ + q.x_, v.y_ + q.y_, v.z_ + q.z_, v.w_ + q.w_);
}

Vector4D operator-(const Vector4D &v, const Vector4D &q) {
  return Vector4D(v.x_ - q.x_, v.y_ - q.y_, v.z_ - q.z_, v.w_ - q.w_);
}

Vector4D operator*(const Vector4D &v, const Vector4D &q) {
  return Vector4D(v.x_ * q.x_, v.y_ * q.y_, v.z_ * q.z_, v.w_ * q.w_);
}

Vector4D operator/(const Vector4D &v, const Vector4D &q) {
  return Vector4D(v.x_ / q.x_, v.y_ / q.y_, v.z_ / q.z_, v.w_ / q.w_);
}

Vector4D operator*(const float s, const Vector4D &v) {
  return Vector4D(v.x_ * s, v.y_ * s, v.z_ * s, v.w_ * s);
}

Vector4D operator*(const Vector4D &v, const float s) { return s * v; }

Vector4D operator/(const Vector4D &v, const float s) { return s / v; }

Vector4D operator/(const float s, const Vector4D &v) { return (1.0 / s) * v; }

Vector4D project(const Vector4D &v, const Vector4D &q) {
  return q * (dot(v, q) / dot(q, q));
}

Vector4D reject(const Vector4D &v, const Vector4D &q) {
  return v - (q * (dot(v, q) / dot(q, q)));
}

Vector4D reflect(const Vector4D &v, const Vector4D &q) {
  return v - (q * (2.0 * dot(v, q)));
}

float dot(const Vector4D &v, const Vector4D &q) {
  return v.x_ * q.x_ + v.y_ * q.y_ + v.z_ * q.z_ + v.w_ * q.w_;
}

} // namespace math

} // namespace liby
