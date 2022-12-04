#include "rgba.hpp"

namespace liby {
namespace math {
RGBA::RGBA(float r, float g, float b, float a)
    : red(r), green(g), blue(b), alpha(a) {}

RGBA &RGBA::operator*=(float s) {
  red *= s;
  green *= s;
  blue *= s;
  alpha *= s;
  return *this;
}

RGBA &RGBA::operator/=(float s) {
  s = 1.0F / s;
  red *= s;
  green *= s;
  blue *= s;
  alpha *= s;
  return *this;
}

RGBA &RGBA::operator+=(const RGBA &c) {
  red += c.red;
  green += c.green;
  blue += c.blue;
  alpha += c.alpha;
  return *this;
}

RGBA &RGBA::operator-=(const RGBA &c) {
  red -= c.red;
  green -= c.green;
  blue -= c.blue;
  alpha -= c.alpha;
  return *this;
}

RGBA &RGBA::operator*=(const RGBA &c) {
  red *= c.red;
  green *= c.green;
  blue *= c.blue;
  alpha *= c.alpha;
  return *this;
}

RGBA operator*(const RGBA &c, float s) {
  return RGBA(c.red * s, c.green * s, c.blue * s, c.alpha * s);
}

RGBA operator/(const RGBA &c, float s) {
  s = 1.0F / s;
  return RGBA(c.red * s, c.green * s, c.blue * s, c.alpha * s);
}

RGBA operator*(const RGBA &c1, const RGBA &c2) {
  return RGBA(c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue,
              c1.alpha * c2.alpha);
}

RGBA operator/(const RGBA &c1, const RGBA &c2) {
  return RGBA(c1.red / c2.red, c1.green / c2.green, c1.blue / c2.blue,
              c1.alpha / c2.alpha);
}

RGBA operator+(const RGBA &c1, const RGBA &c2) {
  return RGBA(c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue,
              c1.alpha + c2.alpha);
}

RGBA operator-(const RGBA &c1, const RGBA &c2) {
  return RGBA(c1.red - c2.red, c1.green - c2.green, c1.blue - c2.blue,
              c1.alpha - c2.alpha);
}
} // namespace math
} // namespace liby
