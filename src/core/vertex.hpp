#pragma once

#include <Eigen/Eigen>

class Vertex
{
public:
  Eigen::Vector4f position;
  Eigen::Vector4f color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  Eigen::Vector3f normal;

  Vertex() {}
  ~Vertex() {}

  Vertex(const Eigen::Vector4f position,
         const Eigen::Vector4f color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f))
      : position(position),
        color(color)
  {
  }

  Vertex(const Eigen::Vector4f position,
         const Eigen::Vector4f color,
         const Eigen::Vector3f normal)
      : position(position),
        color(color),
        normal(normal)
  {
  }
};