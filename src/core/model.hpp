#pragma once

#include <vector>
#include <string>
#include <optional>
#include "triangle.hpp"
#include "texture.hpp"

class Model
{
public:
    int shape_num{};
    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::optional<Texture> texture = std::nullopt;

    Model(const std::string& filename);

    Model(std::vector<Vertex> vertices,std::vector<int> indices);

    void setTexture(Texture tex);
};