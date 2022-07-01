#pragma once

#include <Eigen/Eigen>
#include <iostream>
#include "vertex.hpp"
#include "shader.hpp"
#include "triangle.hpp"
#include "utils.hpp"

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

    VertexShader *vertexshader;

    Eigen::Matrix4f viewPortMatrix; // NDC to screen space

    Rasterizer(int width, int height);
    ~Rasterizer();

    std::vector<Eigen::Vector4f> &render(std::vector<Vertex> &vertices);
    void drawLine(int x0, int y0, int x1, int y1,const Eigen::Vector4f color);
    void drawTriangle(const Triangle triangle);

    void setVertexShader(VertexShader *vs);
    void clearFrameBuffer();
    void clearDepthBuffer();
    void setPixel(int x, int y, Eigen::Vector4f color);
};