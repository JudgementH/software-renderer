#include "renderer.hpp"

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
}

std::vector<Eigen::Vector4f> &Rasterizer::render(std::vector<Vertex> &vertices)
{
    if (vertices.size() % 3 != 0)
    {
        throw "目前只支持绘制三角形";
    }
    if (vertexShader == nullptr)
    {
        throw "没有VertexShader";
    }

    std::vector<Vertex> ctx_v;
    for (auto &v : vertices)
    {
        auto temp_v = vertexShader->shade(v);
        temp_v.position = viewPortMatrix * temp_v.position;
        ctx_v.emplace_back(temp_v);
    }

    if (renderMode == RenderMode::VERTEX)
    {
        renderVertex(ctx_v);
    }
    else if (renderMode == RenderMode::EDGE)
    {
        renderEdge(ctx_v);
    }
    else if (renderMode == RenderMode::FACE)
    {
        renderFace(ctx_v);
    }

    return framebuffer;
}

void Rasterizer::renderVertex(std::vector<Vertex> &vertices)
{
    for (auto &v : vertices)
    {
        setPixel(v.position.x(), v.position.y(), v.color);
    }
}

void Rasterizer::renderEdge(std::vector<Vertex> &vertices)
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        Triangle tri(vertices[i], vertices[i + 1], vertices[i + 2]);
        auto tri_vertices = tri.vertices;
        for (int j = 0; j < 3; j++)
        {
            Vertex v0 = tri_vertices[j];
            Vertex v1 = tri_vertices[(j + 1) % 3];
            drawLine(v0.position.x(), v0.position.y(), v1.position.x(), v1.position.y(), v0.color);
        }
    }
}

void Rasterizer::renderFace(std::vector<Vertex> &vertices)
{
    for (int i = 0; i < vertices.size(); i += 3)
    {
        Triangle triangle(vertices[i], vertices[i + 1], vertices[i + 2]);
        drawTriangle(triangle);
    }
}

void Rasterizer::setVertexShader(std::unique_ptr<VertexShader> &vs)
{
    vertexShader = std::move(vs);
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

void Rasterizer::drawLine(int x0, int y0, int x1, int y1, const Eigen::Vector4f color)
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

void Rasterizer::drawTriangle(const Triangle triangle)
{
    int boxMinX = triangle.boxMin.x();
    int boxMinY = triangle.boxMin.y();

    int boxMaxX = triangle.boxMax.x();
    int boxMaxY = triangle.boxMax.y();

    for (int y = boxMinY; y < boxMaxY; y++)
    {
        for (int x = boxMinX; x < boxMaxX; x++)
        {
            if (triangle.inside(x, y))
            {
                setPixel(x, y, (Eigen::Vector4f::Random() + Eigen::Vector4f::Ones()) / 2.0f);
            }
        }
    }
}

void Rasterizer::setRenderMode(RenderMode mode)
{
    renderMode = mode;
}