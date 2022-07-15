#pragma once
#include <Eigen/Eigen>
#include "../core/shader.hpp"
#include "../core/vertex.hpp"

class NaiveVertexShader : public VertexShader
{
public:
    NaiveVertexShader(Eigen::Matrix4f model = Eigen::Matrix4f::Identity(),
                      Eigen::Matrix4f view = Eigen::Matrix4f::Identity(),
                      Eigen::Matrix4f project = Eigen::Matrix4f::Identity());

    Payload shade(const Vertex &vertex) override;

    void setModelMatrix(const Eigen::Matrix4f &model) override;
    void setViewMatrix(const Eigen::Matrix4f &view) override;
    void setProjectMatrix(const Eigen::Matrix4f &project) override;
};