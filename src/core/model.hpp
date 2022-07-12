#pragma once

#include <vector>
#include <string>
#include "triangle.hpp"

class Model
{
public:
    int shape_num;
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    Model(const std::string& filename);
};