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
    std::vector<Eigen::Vector4f> framebuffer;
    std::vector<float> depthBuffer;

    VertexShader* vertexshader;

    Eigen::Matrix4f viewPortMatrix; // NDC to screen space

    Rasterizer(int width, int height);
    ~Rasterizer();

    std::vector<Eigen::Vector4f> &render(std::vector<Vertex> &vertices);
    void setVertexShader(VertexShader *vs);
    void clearFrameBuffer();
    void clearDepthBuffer();
    void setPixel(int x, int y, Eigen::Vector4f color);
};