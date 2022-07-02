#pragma once

#include <Eigen/Eigen>

class Vertex
{
public:
  Eigen::Vector4f position;
  Eigen::Vector4f color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
  Eigen::Vector3f normal = Eigen::Vector3f::Zero();

  Vertex() {}
  ~Vertex() {}

  Vertex(const Eigen::Vector4f position,
         const Eigen::Vector4f color = Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
         const Eigen::Vector3f normal = Eigen::Vector3f::Zero())
      : position(position),
        color(color),
        normal(normal)
  {
  }
};