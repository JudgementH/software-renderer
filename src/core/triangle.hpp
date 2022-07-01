#pragma once
#include "vertex.hpp"

class Triangle
{
public:
    Vertex vertices[3];

    Triangle(Vertex v0, Vertex v1, Vertex v2);
};