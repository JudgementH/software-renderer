#include <omp.h>
#include "renderer.hpp"
#include "payload.hpp"

const std::vector<Eigen::Vector4f> planes = {
        // far
        Eigen::Vector4f(0, 0, -1, -1),
        // Near
        Eigen::Vector4f(0, 0, 1, -1),
        // left
        Eigen::Vector4f(-1, 0, 0, -1),
        // right
        Eigen::Vector4f(1, 0, 0, -1),
        // bottom
        Eigen::Vector4f(0, -1, 0, -1),
        // top
        Eigen::Vector4f(0, 1, 0, -1)
};


Rasterizer::Rasterizer(int width, int height) : width(width), height(height) {
    aspect_ratio = static_cast<float>(width) / height;
    framebuffer.resize(width * height);
    zBuffer.resize(width * height);
    std::fill(framebuffer.begin(), framebuffer.end(), Eigen::Vector4f(0, 0, 0, 0));
    std::fill(zBuffer.begin(), zBuffer.end(), -1.0f);

    viewPortMatrix = math::GetViewPortMatrix(width, height);

    // TODO: default vertex shader

    // TODO: defalut fragment shader
}

Rasterizer::Rasterizer(int width, int height, Camera *camera) : Rasterizer(width, height) {
    this->camera = camera;
}

Rasterizer::~Rasterizer() {
}

Payload Rasterizer::shadeVertex(const Vertex &vertex) {
    Payload p;
    p.worldPos = vertexShader->modelMatrix * vertex.position;
    p.viewPos = vertexShader->viewMatrix * p.worldPos;
    p.clipPos = vertexShader->projectMatrix * p.viewPos;
    p.normal = vertex.normal;
    p.color = vertex.color;
    return p;
}

std::vector<Eigen::Vector4f> &Rasterizer::render(Model &model) {
    if (vertexShader == nullptr) {
        std::cout << "没有vertexShader" << std::endl;
        throw std::exception();
    }

    // turn vertices from model space to NDC space
    std::vector<Payload> payloads;

    // TODO:可多线程
    for (int i = 0; i < model.vertices.size(); i++) {
        Payload p = shadeVertex(model.vertices[i]);
        payloads.emplace_back(p);
    }

    // 2. fragment shader
    if (renderMode == RenderMode::VERTEX) {
        renderVertex(payloads, model.indices);
    } else if (renderMode == RenderMode::EDGE) {
        renderEdge(payloads, model.indices);
    } else if (renderMode == RenderMode::FACE) {
        renderFace(payloads, model.indices);
    }

    return framebuffer;
}

void Rasterizer::renderVertex(std::vector<Payload> &payloads, const std::vector<int> &indices) {
    for (auto &p: payloads) {
        if (p.windowPos.x() >= 0 && p.windowPos.x() < width && p.windowPos.y() >= 0 && p.windowPos.y() < height) {
            setPixel(p.windowPos.x(), p.windowPos.y(), p.color);
        }
    }
}


void Rasterizer::renderEdge(std::vector<Payload> &payloads, const std::vector<int> &indices) {
    for (int i = 0; i < indices.size(); i += 3) {
        Payload p0 = payloads[indices[i]];
        Payload p1 = payloads[indices[i + 1]];
        Payload p2 = payloads[indices[i + 2]];
        if (camera->inFrustum(p0.clipPos, p1.clipPos, p2.clipPos)) {

            std::vector<Payload> ps = clipHomogeneous(p0, p1, p2);
            if (ps.empty()) {
                continue;
            }

            // 透视除法
            for (int i = 0; i < ps.size(); i++) {
                ps[i].NDCPos = ps[i].clipPos / ps[i].clipPos.w();
                ps[i].windowPos = viewPortMatrix * ps[i].NDCPos;
            }

            for (int j = 0; j < ps.size() - 2; j++) {
                Payload v0_pos = ps[0];
                Payload v1_pos = ps[j + 1];
                Payload v2_pos = ps[j + 2];
                drawLine(v0_pos.windowPos.x(), v0_pos.windowPos.y(),
                         v1_pos.windowPos.x(), v1_pos.windowPos.y(),
                         v0_pos.color);
                drawLine(v1_pos.windowPos.x(), v1_pos.windowPos.y(),
                         v2_pos.windowPos.x(), v2_pos.windowPos.y(),
                         v1_pos.color);
                drawLine(v2_pos.windowPos.x(), v2_pos.windowPos.y(),
                         v0_pos.windowPos.x(), v0_pos.windowPos.y(),
                         v2_pos.color);
            }
        }
    }
}

void Rasterizer::renderFace(std::vector<Payload> &payloads, const std::vector<int> &indices) {
    for (int i = 0; i < indices.size(); i += 3) {
        Payload p0 = payloads[indices[i]];
        Payload p1 = payloads[indices[i + 1]];
        Payload p2 = payloads[indices[i + 2]];

        // Frustum Culling
        if (camera->inFrustum(p0.clipPos, p1.clipPos, p2.clipPos)) {

            // Homogeneous Space Clipping
            std::vector<Payload> ps = clipHomogeneous(p0, p1, p2);

            if (ps.empty()) {
                continue;
            }

            // 透视除法
            for (int i = 0; i < ps.size(); i++) {
                ps[i].NDCPos = ps[i].clipPos / ps[i].clipPos.w();
                ps[i].windowPos = viewPortMatrix * ps[i].NDCPos;
            }

            for (int j = 0; j < ps.size() - 2; j++) {
                Payload t0 = ps[0];
                Payload t1 = ps[j + 1];
                Payload t2 = ps[j + 2];
                drawTriangle(t0, t1, t2);
            }

//            drawTriangle(p0, p1, p2);
        }
    }
}

