#pragma once

#include <Eigen/Eigen>
#include <iostream>
#include <memory>
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

enum class RenderMode
{
    VERTEX,
    EDGE,
    FACE
};

class Rasterizer : Renderer
{
public:
    RenderMode renderMode = RenderMode::EDGE;
    int width;
    int height;
    float aspect_ratio;
    std::vector<Eigen::Vector4f> framebuffer;
    std::vector<float> depthBuffer;

    std::unique_ptr<VertexShader> vertexshader;

    Eigen::Matrix4f viewPortMatrix; // NDC to screen space

    Rasterizer(int width, int height);
    ~Rasterizer();

    std::vector<Eigen::Vector4f> &render(std::vector<Vertex> &vertices);
    void renderVertex(std::vector<Vertex> &vertices);
    void renderEdge(std::vector<Vertex> &vertices);
    

    void drawLine(int x0, int y0, int x1, int y1, const Eigen::Vector4f color);
    void drawTriangle(const Triangle triangle);

    void setVertexShader(std::unique_ptr<VertexShader> &vs);
    void clearFrameBuffer();
    void clearDepthBuffer();
    void setPixel(int x, int y, Eigen::Vector4f color);
    void setRenderMode(RenderMode mode);
};