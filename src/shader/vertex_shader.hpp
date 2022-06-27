#pragma once
#include <Eigen/Eigen>
#include "../core/shader.hpp"
#include "../core/vertex.hpp"

class NaiveVertexShader : public VertexShader
{
public:
    Eigen::Matrix4f modelMatrix = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f viewMatrix = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f projectMatrix = Eigen::Matrix4f::Identity();

    NaiveVertexShader(Eigen::Matrix4f model = Eigen::Matrix4f::Identity(),
                      Eigen::Matrix4f view = Eigen::Matrix4f::Identity(),
                      Eigen::Matrix4f project = Eigen::Matrix4f::Identity());
    ~NaiveVertexShader();

    Vertex shade(const Vertex &vertex);

    void setModelMatrix(const Eigen::Matrix4f &model);
    void setViewMatrix(const Eigen::Matrix4f &view);
    void setProjectMatrix(const Eigen::Matrix4f &project);
};