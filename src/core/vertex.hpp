#pragma once

#include <Eigen/Eigen>

class Vertex
{
public:
  Eigen::Vector4f position;
  Eigen::Vector4f color;
  Eigen::Vector3f normal;

  ~Vertex() {}

  Vertex(const Eigen::Vector4f position,
         const Eigen::Vector4f color,
         const Eigen::Vector3f normal)
      : position(position),
        color(color),
        normal(normal)
  {
  }
};