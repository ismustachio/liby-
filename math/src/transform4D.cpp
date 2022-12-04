#include "transform4D.hpp"

namespace liby {
namespace math {
Transform4D::Transform4D(float n00, float n01, float n02, float n03, float n10,
                         float n11, float n12, float n13, float n20, float n21,
                         float n22, float n23) {
  n[0][0] = n00;
  n[1][0] = n01;
  n[2][0] = n02;
  n[3][0] = n03;
  n[0][1] = n10;
  n[1][1] = n11;
  n[2][1] = n12;
  n[3][1] = n13;
  n[0][2] = n20;
  n[1][2] = n21;
  n[2][2] = n22;
  n[3][2] = n23;
  n[0][3] = 0.0F;
  n[1][3] = 0.0F;
  n[2][3] = 0.0F;
  n[3][3] = 1.0F;
}

Transform4D::Transform4D(const Vector3D &a, const Vector3D &b,
                         const Vector3D &c, const Point3D &p) {
  n[0][0] = a[0];
  n[1][0] = b[0];
  n[2][0] = c[0];
  n[3][0] = p[0];
  n[0][1] = a[1];
  n[1][1] = b[1];
  n[2][1] = c[1];
  n[3][1] = p[1];
  n[0][2] = a[2];
  n[1][2] = b[2];
  n[2][2] = c[2];
  n[3][2] = p[2];
  n[0][3] = 0.0F;
  n[1][3] = 0.0F;
  n[2][3] = 0.0F;
  n[3][3] = 1.0F;
}

Vector3D &Transform4D::operator[](int j) {
  if (j >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<Vector3D *>(n[j]));
}

const Vector3D &Transform4D::operator[](int j) const {
  if (j >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<const Vector3D *>(n[j]));
}

const Point3D Transform4D::getTranslation(void) const {
  return Point3D(n[0][3], n[1][3], n[2][3]);
}

void Transform4D::setTranslsation(const Point3D &p) {}

Transform4D Transform4D::makeReflection(const Vector3D &v) {}

Transform4D Transform4D::makeRotation(float r, const Vector3D &v) {}

Transform4D Transform4D::makeRotationX(float x) {
  auto c = cos(x);
  auto s = sin(x);
  return Transform4D(1.0F, 0, 0, 0, c, s, 0, -s, c, 0, 0, 0);
}

Transform4D Transform4D::makeRotationY(float y) {
  auto c = cos(y);
  auto s = sin(y);
  return Transform4D(c, 0, -s, 1.0F, 0, s, 0, c, 0, 0, 0, 0);
}

Transform4D Transform4D::makeRotationZ(float z) {
  auto c = cos(z);
  auto s = sin(z);
  return Transform4D(c, s, 0, -s, c, 0, 0, 0, 1.0F, 0, 0, 0);
}

Transform4D Transform4D::makeScale(float s, const Vector3D &v) {
  s = s - 1.0F;
  auto x = v[0] * s;
  auto y = v[1] * s;
  auto z = v[2] * s;
  auto axay = x * v[1];
  auto axaz = x * v[2];
  auto ayaz = y * v[2];
  return Transform4D(x * v[0] + 1.0F, y * v[1] + 1.0F, z * v[2] + 1.0F, axay,
                     ayaz, 0, axaz, axay, 0, axay, ayaz, 0);
}

Transform4D Transform4D::makeScale(float sx, float sy, float sz) {
  return Transform4D(sx, 0, 0, 0, sy, 0, 0, 0, sz, 0, 0, 0);
}

Transform4D Transform4D::makeSkew(float t, const Vector3D &v,
                                  const Vector3D &q) {
  t = tan(t);
  auto x = v[0] * t;
  auto y = v[1] * t;
  auto z = v[2] * t;
  return Transform4D(x * q[0] + 1.0F, y * q[0], z * q[0], x * q[1], y * q[1],
                     z * q[2], x * q[2], y * q[2], z * q[2] + 1.0F, 0.0F, 0.0F,
                     0.0F);
}

Vector3D operator*(const Transform4D &h, const Vector3D &v) {
  return (Vector3D(v[0] * h.n[0][0] + v[1] * h.n[0][1] + v[2] * h.n[0][2],
                   v[0] * h.n[1][0] + v[1] * h.n[1][1] + v[2] * h.n[1][2],
                   v[0] * h.n[2][0] + v[1] * h.n[2][1] + v[2] * h.n[2][2]));
}

Transform4D makeReflection(const Plane &f) {
  auto x = f[0] * -2.0F;
  auto y = f[1] * -2.0F;
  auto z = f[2] * -2.0F;
  auto nxny = x * f[1];
  auto nxnz = x * f[2];
  auto nynz = y * f[2];
  return Transform4D(x * f[0] + 1.0F, nxny, nxnz, x * f[3], nxny,
                     y * f[1] + 1.0F, nynz, y * f[3], nxnz, nynz,
                     z * f[2] + 1.0F, z * f[3]);
}

Plane operator*(const Transform4D &h, const Plane &f) {
  return Plane(f[0] * h.n[0][0] + f[1] * h.n[0][1] + f[2] * h.n[0][2],
               f[0] * h.n[1][0] + f[1] * h.n[1][1] + f[2] * h.n[1][2],
               f[0] * h.n[2][0] + f[1] * h.n[2][1] + f[2] * h.n[2][2],
               f[0] * h.n[3][0] + f[1] * h.n[3][1] + f[2] * h.n[3][2] + f[3]);
}
} // namespace math
} // namespace liby
