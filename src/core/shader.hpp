#pragma once

#include <Eigen/Eigen>
#include "vertex.hpp"
#include "payload.hpp"
#include "light.hpp"

class Shader {
public:
    Shader() {}

    ~Shader() {}
};

class VertexShader : public Shader {
public:
    Eigen::Matrix4f modelMatrix;
    Eigen::Matrix4f viewMatrix;
    Eigen::Matrix4f projectMatrix;

    virtual void setModelMatrix(const Eigen::Matrix4f &model) = 0;

    virtual void setViewMatrix(const Eigen::Matrix4f &view) = 0;

    virtual void setProjectMatrix(const Eigen::Matrix4f &project) = 0;

    virtual Payload shade(const Vertex &vertex) = 0;
};

class FragmentShader : public Shader {
public:
    std::vector<Light *> lights;
    Eigen::Vector3f view_pos;

    virtual Eigen::Vector4f shade(const Payload &payload) = 0;
};