void Rasterizer::setVertexShader(std::unique_ptr<VertexShader> &vs) {
    vertexShader = std::move(vs);
}

void Rasterizer::setFragmentShader(std::unique_ptr<FragmentShader> &fs) {
    fragmentShader = std::move(fs);
}

void Rasterizer::setViewMatrix(Eigen::Matrix4f &view) {
    vertexShader->setViewMatrix(view);
}

void Rasterizer::setProjectMatrix(Eigen::Matrix4f &project) {
    vertexShader->setProjectMatrix(project);
}

void Rasterizer::clearFrameBuffer() {
    std::fill(framebuffer.begin(), framebuffer.end(), Eigen::Vector4f(0, 0, 0, 0));
}

void Rasterizer::clearDepthBuffer() {
    std::fill(zBuffer.begin(), zBuffer.end(), -1.0f);
}

void Rasterizer::setPixel(int x, int y, Eigen::Vector4f color) {
    /**
     * @brief set Screen framebuffer
     *
     */
    x = (x == width) ? x - 1 : x;
    y = (y == height) ? y - 1 : y;
    if (x < 0 || x >= width || y < 0 || y >= height) {

        // std::cout << "My Exception setPixel() OutOfRange\n";
        return;
    }
    int index = x + y * width;
    framebuffer[index] = color;
}

void Rasterizer::drawLine(int x0, int y0, int x1, int y1, const Eigen::Vector4f color) {
    /**
     * @brief Bresenham’s 直线算法
     *
     */

    // k [+-1, +-infinity] to [0, +-1]
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int error = 0;
    int y = y0;

    // k [0, +-1] -> [0, 1]
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    if (steep) {
        for (int x = x0; x <= x1; x++) {
            setPixel(y, x, color);

            error += dy;
            if ((error << 1) > dx) {
                y += yi;
                error -= dx;
            }
        }
    } else {
        for (int x = x0; x <= x1; x++) {
            setPixel(x, y, color);

            error += dy;
            if ((error << 1) > dx) {
                y += yi;
                error -= dx;
            }
        }
    }
}

void Rasterizer::drawTriangle(const Triangle triangle) {

    // TODO: 目前为随机值，有待修改
    Eigen::Vector4f color = (Eigen::Vector4f::Random() + Eigen::Vector4f::Ones()) / 2.0f;

    int boxMinX = triangle.boxMin.x();
    int boxMinY = triangle.boxMin.y();

    int boxMaxX = triangle.boxMax.x();
    int boxMaxY = triangle.boxMax.y();

    for (int y = boxMinY; y <= boxMaxY; y++) {
        for (int x = boxMinX; x <= boxMaxX; x++) {
            if (triangle.inside(x, y)) {
                setPixel(x, y, color);
            }
        }
    }
}

void Rasterizer::drawTriangle(const Payload &payload0, const Payload &payload1, const Payload &payload2) {
    if (fragmentShader == nullptr) {
        std::cout << "fragmentShader 为空" << std::endl;
        throw std::exception();
    }

    Triangle triangle(Vertex(payload0.windowPos, payload0.color, payload0.normal),
                      Vertex(payload1.windowPos, payload1.color, payload1.normal),
                      Vertex(payload2.windowPos, payload2.color, payload2.normal));

    int boxMinX = int(triangle.boxMin.x());
    int boxMaxX = int(triangle.boxMax.x());

    int boxMinY = int(triangle.boxMin.y());
    int boxMaxY = int(triangle.boxMax.y());

    // TODO: 并行化 最后深度测试
    // TODO: 背面剔除
    for (int y = boxMinY; y <= boxMaxY; y++) {
        for (int x = boxMinX; x <= boxMaxX; x++) {

            if (triangle.inside(x, y)) {
                auto[w0, w1, w2] = triangle.computeBarycentric2D(x, y);
                Payload p = w0 * payload0 + w1 * payload1 + w2 * payload2;

                //深度测试
                if (p.windowPos.z() > getDepth(x, y)) {
                    setDepth(x, y, p.windowPos.z());
                    Eigen::Vector4f color = fragmentShader->shade(p);
                    setPixel(x, y, color);
                }
            }
        }
    }
}

void Rasterizer::setRenderMode(RenderMode mode) {
    renderMode = mode;
}

float Rasterizer::getDepth(const int &x, const int &y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return 1.0;
    }
    int index = x + y * width;
    return zBuffer[index];
}

void Rasterizer::setDepth(const int &x, const int &y, const float &depth) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    int index = x + y * width;
    zBuffer[index] = depth;
}

std::vector<Payload> Rasterizer::clipHomogeneous(const Payload &p0, const Payload &p1, const Payload &p2) {
    std::vector<Payload> output = {p0, p1, p2};

    for (int i = 0; i < planes.size(); i++) {
        std::vector<Payload> input(output);
        output.clear();

        for (int j = 0; j < input.size(); j++) {
            int current_index = j;
            int previous_index = (j + input.size() - 1) % input.size();

            Payload P = input[previous_index];
            Payload Q = input[current_index];

            // from P to Q
            float dis_P = math::ProjectDistance(planes[i], P.clipPos);
            float dis_Q = math::ProjectDistance(planes[i], Q.clipPos);

            if (dis_P * dis_Q < 0) {
                // cross the plane
                Payload intersect = Payload::Lerp(P, Q, (dis_P / (dis_P - dis_Q)));
                output.emplace_back(intersect);
            }

            if (dis_Q >= 0) {
                // Q in plane
                output.emplace_back(Q);
            }
        }
    }

    return output;
}


