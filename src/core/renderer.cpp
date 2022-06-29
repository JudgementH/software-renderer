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
    }

    for (int i = 0; i < res_vertices.size(); i++)
    {
        Vertex v0 = res_vertices[i];
        Vertex v1 = res_vertices[(i + 1) % 3];
        drawLine(v0.position.x(), v0.position.y(), v1.position.x(), v1.position.y(), v0.color);
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
    /**
     * @brief set Screen framebuffer
     *
     */
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

void Rasterizer::drawLine(int x0, int y0, int x1, int y1, Eigen::Vector4f color)
{
    /**
     * @brief Bresenham’s 直线算法
     *
     */

    
    // k [+-1, +-infty] to [0, +-1]
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int error = 0;
    int y = y0;

    // k [0, +-1] -> [0, 1]
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    if (steep)
    {
        for (int x = x0; x <= x1; x++)
        {
            setPixel(y, x, color);

            error += dy;
            if ((error << 1) > dx)
            {
                y += yi;
                error -= dx;
            }
        }
    }
    else
    {
        for (int x = x0; x <= x1; x++)
        {
            setPixel(x, y, color);

            error += dy;
            if ((error << 1) > dx)
            {
                y += yi;
                error -= dx;
            }
        }
    }
}