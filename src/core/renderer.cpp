#include "renderer.hpp"
#include "utils.hpp"

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

std::vector<Eigen::Vector4i> &Rasterizer::render(std::vector<Vertex> &vertices)
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
    }

    return framebuffer;
}

void Rasterizer::setVertexShader(VertexShader *vs)
{
    vertexshader = vs;
}