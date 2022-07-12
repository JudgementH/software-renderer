#include "vertex_shader.hpp"

NaiveVertexShader::NaiveVertexShader(Eigen::Matrix4f model, Eigen::Matrix4f view, Eigen::Matrix4f project) {
    this->modelMatrix = model;
    this->viewMatrix = view;
    this->projectMatrix = project;
}

void NaiveVertexShader::setModelMatrix(const Eigen::Matrix4f &model) {
    modelMatrix = model;
}

void NaiveVertexShader::setViewMatrix(const Eigen::Matrix4f &view) {
    viewMatrix = view;
}

void NaiveVertexShader::setProjectMatrix(const Eigen::Matrix4f &project) {
    projectMatrix = project;
}

Vertex NaiveVertexShader::shade(const Vertex &vertex) {
    /**
     * turn model space to Frustum space.
     * NDC space == Frustum space / w
     */
    Eigen::Vector4f pos = projectMatrix * viewMatrix * modelMatrix * vertex.position;
    return {pos, vertex.color, vertex.normal};
}