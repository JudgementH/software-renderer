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

Payload NaiveVertexShader::shade(const Vertex &vertex) {
    /**
     * turn model space to Frustum space.
     * NDC space == Frustum space / w
     */
    Payload p;
    p.worldPos = modelMatrix * vertex.position;
    p.viewPos = viewMatrix * p.worldPos;
    p.clipPos = projectMatrix * p.viewPos;
    p.normal = vertex.normal;
    p.color = vertex.color;
    return p;
}