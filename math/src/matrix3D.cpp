#include "matrix3D.hpp"

namespace liby {
namespace math {
Matrix3D::Matrix3D(float a, float b, float c, float d, float e, float f,
                   float g, float h, float i) {
  n[0][0] = a;
  n[1][0] = b;
  n[2][0] = c;
  n[0][1] = d;
  n[1][1] = e;
  n[2][1] = f;
  n[0][2] = g;
  n[1][2] = h;
  n[2][2] = i;
}

Matrix3D::Matrix3D(const Vector3D &v, const Vector3D &q, const Vector3D &w) {
  n[0][0] = v[0];
  n[1][0] = q[0];
  n[2][0] = w[0];
  n[0][1] = v[1];
  n[1][1] = q[1];
  n[2][1] = w[1];
  n[0][2] = v[2];
  n[1][2] = q[2];
  n[2][2] = w[2];
}

Vector3D &Matrix3D::operator[](int j) {
  if (j >= 3) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<Vector3D *>(n[j]));
}

const Vector3D &Matrix3D::operator[](int j) const {
  if (j >= 3) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<const Vector3D *>(n[j]));
}

float &Matrix3D::operator()(int i, int j) {
  if (i < 0 || i >= 3 || j < 0 || j >= 3) {
    throw std::runtime_error("Index out of bounds");
  }
  return (n[j][i]);
}

const float &Matrix3D::operator()(int i, int j) const {
  if (i < 0 || i >= 3 || j < 0 || j >= 3) {
    throw std::runtime_error("Index out of bounds");
  }
  return (n[j][i]);
}

Matrix3D &Matrix3D::operator*=(const Matrix3D &m) {
  n[0][0] = n[0][0] * m.n[0][0] + n[1][0] * m.n[0][1] + n[2][0] * m.n[0][2];
  n[0][1] = n[0][0] * m.n[1][0] + n[1][0] * m.n[1][1] + n[2][0] * m.n[1][2];
  n[0][2] = n[0][0] * m.n[1][0] + n[1][0] * m.n[1][1] + n[2][0] * m.n[2][2];
  n[1][0] = n[0][1] * m.n[0][0] + n[1][1] * m.n[0][1] + n[2][1] * m.n[0][2];
  n[1][1] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][1] * m.n[1][2];
  n[1][2] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][1] * m.n[2][2];
  n[2][0] = n[0][1] * m.n[0][0] + n[1][1] * m.n[0][1] + n[2][2] * m.n[0][2];
  n[2][1] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[1][2];
  n[2][2] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[2][2];
  return *this;
}

Matrix3D &Matrix3D::operator*=(const float s) {
  n[0][0] *= s;
  n[0][1] *= s;
  n[0][2] *= s;
  n[1][0] *= s;
  n[1][1] *= s;
  n[1][2] *= s;
  n[2][0] *= s;
  n[2][1] *= s;
  n[2][2] *= s;
  return *this;
}

Matrix3D operator*(const Matrix3D &m, const Matrix3D &q) {
  return Matrix3D(
      m.n[0][0] * q.n[0][0] + m.n[1][0] * q.n[0][1] + m.n[2][0] * q.n[0][2],
      m.n[0][0] * q.n[1][0] + m.n[1][0] * q.n[1][1] + m.n[2][0] * q.n[1][2],
      m.n[0][0] * q.n[1][0] + m.n[1][0] * q.n[1][1] + m.n[2][0] * q.n[2][2],
      m.n[0][1] * q.n[0][0] + m.n[1][1] * q.n[0][1] + m.n[2][1] * q.n[0][2],
      m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] + m.n[2][1] * q.n[1][2],
      m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] + m.n[2][1] * q.n[2][2],
      m.n[0][1] * q.n[0][0] + m.n[1][1] * q.n[0][1] + m.n[2][2] * q.n[0][2],
      m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] + m.n[2][2] * q.n[1][2],
      m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] + m.n[2][2] * q.n[2][2]);
}

Vector3D operator*(const Matrix3D &m, const Vector3D &v) {
  return Vector3D(m.n[0][0] * v[0] + m.n[0][1] * v[1] + m.n[0][2] * v[2],
                  m.n[0][1] * v[0] + m.n[1][1] * v[1] + m.n[1][2] * v[2],
                  m.n[0][2] * v[0] + m.n[2][1] * v[1] + m.n[2][2] * v[2]);
}

Matrix3D operator*(const Matrix3D &m, float s) {
  return Matrix3D(m.n[0][0] * s, m.n[1][0] * s, m.n[2][0] * s, m.n[0][1] * s,
                  m.n[1][1] * s, m.n[1][2] * s, m.n[2][0] * s, m.n[2][1] * s,
                  m.n[2][2] * s);
}

