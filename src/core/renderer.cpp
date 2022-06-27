#include "renderer.hpp"
#include "utils.hpp"
#include <iostream>

Rasterizer::Rasterizer(int width, int height) : width(width), height(height)
{
    aspect_ratio = static_cast<float>(width) / height;
    framebuffer.resize(width * height);
    depthBuffer.resize(width * height);

    viewPortMatrix = math::GetViewPortMatrix(width, height);

    // default vertex shader
}

Rasterizer::~Rasterizer()
{
    if (vertexshader != nullptr)
    {
        delete vertexshader;
    }
}

std::vector<Eigen::Vector4f> &Rasterizer::render(std::vector<Vertex> &vertices)
{
    if (vertices.size() != 3)
    {
        throw "目前只支持画一个三角形";
    }
    if (vertexshader == nullptr)
    {
        throw "目前只支持画一个三角形";
    }

    std::vector<Vertex> res_vertices;
    for (auto &v : vertices)
    {
        res_vertices.emplace_back(vertexshader->shade(v));
    }

    // view port transform
    for (auto &v : res_vertices)
    {
        v.position = viewPortMatrix * v.position;
        setPixel(v.position.x(), v.position.y(), v.color);
    }

    return framebuffer;
}

void Rasterizer::setVertexShader(VertexShader *vs)
{
    vertexshader = vs;
}

void Rasterizer::clearFrameBuffer()
{
    std::fill(framebuffer.begin(), framebuffer.end(), Eigen::Vector4f(0, 0, 0, 0));
}

void Rasterizer::clearDepthBuffer()
{
    std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::infinity());
}

void Rasterizer::setPixel(int x, int y, Eigen::Vector4f color)
{
    x = (x == width) ? x - 1 : x;
    y = (y == height) ? y - 1 : y;
    if (x < 0 || x >= width || y < 0 || y >= height)
    {

        std::cout << "OutOfRange\n";
        throw std::exception();
    }
    int index = x + y * width;
    framebuffer[index] = color;
}