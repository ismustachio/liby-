#include "matrix4D.hpp"

namespace liby {
namespace math {
Matrix4D::Matrix4D(float a, float b, float c, float d, float e, float f,
                   float g, float h, float i, float j, float k, float l,
                   float m, float mn, float o, float p) {
  n[0][0] = a;
  n[1][0] = b;
  n[2][0] = c;
  n[3][0] = d;
  n[0][1] = e;
  n[1][1] = f;
  n[2][1] = g;
  n[3][1] = h;
  n[0][2] = i;
  n[1][2] = j;
  n[2][2] = k;
  n[3][2] = l;
  n[0][3] = m;
  n[1][3] = mn;
  n[2][3] = o;
  n[3][3] = p;
}

Matrix4D::Matrix4D(const Vector4D &v, const Vector4D &q, const Vector4D &w,
                   const Vector4D &j) {
  n[0][0] = v[0];
  n[1][0] = q[0];
  n[2][0] = w[0];
  n[3][0] = j[0];
  n[0][1] = v[1];
  n[1][1] = q[1];
  n[2][1] = w[1];
  n[3][1] = j[1];
  n[0][2] = v[2];
  n[1][2] = q[2];
  n[2][2] = w[2];
  n[3][2] = j[2];
  n[0][3] = v[3];
  n[1][3] = q[3];
  n[2][3] = w[3];
  n[3][3] = j[3];
}

Vector4D &Matrix4D::operator[](int j) {
  if (j >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<Vector4D *>(n[j]));
}

const Vector4D &Matrix4D::operator[](int j) const {
  if (j >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<const Vector4D *>(n[j]));
}

float &Matrix4D::operator()(int i, int j) {
  if (i < 0 || i >= 4 || j < 0 || j >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return (n[j][i]);
}

const float &Matrix4D::operator()(int i, int j) const {
  if (i < 0 || i >= 4 || j < 0 || j >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return (n[j][i]);
}

Matrix4D &Matrix4D::operator*=(const Matrix4D &m) {
  n[0][0] = n[0][0] * m.n[0][0] + n[1][0] * m.n[0][1] + n[2][0] * m.n[0][2] +
            n[3][0] * m.n[0][3];
  n[0][1] = n[0][0] * m.n[1][0] + n[1][0] * m.n[1][1] + n[2][0] * m.n[1][2] +
            n[3][0] * m.n[1][3];
  n[0][2] = n[0][0] * m.n[1][0] + n[1][0] * m.n[1][1] + n[2][0] * m.n[2][2] +
            n[3][0] * m.n[2][3];
  n[0][3] = n[0][0] * m.n[1][0] + n[1][0] * m.n[1][1] + n[2][0] * m.n[2][2] +
            n[3][0] * m.n[3][3];
  n[1][0] = n[0][1] * m.n[0][0] + n[1][1] * m.n[0][1] + n[2][1] * m.n[0][2] +
            n[3][1] * m.n[0][3];
  n[1][1] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][1] * m.n[1][2] +
            n[3][1] * m.n[1][3];
  n[1][2] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][1] * m.n[2][2] +
            n[3][1] * m.n[2][3];
  n[1][3] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][1] * m.n[2][2] +
            n[3][1] * m.n[3][3];
  n[2][0] = n[0][1] * m.n[0][0] + n[1][1] * m.n[0][1] + n[2][2] * m.n[0][2] +
            n[3][2] * m.n[0][3];
  n[2][1] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[1][2] +
            n[3][2] * m.n[1][3];
  n[2][2] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[2][2] +
            n[3][2] * m.n[2][3];
  n[2][3] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[2][2] +
            n[3][2] * m.n[3][3];
  n[3][0] = n[0][1] * m.n[0][0] + n[1][1] * m.n[0][1] + n[2][2] * m.n[0][2] +
            n[3][3] * m.n[0][3];
  n[3][1] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[1][2] +
            n[3][3] * m.n[1][3];
  n[3][2] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[2][2] +
            n[3][3] * m.n[2][3];
  n[3][3] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1] + n[2][2] * m.n[2][2] +
            n[3][3] * m.n[3][3];
  return *this;
}

Matrix4D &Matrix4D::operator*=(const float s) {
  n[0][0] *= s;
  n[0][1] *= s;
  n[0][2] *= s;
  n[0][3] *= s;
  n[1][0] *= s;
  n[1][1] *= s;
  n[1][2] *= s;
  n[1][3] *= s;
  n[2][0] *= s;
  n[2][1] *= s;
  n[2][2] *= s;
  n[2][3] *= s;
  n[3][0] *= s;
  n[3][1] *= s;
  n[3][2] *= s;
  n[3][3] *= s;
  return *this;
}

