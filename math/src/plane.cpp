#include "plane.hpp"

namespace liby {
namespace math {
Plane::Plane(float nx, float ny, float nz, float d)
    : x_(nx), y_(ny), z_(nz), w_(d) {}

Plane::Plane(const Vector3D &v, float d)
    : x_(v[0]), y_(v[1]), z_(v[2]), w_(d) {}

float &Plane::operator[](int i) {
  if (i >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return ((&x_)[i]);
}

const float &Plane::operator[](int i) const {
  if (i >= 4) {
    throw std::runtime_error("Index out of bounds");
  }
  return ((&x_)[i]);
}

const float &Plane::x() const { return x_; }
const float &Plane::y() const { return y_; }
const float &Plane::z() const { return z_; }
const float &Plane::w() const { return w_; }

const Vector3D &Plane::getNormal(void) const {
  return (reinterpret_cast<const Vector3D &>(x_));
}

float dot(const Plane &p, const Vector3D &v) {
  return (p.x_ * v[0] + p.y_ * v[1] + p.z_ * v[2]);
}

bool intersectLine(const Plane &f, const Point3D &p, const Vector3D &v,
                   Point3D *q) {
  auto fv = dot(f, v);
  if (fabs(fv) > 0.0001) {
    *q = p - v * (dot(f, p) / fv);
    return true;
  }
  return false;
}

float dot(const Plane &f, const Point3D &p) {
  return (f.x_ * p[0] + f.y_ * p[1] + f.z_ * p[2]);
}

bool intersectThreePlanes(const Plane &f1, const Plane &f2, const Plane &f3,
                          Point3D *p) {
  auto n1 = f1.getNormal();
  auto n2 = f2.getNormal();
  auto n3 = f3.getNormal();
  auto n1xn2 = cross(n1, n2);
  auto det = dot(n1xn2, n3);
  if (fabs(det) > 0.0001) {
    *p = (cross(n3, n2) * f1.w_ + cross(n1, n3) * f2.w_ - n1xn2 * f3.w_) / det;
    return true;
  }
  return false;
}

bool intersectTwoPlanes(const Plane &f1, const Plane &f2, Point3D *p,
                        Vector3D *v) {
  auto n1 = f1.getNormal();
  auto n2 = f2.getNormal();
  *v = cross(n1, n2);
  auto det = dot(*v, *v);
  if (fabs(det) > 0.0001) {
    *p = (cross(*v, n2) * f1.w_ + cross(n1, *v) * f2.w_) / det;
    return true;
  }
  return false;
}
} // namespace math
} // namespace liby
