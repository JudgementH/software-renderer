#pragma once

#include <vector>
#include <string>
#include "triangle.hpp"

class Model
{
public:
    int shape_num;
    std::vector<Triangle> faces;
    std::vector<Vertex> allVertices;

    Model(const std::string filename);
};