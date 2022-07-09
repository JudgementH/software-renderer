#include "vertex_shader.hpp"

NaiveVertexShader::NaiveVertexShader(Eigen::Matrix4f model, Eigen::Matrix4f view, Eigen::Matrix4f project)
    : modelMatrix(model), viewMatrix(view), projectMatrix(project)
{
}

void NaiveVertexShader::setModelMatrix(const Eigen::Matrix4f &model)
{
    modelMatrix = model;
}

void NaiveVertexShader::setViewMatrix(const Eigen::Matrix4f &view)
{
    viewMatrix = view;
}

void NaiveVertexShader::setProjectMatrix(const Eigen::Matrix4f &project)
{
    projectMatrix = project;
}

Vertex NaiveVertexShader::shade(const Vertex &vertex)
{
    Eigen::Vector4f NDC_pos = projectMatrix * viewMatrix * modelMatrix * vertex.position;
    NDC_pos = NDC_pos / NDC_pos.w();
    return Vertex(NDC_pos, vertex.color, vertex.normal);
}