#include <omp.h>
#include "renderer.hpp"
#include "payload.hpp"

Rasterizer::Rasterizer(int width, int height) : width(width), height(height)
{
    aspect_ratio = static_cast<float>(width) / height;
    framebuffer.resize(width * height);
    zBuffer.resize(width * height);
    std::fill(framebuffer.begin(), framebuffer.end(), Eigen::Vector4f(0, 0, 0, 0));
    std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::lowest());

    viewPortMatrix = math::GetViewPortMatrix(width, height);

    // TODO: default vertex shader

    // TODO: defalut fragment shader
}

Rasterizer::~Rasterizer()
{
}

std::vector<Eigen::Vector4f> &Rasterizer::render(std::vector<Vertex> &vertices)
{
    // TODO: 有待添加重载方法 model
    if (vertices.size() % 3 != 0)
    {
        throw "目前只支持绘制三角形";
    }
    if (vertexShader == nullptr)
    {
        throw "没有VertexShader";
    }

    std::vector<Payload> payloads;
    for (auto &v : vertices)
    {
        Vertex temp_v = vertexShader->shade(v);
        Eigen::Vector4f worldPos = temp_v.position;
        Eigen::Vector4f windowPos = viewPortMatrix * temp_v.position;
        Eigen::Vector3f normal = temp_v.normal;
        Eigen::Vector4f color = temp_v.color;
        payloads.emplace_back(worldPos, windowPos, color, normal);
    }

    if (renderMode == RenderMode::VERTEX)
    {
        renderVertex(payloads);
    }
    else if (renderMode == RenderMode::EDGE)
    {

        renderEdge(payloads);
    }
    else if (renderMode == RenderMode::FACE)
    {
        renderFace(payloads);
    }

    return framebuffer;
}

void Rasterizer::renderVertex(std::vector<Payload> &payloads)
{
    for (auto &p : payloads)
    {
        setPixel(p.windowPos.x(), p.windowPos.y(), p.color);
    }
}

void Rasterizer::renderEdge(std::vector<Payload> &payloads)
{
    for (int i = 0; i < payloads.size(); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            auto v0_pos = payloads[i + j].windowPos;
            auto v1_pos = payloads[i + (j + 1) % 3].windowPos;
            drawLine(v0_pos.x(), v0_pos.y(), v1_pos.x(), v1_pos.y(), payloads[i + j].color);
        }
    }
}

void Rasterizer::renderFace(std::vector<Payload> &payloads)
{
    for (int i = 0; i < payloads.size(); i += 3)
    {
        drawTriangle(payloads[i], payloads[i + 1], payloads[i + 2]);
    }
}

void Rasterizer::setVertexShader(std::unique_ptr<VertexShader> &vs)
{
    vertexShader = std::move(vs);
}

void Rasterizer::setFragmentShader(std::unique_ptr<FragmentShader> &fs)
{
    fragmentShader = std::move(fs);
}

void Rasterizer::setViewMatrix(Eigen::Matrix4f &view)
{
    vertexShader->setViewMatrix(view);
}

void Rasterizer::setProjectMatrix(Eigen::Matrix4f &project)
{
    vertexShader->setProjectMatrix(project);
}

void Rasterizer::clearFrameBuffer()
{
    std::fill(framebuffer.begin(), framebuffer.end(), Eigen::Vector4f(0, 0, 0, 0));
}

void Rasterizer::clearDepthBuffer()
{
    std::fill(zBuffer.begin(), zBuffer.end(), std::numeric_limits<float>::lowest());
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

    // TODO: 目前为随机值，有待修改
    Eigen::Vector4f color = (Eigen::Vector4f::Random() + Eigen::Vector4f::Ones()) / 2.0f;

    int boxMinX = triangle.boxMin.x();
    int boxMinY = triangle.boxMin.y();

    int boxMaxX = triangle.boxMax.x();
    int boxMaxY = triangle.boxMax.y();

    for (int y = boxMinY; y <= boxMaxY; y++)
    {
        for (int x = boxMinX; x <= boxMaxX; x++)
        {
            if (triangle.inside(x, y))
            {
                setPixel(x, y, color);
            }
        }
    }
}

void Rasterizer::drawTriangle(const Payload &payload0, const Payload &payload1, const Payload &payload2)
{
    Triangle triangle(Vertex(payload0.windowPos, payload0.color, payload0.normal),
                      Vertex(payload1.windowPos, payload1.color, payload1.normal),
                      Vertex(payload2.windowPos, payload2.color, payload2.normal));

    int boxMinX = int(triangle.boxMin.x());
    int boxMaxX = int(triangle.boxMax.x());

    int boxMinY = int(triangle.boxMin.y());
    int boxMaxY = int(triangle.boxMax.y());

    boxMinX = std::clamp(boxMinX, 0, width);
    boxMaxX = std::clamp(boxMaxX, 0, width);
    boxMinY = std::clamp(boxMinY, 0, width);
    boxMaxY = std::clamp(boxMaxY, 0, width);

    // TODO: 并行化
    for (int y = boxMinY; y <= boxMaxY; y++)
    {
        for (int x = boxMinX; x <= boxMaxX; x++)
        {
            // x = std::clamp(0, width - 1, x);
            // y = std::clamp(0, height - 1, y);
            if (triangle.inside(x, y))
            {
                auto [w0, w1, w2] = triangle.computeBarycentric2D(x, y);
                Payload p = w0 * payload0 + w1 * payload1 + w2 * payload2;
                if (fragmentShader == nullptr)
                {
                    std::cout << "fragmentShader 为空" << std::endl;
                    throw std::exception();
                }

                //深度测试
                int index = x + y * width;
                if (p.windowPos.z() > zBuffer[index])
                {
                    zBuffer[index] = p.windowPos.z();
                    auto color = fragmentShader->shade(p);
                    setPixel(x, y, color);
                }
            }
        }
    }
}

void Rasterizer::setRenderMode(RenderMode mode)
{
    renderMode = mode;
}