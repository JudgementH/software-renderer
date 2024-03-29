#pragma once

#include <Eigen/Eigen>
#include <iostream>
#include <memory>
#include "vertex.hpp"
#include "shader.hpp"
#include "triangle.hpp"
#include "utils.hpp"
#include "payload.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "scene.hpp"

class Renderer {
public:
    Renderer() = default;

    ~Renderer() = default;
};

enum class RenderMode {
    VERTEX,
    EDGE,
    FACE
};

class Rasterizer : Renderer {
private:

    void renderVertex(Payload &p0, Payload &p1, Payload &p2);

    void renderEdge(Payload &p0, Payload &p1, Payload &p2);

    void renderFace(Payload &p0, Payload &p1, Payload &p2);

    float getDepth(const int &x, const int &y);

    void setDepth(const int &x, const int &y, const float &depth);

    static std::vector<Payload> clipHomogeneous(const Payload &p0, const Payload &p1, const Payload &p2);

public:
    RenderMode renderMode = RenderMode::EDGE;
    int width;
    int height;
    float aspect_ratio;
    std::vector<Eigen::Vector4f> framebuffer;
    std::vector<float> zBuffer;
    std::unique_ptr<Texture> shadowMap = nullptr;
    Eigen::Matrix4f lightVP;

    Camera *camera;
    std::unique_ptr<VertexShader> vertexShader;
    std::unique_ptr<FragmentShader> fragmentShader;
    std::unique_ptr<FragmentShader> shadowShader;

    Eigen::Matrix4f viewPortMatrix; // NDC to screen space

    Rasterizer(int width, int height);

    Rasterizer(int width, int height, Camera *camera);

    ~Rasterizer();

    std::vector<Eigen::Vector4f> &render(Model &model);

    std::vector<Eigen::Vector4f> render(Scene &scene);

    void drawLine(int x0, int y0, int x1, int y1, Eigen::Vector4f color);

    void drawTriangle(Triangle triangle);

    void drawTriangle(const Payload &payload0, const Payload &payload1, const Payload &payload2);

    void setVertexShader(std::unique_ptr<VertexShader> &vs);

    void setFragmentShader(std::unique_ptr<FragmentShader> &fs);

    void setShadowShader(std::unique_ptr<FragmentShader> &ss);

    void setViewMatrix(Eigen::Matrix4f &view);

    void setProjectMatrix(Eigen::Matrix4f &project);

    void clearFrameBuffer();

    void clearDepthBuffer();

    void setPixel(int x, int y, Eigen::Vector4f color);

    void setRenderMode(RenderMode mode);
};