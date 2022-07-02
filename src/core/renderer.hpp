#pragma once

#include <Eigen/Eigen>
#include <iostream>
#include <memory>
#include "vertex.hpp"
#include "shader.hpp"
#include "triangle.hpp"
#include "utils.hpp"
#include "payload.hpp"

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
private:
    void renderVertex(std::vector<Payload> &payloads);
    void renderEdge(std::vector<Payload> &payloads);
    void renderFace(std::vector<Payload> &payloads);

public:
    RenderMode renderMode = RenderMode::EDGE;
    int width;
    int height;
    float aspect_ratio;
    std::vector<Eigen::Vector4f> framebuffer;
    std::vector<float> zBuffer;

    std::unique_ptr<VertexShader> vertexShader;
    std::unique_ptr<FragmentShader> fragmentShader;

    Eigen::Matrix4f viewPortMatrix; // NDC to screen space

    Rasterizer(int width, int height);
    ~Rasterizer();

    std::vector<Eigen::Vector4f> &render(std::vector<Vertex> &vertices);

    void drawLine(int x0, int y0, int x1, int y1, const Eigen::Vector4f color);
    void drawTriangle(const Triangle triangle);
    void drawTriangle(const Payload &payload0, const Payload &payload1, const Payload &payload2);

    void setVertexShader(std::unique_ptr<VertexShader> &vs);
    void setFragmentShader(std::unique_ptr<FragmentShader> &fs);

    void clearFrameBuffer();
    void clearDepthBuffer();
    void setPixel(int x, int y, Eigen::Vector4f color);
    void setRenderMode(RenderMode mode);
};