Matrix3D operator/(const Matrix3D &m, float s) {
  return Matrix3D(m.n[0][0] / s, m.n[1][0] / s, m.n[2][0] / s, m.n[0][1] / s,
                  m.n[1][1] / s, m.n[1][2] / s, m.n[2][0] / s, m.n[2][1] / s,
                  m.n[2][2] / s);
}

Matrix3D operator*(float s, const Matrix3D &m) { return m * s; }

Matrix3D operator/(float s, const Matrix3D &m) {
  s = 1.0 / s;
  return m * s;
}

float determinant(const Matrix3D &m) {
  return m.n[0][0] * (m.n[1][1] * m.n[2][2] - m.n[2][1] * m.n[1][2]) +
         m.n[1][0] * (m.n[2][1] * m.n[0][2] - m.n[0][1] * m.n[2][2]) +
         m.n[2][0] * (m.n[0][1] * m.n[1][2] - m.n[1][1] * m.n[0][2]);
}

Matrix3D transpose(const Matrix3D &m) {
  return Matrix3D(m.n[0][0], m.n[0][1], m.n[0][2], m.n[1][0], m.n[1][1],
                  m.n[1][2], m.n[2][0], m.n[2][1], m.n[2][2]);
}

Matrix3D inverse(const Matrix3D &m) {
  auto a = m[0];
  auto b = m[1];
  auto c = m[2];

  auto r0 = cross(b, c);
  auto r1 = cross(c, a);
  auto r2 = cross(a, b);
  auto ivd = 1.0 / dot(r2, c);
  return Matrix3D(r0[0] * ivd, r0[1] * ivd, r0[2] * ivd, r1[0] * ivd,
                  r1[1] * ivd, r1[2] * ivd, r2[0] * ivd, r2[1] * ivd,
                  r2[2] * ivd);
}

Matrix3D Matrix3D::makeReflection(const Vector3D &v) {
  auto x = v[0] * -2.0F;
  auto y = v[1] * -2.0F;
  auto z = v[2] * -2.0F;
  auto axay = x * v[1];
  auto axaz = x * v[2];
  auto ayaz = y * v[2];
  return Matrix3D(x * v[0] + 1.0F, axay, axaz, axay, y * v[1] + 1.0F, ayaz,
                  axaz, ayaz, z * v[2] + 1.0F);
}

Matrix3D Matrix3D::makeRotation(float r, const Vector3D &v) {
  auto c = cos(r);
  auto s = sin(r);
  auto d = 1.0F - c;

  auto x = v[0] * d;
  auto y = v[1] * d;
  auto z = v[2] * d;

  auto axay = x * v[1];
  auto axaz = x * v[2];
  auto ayaz = y * v[2];
  return Matrix3D(c + v[0], axay - s * v[2], axaz + s * v[1], axay + s * v[2],
                  c + y * v[1], ayaz - s * v[0], axaz - s * v[1],
                  ayaz + s * v[0], c + z * v[2]);
}

Matrix3D Matrix3D::makeRotationX(float x) {
  auto c = cos(x);
  auto s = sin(x);
  return Matrix3D(1.0F, 0, 0, 0, c, -s, 0, s, c);
}

Matrix3D Matrix3D::makeRotationY(float y) {
  auto c = cos(y);
  auto s = sin(y);
  return Matrix3D(c, 0, -s, 0, 1.0F, 0, s, 0, c);
}

Matrix3D Matrix3D::makeRotationZ(float z) {
  auto c = cos(z);
  auto s = sin(z);
  return Matrix3D(c, -s, 0, s, c, 0, 0, 0, 1.0F);
}

Matrix3D Matrix3D::makeScale(float s, const Vector3D &v) {
  s -= 1.0F;
  auto x = v[0] * s;
  auto y = v[1] * s;
  auto z = v[2] * s;
  auto axay = x * v[1];
  auto axaz = x * v[2];
  auto ayaz = y * v[2];
  return Matrix3D(x * v[0] + 1.0F, axay, axaz, axay, y * v[1] + 1.0F, ayaz,
                  axaz, ayaz, z * v[2] + 1.0F);
}

Matrix3D Matrix3D::makeScale(float sx, float sy, float sz) {
  return Matrix3D(sx, 0, 0, 0, sy, 0, 0, 0, sz);
}

Matrix3D Matrix3D::makeSkew(float t, const Vector3D &v, const Vector3D &q) {
  t = tan(t);
  auto x = v[0] * t;
  auto y = v[1] * t;
  auto z = v[2] * t;
  return Matrix3D(x * q[0] + 1.0F, x * q[1], x * q[2], y * q[0],
                  y * q[1] + 1.0F, y * q[2], z * q[0], z * q[1],
                  z * q[2] + 1.0F);
}

Matrix3D Matrix3D::identity() {
  return Matrix3D(1.0F, 0, 0, 0, 1.0F, 0, 0, 0, 1.0F);
}
} // namespace math
} // namespace liby
