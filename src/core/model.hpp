#pragma once

#include <vector>
#include <string>
#include "triangle.hpp"

//TODO: 改为vertex和index类型
class Model
{
public:
    int shape_num;
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    Model(const std::string& filename);
};