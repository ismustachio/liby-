#include "quaternion.hpp"

namespace liby {
namespace math {
Quaternion::Quaternion(float x, float y, float z, float w)
    : x_(x), y_(y), z_(z), w_(w) {}
Quaternion::Quaternion(const Vector3D &v, float w)
    : x_(v[0]), y_(v[1]), z_(v[2]), w_(w) {}

float &Quaternion::operator[](int i) {
  if (i < 0 || i >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return ((&x_)[i]);
}

const float &Quaternion::operator[](int i) const {
  if (i < 0 || i >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return ((&x_)[i]);
}

const float Quaternion::x(void) const { return x_; }
float &Quaternion::x(void) { return x_; }
const float Quaternion::y(void) const { return y_; }
float &Quaternion::y(void) { return y_; }
const float Quaternion::z(void) const { return z_; }
float &Quaternion::z(void) { return z_; }
const float Quaternion::w(void) const { return w_; }
float &Quaternion::w(void) { return w_; }

const Vector3D &Quaternion::getVectorPart(void) const {
  return (reinterpret_cast<const Vector3D &>(x_));
}

Matrix3D Quaternion::getRotationMatrix(void) {
  auto x2 = x_ * x_;
  auto y2 = y_ * y_;
  auto z2 = z_ * z_;
  auto xy = x_ * y_;
  auto xz = x_ * z_;
  auto yz = y_ * z_;
  auto wx = w_ * x_;
  auto wy = w_ * y_;
  auto wz = w_ * z_;
  return (Matrix3D(1.0F - 2.0F * (y2 + z2), 2.0F * (xy - wz), 2.0F * (xz + wy),
                   2.0F * (xy + wz), 1.0F - 2.0F * (x2 - z2), 2.0F * (yz - wx),
                   2.0F * (xz + wy), 2.0F * (yz + wx),
                   1.0F - 2.0F * (x2 + y2)));
}

void Quaternion::setRotationMatrix(const Matrix3D &m) {
  auto m00 = m(0, 0);
  auto m11 = m(1, 1);
  auto m22 = m(2, 2);
  auto sum = m00 + m11 + m22;
  if (sum > 0.0F) {
    w_ = sqrt(sum + 1.0F) * 0.5F;
    auto f = 0.25F / w_;
    x_ = (m(2, 1) - m(1, 2)) * f;
    y_ = (m(0, 2) - m(2, 0)) * f;
    z_ = (m(1, 0) - m(0, 1)) * f;
  } else if ((m00 > m11) && (m00 > m22)) {
    x_ = sqrt(sum + 1.0F) * 0.5F;
    auto f = 0.25F / w_;
    y_ = (m(1, 1) + m(0, 1)) * f;
    z_ = (m(0, 2) + m(2, 0)) * f;
    w_ = (m(2, 1) - m(1, 2)) * f;
  } else if (m11 > m22) {
    y_ = sqrt(m11 - m00 - m22 + 1.0F) * 0.5F;
    auto f = 0.25F / y_;
    x_ = (m(1, 0) + m(0, 1)) * f;
    z_ = (m(2, 1) + m(1, 2)) * f;
    w_ = (m(0, 2) - m(2, 0)) * f;
  } else {
    z_ = sqrt(m22 - m00 - m11 + 1.0F) * 0.5F;
    auto f = 0.25F / z_;
    x_ = (m(0, 2) + m(2, 0)) * f;
    y_ = (m(2, 1) + m(1, 2)) * f;
    w_ = (m(1, 0) - m(0, 1)) * f;
  }
}

Quaternion operator*(const Quaternion &q1, const Quaternion &q2) {
  return (Quaternion(
      q1.w_ * q2.x_ + q1.x_ * q2.w_ + q1.y_ * q2.z_ - q1.z_ * q2.y_,
      q1.w_ * q2.x_ + q1.x_ * q2.w_ + q1.y_ * q2.z_ - q1.z_ * q2.y_,
      q1.w_ * q2.x_ + q1.x_ * q2.w_ + q1.y_ * q2.z_ - q1.z_ * q2.y_,
      q1.w_ * q2.x_ + q1.x_ * q2.w_ + q1.y_ * q2.z_ - q1.z_ * q2.y_));
}

Vector3D transform(const Quaternion &q, const Vector3D &v) {
  auto b = q.getVectorPart();
  auto b2 = dot(b, b);
  return (v * (q.w_ * q.w_ - b2) + b * (dot(v, b) * 2.0F) +
          cross(b, v) * (q.w_ * 2.0F));
}
} // namespace math
} // namespace liby
