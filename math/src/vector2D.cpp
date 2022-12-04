#include "vector2D.hpp"

namespace liby {
namespace math {
Vector2D::Vector2D(float x, float y) : x_(x), y_(y) {}

Vector2D &Vector2D::operator=(const Vector2D &v) {
  x_ = v.x_;
  y_ = v.y_;
  return *this;
}

float &Vector2D::operator[](int i) {
  if (i >= 2) {
    throw std::runtime_error("Index out of bound");
  }
  return ((&x_)[i]);
}

const float &Vector2D::operator[](int i) const {
  if (i >= 2) {
    throw std::runtime_error("Index out of bound");
  }
  return ((&x_)[i]);
}

const float &Vector2D::x() const { return x_; }
const float &Vector2D::y() const { return y_; }

Vector2D &Vector2D::operator-() {
  x_ = -x_;
  y_ = -y_;
  return (*this);
}

const Vector2D Vector2D::operator-() const { return Vector2D(-x_, -y_); }

Vector2D &Vector2D::operator+=(const Vector2D &v) {
  x_ += v.x_;
  y_ += v.y_;
  return (*this);
}

Vector2D &Vector2D::operator-=(const Vector2D &v) {
  x_ -= v.x_;
  y_ -= v.y_;
  return (*this);
}

Vector2D &Vector2D::operator*=(const Vector2D &v) {
  x_ *= v.x_;
  y_ *= v.y_;
  return (*this);
}

Vector2D &Vector2D::operator/=(const Vector2D &v) {
  x_ /= v.x_;
  y_ /= v.y_;
  return (*this);
}

Vector2D &Vector2D::operator*=(const float s) {
  x_ *= s;
  y_ *= s;
  return (*this);
}

float dot(const Vector2D &v, const Vector2D &q) {
  return v.x_ * q.x_ + v.y_ * q.y_;
}

Vector2D &Vector2D::operator/=(const float s) { return (*this) *= (1.0 / s); }

Vector2D normalize(const Vector2D &v) { return v / magnitude(v); }

float magnitude(const Vector2D &v) { return sqrt(v.x_ * v.x_ + v.y_ * v.y_); }

Vector2D operator+(const Vector2D &v, const Vector2D &q) {
  return Vector2D(v.x_ + q.x_, v.y_ + q.y_);
}

Vector2D operator-(const Vector2D &v, const Vector2D &q) {
  return Vector2D(v.x_ - q.x_, v.y_ - q.y_);
}

Vector2D operator*(const Vector2D &v, const Vector2D &q) {
  return Vector2D(v.x_ * q.x_, v.y_ * q.y_);
}

Vector2D operator/(const Vector2D &v, const Vector2D &q) {
  return Vector2D(v.x_ / q.x_, v.y_ / q.y_);
}

Vector2D operator*(const float s, const Vector2D &v) {
  return Vector2D(v.x_ * s, v.y_ * s);
}

Vector2D operator*(const Vector2D &v, const float s) { return s * v; }

Vector2D operator/(const Vector2D &v, const float s) { return s / v; }

Vector2D operator/(const float s, const Vector2D &v) { return (1.0 / s) * v; }

Vector2D project(const Vector2D &v, const Vector2D &q) {
  return q * (dot(v, q) / dot(q, q));
}

Vector2D reject(const Vector2D &v, const Vector2D &q) {
  return v - (q * (dot(v, q) / dot(q, q)));
}

Vector2D reflect(const Vector2D &v, const Vector2D &q) {
  return v - (q * (2.0 * dot(v, q)));
}
} // namespace math

} // namespace liby