Matrix4D operator*(const Matrix4D &m, const Matrix4D &q) {
  return Matrix4D(m.n[0][0] * q.n[0][0] + m.n[1][0] * q.n[0][1] +
                      m.n[2][0] * q.n[0][2] + m.n[3][0] * q.n[0][3],
                  m.n[0][0] * q.n[1][0] + m.n[1][0] * q.n[1][1] +
                      m.n[2][0] * q.n[1][2] + m.n[3][0] * q.n[1][3],
                  m.n[0][0] * q.n[1][0] + m.n[1][0] * q.n[1][1] +
                      m.n[2][0] * q.n[2][2] + m.n[3][0] * q.n[2][3],
                  m.n[0][0] * q.n[1][0] + m.n[1][0] * q.n[1][1] +
                      m.n[2][0] * q.n[2][2] + m.n[3][0] * q.n[3][3],
                  m.n[0][1] * q.n[0][0] + m.n[1][1] * q.n[0][1] +
                      m.n[2][1] * q.n[0][2] + m.n[3][1] * q.n[0][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][1] * q.n[1][2] + m.n[3][1] * q.n[1][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][1] * q.n[2][2] + m.n[3][1] * q.n[2][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][1] * q.n[2][2] + m.n[3][1] * q.n[3][3],
                  m.n[0][1] * q.n[0][0] + m.n[1][1] * q.n[0][1] +
                      m.n[2][2] * q.n[0][2] + m.n[3][2] * q.n[0][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][2] * q.n[1][2] + m.n[3][2] * q.n[1][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][2] * q.n[2][2] + m.n[3][2] * q.n[2][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][2] * q.n[2][2] + m.n[3][2] * q.n[3][3],
                  m.n[0][1] * q.n[0][0] + m.n[1][1] * q.n[0][1] +
                      m.n[2][2] * q.n[0][2] + m.n[3][3] * q.n[0][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][2] * q.n[1][2] + m.n[3][3] * q.n[1][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][2] * q.n[2][2] + m.n[3][3] * q.n[2][3],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1] +
                      m.n[2][2] * q.n[2][2] + m.n[3][3] * q.n[3][3]);
}

Vector4D operator*(const Matrix4D &m, const Vector3D &v) {
  return Vector4D(m.n[0][0] * v[0] + m.n[0][1] * v[1] + m.n[0][2] * v[2],
                  m.n[1][0] * v[0] + m.n[1][1] * v[1] + m.n[1][2] * v[2],
                  m.n[2][0] * v[0] + m.n[2][1] * v[1] + m.n[2][2] * v[2],
                  m.n[3][0] * v[0] + m.n[3][1] * v[1] + m.n[3][2] * v[2]);
}

Vector4D operator*(const Matrix4D &m, const Point3D &p) {
  return Vector4D(
      m.n[0][0] * p[0] + m.n[0][1] * p[1] + m.n[0][2] * p[2] + m.n[3][0],
      m.n[1][0] * p[0] + m.n[1][1] * p[1] + m.n[1][2] * p[2] + m.n[3][1],
      m.n[2][0] * p[0] + m.n[2][1] * p[1] + m.n[2][2] * p[2] + m.n[3][2],
      m.n[3][0] * p[0] + m.n[3][1] * p[1] + m.n[3][2] * p[2] + m.n[3][3]);
}

float determinant(const Matrix4D &m) {
  return m.n[3][0] * m.n[2][1] * m.n[1][2] * m.n[0][3] -
         m.n[2][0] * m.n[3][1] * m.n[1][2] * m.n[0][3] -
         m.n[3][0] * m.n[1][1] * m.n[2][2] * m.n[0][3] +
         m.n[3][0] * m.n[2][1] * m.n[1][2] * m.n[0][3] -
         m.n[2][0] * m.n[3][1] * m.n[1][2] * m.n[0][3] -
         m.n[3][0] * m.n[1][1] * m.n[2][2] * m.n[0][3] +
         m.n[3][0] * m.n[2][1] * m.n[1][2] * m.n[0][3] -
         m.n[2][0] * m.n[3][1] * m.n[1][2] * m.n[0][3] -
         m.n[3][0] * m.n[1][1] * m.n[2][2] * m.n[0][3] +
         m.n[3][0] * m.n[2][1] * m.n[1][2] * m.n[0][3] -
         m.n[2][0] * m.n[3][1] * m.n[1][2] * m.n[0][3] -
         m.n[3][0] * m.n[1][1] * m.n[2][2] * m.n[0][3] +
         m.n[3][0] * m.n[2][1] * m.n[1][2] * m.n[0][3] -
         m.n[2][0] * m.n[3][1] * m.n[1][2] * m.n[0][3] -
         m.n[3][0] * m.n[1][1] * m.n[2][2] * m.n[0][3] +
         m.n[3][0] * m.n[1][1] * m.n[2][2] * m.n[0][3];
}

Matrix4D transpose(const Matrix4D &m) {
  return Matrix4D(m.n[0][0], m.n[0][1], m.n[0][2], m.n[0][3], m.n[1][0],
                  m.n[1][1], m.n[1][2], m.n[1][3], m.n[2][0], m.n[2][1],
                  m.n[2][2], m.n[2][3], m.n[3][0], m.n[3][1], m.n[3][2],
                  m.n[3][3]);
}

Matrix4D inverse(const Matrix4D &m) {
  auto a = reinterpret_cast<const Vector3D &>(m[0]);
  auto b = reinterpret_cast<const Vector3D &>(m[1]);
  auto c = reinterpret_cast<const Vector3D &>(m[2]);
  auto d = reinterpret_cast<const Vector3D &>(m[3]);

  auto x = m(3, 0);
  auto y = m(3, 1);
  auto z = m(3, 2);
  auto w = m(3, 3);

  auto s = cross(a, b);
  auto t = cross(c, d);
  auto u = a * y - b * x;
  auto v = c * w - d * z;

  auto ivd = 1.0F / (dot(s, v) + dot(t, u));
  s *= ivd;
  t *= ivd;
  u *= ivd;
  v *= ivd;

  auto r0 = cross(b, v) + t * y;
  auto r1 = cross(v, a) - t * x;
  auto r2 = cross(d, u) + s * w;
  auto r3 = cross(u, c) - s * z;

  return Matrix4D(r0[0], r1[0], r2[0], r3[0], r0[1], r1[1], r2[1], r3[1], r0[2],
                  r1[2], r2[2], r3[2], -dot(b, t), dot(a, t), -dot(d, s),
                  dot(c, s));
}

Matrix4D Matrix4D::identity() {
  return Matrix4D(1.0F, 0, 0, 0, 0, 1.0F, 0, 0, 0, 0, 1.0F, 0, 0, 0, 0, 1.0F);
}

} // namespace math
} // namespace liby
