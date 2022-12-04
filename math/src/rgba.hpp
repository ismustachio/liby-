#pragma once

namespace liby {
namespace math {
class RGBA {
  RGBA() = default;
  RGBA(float r, float g, float b, float a = 1.0F);
  RGBA &operator*=(float s);
  RGBA &operator/=(float s);
  RGBA &operator+=(const RGBA &);
  RGBA &operator-=(const RGBA &);
  RGBA &operator*=(const RGBA &);

  friend RGBA operator*(const RGBA &, float);
  friend RGBA operator/(const RGBA &, float);
  friend RGBA operator*(const RGBA &, const RGBA &);
  friend RGBA operator/(const RGBA &, const RGBA &);
  friend RGBA operator+(const RGBA &, const RGBA &);
  friend RGBA operator-(const RGBA &, const RGBA &);

private:
  float red;
  float green;
  float blue;
  float alpha;
};
} // namespace math
} // namespace liby
