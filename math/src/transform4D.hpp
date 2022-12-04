#pragma once
#include "matrix4D.hpp"

namespace liby {
namespace math {
class Transform4D : public Matrix4D {
public:
  Transform4D(float n00, float n01, float n02, float n03, float n10, float n11,
              float n12, float n13, float n20, float n21, float n22, float n23);
  Transform4D(const Vector3D &a, const Vector3D &b, const Vector3D &c,
              const Point3D &p);

  Vector3D &operator[](int j);
  const Vector3D &operator[](int j) const;

  const Point3D getTranslation(void) const;
  void setTranslsation(const Point3D &p);

  friend Vector3D operator*(const Transform4D &, const Vector3D &);
  friend Plane operator*(const Transform4D &, const Plane &);

  static Transform4D makeReflection(const Vector3D &);
  static Transform4D makeReflection(const Plane &);
  static Transform4D makeRotation(float, const Vector3D &);
  static Transform4D makeRotationX(float x);
  static Transform4D makeRotationY(float y);
  static Transform4D makeRotationZ(float z);
  static Transform4D makeScale(float s, const Vector3D &);
  static Transform4D makeScaleX(float x);
  static Transform4D makeScale(float s);
  static Transform4D makeScaleY(float y);
  static Transform4D makeScaleZ(float z);
  static Transform4D makeScale(float sx, float sy, float sz);
  static Transform4D makeSkew(float t, const Vector3D &, const Vector3D &);
};
} // namespace math
} // namespace liby
