#pragma once
#include <vector>
#include "vertex.hpp"

class Triangle
{
public:
    Vertex vertices[3];
    Eigen::Vector3f boxMin;
    Eigen::Vector3f boxMax;

    Triangle(Vertex v0, Vertex v1, Vertex v2);

    bool inside(float x,float y) const;
};