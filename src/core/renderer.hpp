#pragma once

#include <Eigen/Eigen>
#include "vertex.hpp"
#include "shader.hpp"

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;
};

class Rasterizer : Renderer
{
public:
    int width;
    int height;
    float aspect_ratio;
    std::vector<Eigen::Vector4i> framebuffer;
    std::vector<float> depthBuffer;

    VertexShader* vertexshader;

    Eigen::Matrix4f viewPortMatrix; // NDC to screen space

    Rasterizer(int width, int height);
    ~Rasterizer();

    std::vector<Eigen::Vector4i> &render(std::vector<Vertex> &vertices);
    void setVertexShader(VertexShader *vs);
};