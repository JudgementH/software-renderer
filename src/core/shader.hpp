#pragma once
#include <Eigen/Eigen>
#include "vertex.hpp"
#include "payload.hpp"

class Shader
{
public:
    Shader() {}
    ~Shader() {}
};

class VertexShader : public Shader
{
public:
    virtual Vertex shade(const Vertex &vertex) = 0;
};

class FragmentShader : public Shader
{
public:
    virtual Eigen::Vector4f shade(const Payload &payload) = 0;
};