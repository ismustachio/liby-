#pragma once

#include "line.hpp"
#include "material.hpp"
#include "matrix4D.hpp"
#include "vector3D.hpp"
#include <memory>

namespace raytracer {
class ShapeManager;
class Shape {
public:
  Shape();
  Shape(float id, float radius, Material material,
        std::unique_ptr<mathengine::Transform4D> matrix,
        mathengine::Point3D center, std::unique_ptr<ShapeManager> manager);
  virtual ~Shape();

  virtual mathengine::Vector3D normal(const mathengine::Point3D &) const;
  virtual std::vector<float> intersect(const mathengine::Line &) const;

protected:
  float id_;
  float radius_;
  Material material_;
  mathengine::Point3D center_;
  std::unique_ptr<mathengine::Transform4D> matrix_;
  std::unique_ptr<ShapeManager> manager_;
};

class ShapeManager {
public:
  ShapeManager();
  virtual ~ShapeManager();

  virtual mathengine::Vector3D normal(const Shape &,
                                      const mathengine::Point3D &) = 0;
  virtual std::vector<float> intersect(const Shape &, const mathengine::Line &);
};

class SphereManager : public ShapeManager {
public:
  SphereManager();
  mathengine::Vector3D normal(const Shape &, const mathengine::Point3D &);
  std::vector<float> intersect(const Shape &, const mathengine::Line &);
};

class Sphere : public Shape {
public:
  Sphere();
  Sphere(Material material, std::unique_ptr<mathengine::Transform4D> matrix,
         std::unique_ptr<SphereManager> manager);

  mathengine::Vector3D normal(const mathengine::Point3D &) const;
  std::vector<float> intersect(const mathengine::Line &) const;
};

class PlaneManager : public ShapeManager {
public:
  PlaneManager();
  mathengine::Vector3D normal(const Shape &, const mathengine::Point3D &);
  std::vector<float> intersect(const Shape &, const mathengine::Line &);
};

class Plane : public Shape {
public:
  Plane();
  Plane(Material material, std::unique_ptr<mathengine::Transform4D> matrix,
        std::unique_ptr<PlaneManager> manager);

  mathengine::Vector3D normal(const mathengine::Point3D &) const;
  std::vector<float> intersect(const mathengine::Line &) const;
};

class CylinderManager : public ShapeManager {
public:
  CylinderManager();
  mathengine::Vector3D normal(const Shape &, const mathengine::Point3D &);
  std::vector<float> intersect(const Shape &, const mathengine::Line &);
};

class Cylinder : public Shape {
public:
  Cylinder();
  Cylinder(Material material, std::unique_ptr<mathengine::Transform4D> matrix,
           std::unique_ptr<CylinderManager> manager);

  mathengine::Vector3D normal(const mathengine::Point3D &) const;
  std::vector<float> intersect(const mathengine::Line &) const;

protected:
  float minimum;
  float maximum;
  bool isClosed;
};

class ConeManager : public ShapeManager {
public:
  ConeManager();
  mathengine::Vector3D normal(const Shape &, const mathengine::Point3D &);
  std::vector<float> intersect(const Shape &, const mathengine::Line &);
};

class Cone : public Cylinder {
public:
  Cone();
  Cone(Material material, std::unique_ptr<mathengine::Transform4D> matrix,
       std::unique_ptr<ConeManager> manager);

  mathengine::Vector3D normal(const mathengine::Point3D &) const;
  std::vector<float> intersect(const mathengine::Line &) const;
};

class CubeManager : public ShapeManager {
public:
  CubeManager();
  mathengine::Vector3D normal(const Shape &, const mathengine::Point3D &);
  std::vector<float> intersect(const Shape &, const mathengine::Line &);
};

class Cube : public Shape {
public:
  Cube();
  Cube(Material material, std::unique_ptr<mathengine::Transform4D> matrix,
       std::unique_ptr<CubeManager> manager);

  mathengine::Vector3D normal(const mathengine::Point3D &) const;
  std::vector<float> intersect(const mathengine::Line &) const;
};
} // namespace raytracer
