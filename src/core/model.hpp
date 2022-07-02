#pragma once

#include <vector>
#include <string>
#include "triangle.hpp"

//TODO: 改为vertex和index类型
class Model
{
public:
    int shape_num;
    std::vector<Triangle> faces;
    std::vector<Vertex> allVertices;

    Model(const std::string filename);
};