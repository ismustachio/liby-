#include "matrix2D.hpp"

namespace liby {
namespace math {
Matrix2D::Matrix2D(float a, float b, float c, float d) {
  n[0][0] = a;
  n[1][0] = b;
  n[0][1] = c;
  n[1][1] = d;
}

Matrix2D::Matrix2D(const Vector2D &v, const Vector2D &q) {
  n[0][0] = v[0];
  n[1][0] = q[0];
  n[0][1] = v[1];
  n[1][1] = q[1];
}

Vector2D &Matrix2D::operator[](int j) {
  if (j >= 2) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<Vector2D *>(n[j]));
}

const Vector2D &Matrix2D::operator[](int j) const {
  if (j >= 2) {
    throw std::runtime_error("Index out of bounds");
  }
  return (*reinterpret_cast<const Vector2D *>(n[j]));
}

float &Matrix2D::operator()(int i, int j) {
  if (i < 0 || i >= 2 || j < 0 || j >= 2) {
    throw std::runtime_error("Index out of bounds");
  }
  return (n[j][i]);
}

const float &Matrix2D::operator()(int i, int j) const {
  if (i < 0 || i >= 2 || j < 0 || j >= 2) {
    throw std::runtime_error("Index out of bounds");
  }
  return (n[j][i]);
}

Matrix2D &Matrix2D::operator*=(const Matrix2D &m) {
  n[0][0] = n[0][0] * m.n[0][0] + n[1][0] * m.n[0][1];
  n[0][1] = n[0][0] * m.n[1][0] + n[1][0] * m.n[1][1];
  n[1][0] = n[0][1] * m.n[0][0] + n[1][1] * m.n[0][1];
  n[1][1] = n[0][1] * m.n[0][1] + n[1][1] * m.n[1][1];
  return *this;
}

Matrix2D &Matrix2D::operator*=(const float s) {
  n[0][0] *= s;
  n[0][1] *= s;
  n[1][0] *= s;
  n[1][1] *= s;
  return *this;
}

Matrix2D operator*(const Matrix2D &m, const Matrix2D &q) {
  return Matrix2D(m.n[0][0] * q.n[0][0] + m.n[1][0] * q.n[0][1],
                  m.n[0][1] * q.n[0][0] + m.n[1][1] * q.n[0][1],
                  m.n[0][0] * q.n[1][0] + m.n[1][0] * q.n[1][1],
                  m.n[0][1] * q.n[0][1] + m.n[1][1] * q.n[1][1]);
}

Vector2D operator*(const Matrix2D &m, const Vector2D &v) {
  return Vector2D(m.n[0][0] * v[0] + m.n[0][1] * v[0],
                  m.n[1][0] * v[1] + m.n[1][1] * v[1]);
}

Matrix2D operator*(const Matrix2D &m, float s) {
  return Matrix2D(m.n[0][0] * s, m.n[1][0] * s, m.n[0][1] * s, m.n[1][1] * s);
}

Matrix2D operator/(const Matrix2D &m, float s) {
  s = 1.0 / s;
  return m * s;
}

Matrix2D operator*(float s, const Matrix2D &m) { return m * s; }

Matrix2D operator/(float s, const Matrix2D &m) { return m / s; }

float determinant(const Matrix2D &m) {
  return m.n[0][0] * m.n[1][1] - m.n[0][1] * m.n[1][0];
}

Matrix2D transpose(const Matrix2D &m) {
  return Matrix2D(m.n[0][0], m.n[0][1], m.n[1][0], m.n[1][1]);
}

Matrix2D inverse(const Matrix2D &m) {
  float ivd = 1.0 / determinant(m);
  return Matrix2D(m.n[1][1] * ivd, -m.n[1][0] * ivd, -m.n[0][1] * ivd,
                  m.n[1][1] * ivd);
}

Matrix2D Matrix2D::makeScale(float sx, float sy) {
  return Matrix2D(sx, 0, 0, sy);
}

Matrix2D Matrix2D::makeScaleX(float sx) { return Matrix2D(sx, 0, 0, 0); }

Matrix2D Matrix2D::makeScaleY(float sy) { return Matrix2D(0, 0, 0, sy); }

Matrix2D Matrix2D::makeRotation(float r) {
  float c = cos(r);
  float s = sin(r);
  return Matrix2D(c, -s, s, c);
}

Matrix2D Matrix2D::identity() { return Matrix2D(1, 0, 0, 1); }

} // namespace math
} // namespace liby
