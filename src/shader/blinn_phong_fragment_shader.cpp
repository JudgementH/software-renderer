#include "blinn_phong_fragment_shader.hpp"

float EPS = 0.03;

float calculate_visibility_with_SM(const Payload &payload) {
    float visibility = 1.0;
    if ((payload.worldPos - Eigen::Vector4f{2.77638, 0, 2.7866, 1}).norm() < 1e-2) {
        float a = payload.NDCPos.z();
    }
    float shadow_depth = payload.shadow->getColor( payload.lightPos.x(), 1 - payload.lightPos.y()).x();
    float current_depth = payload.lightPos.z();
    if (current_depth < shadow_depth - EPS) {
        visibility = 0.3;
    }
    return visibility;
}

Eigen::Vector4f BlinnPhongFragmentShader::shade(const Payload &payload) {
    Eigen::Vector4f albedo{0.5, 0.5, 0.5, 1.0};
    if (payload.texture != nullptr) {
        albedo = payload.texture->getColor(payload.texcood.x(), payload.texcood.y());
    }
    float visibility = 1.0;
    if (payload.shadow != nullptr) {
        visibility = calculate_visibility_with_SM(payload);
    }
    if (lights.empty()) {
        return albedo;
    }
    Eigen::Vector4f color = Eigen::Vector4f::Zero();

    Eigen::Vector3f view_dir = (view_pos - payload.worldPos.head<3>()).normalized();
    Eigen::Vector3f fragment_pos = payload.worldPos.head<3>();


    for (int i = 0; i < lights.size(); i++) {
        Eigen::Vector4f &light_color = lights[i]->color;
        Eigen::Vector3f light_dir = lights[i]->getDirection(fragment_pos);

        Eigen::Vector4f ambient = Eigen::Vector4f(0.2f, 0.2f, 0.2f, 1.0f);
        Eigen::Vector4f diffuse = std::max(payload.normal.dot(light_dir), 0.0f) * light_color;
        Eigen::Vector4f specular = std::pow(std::max(0.0f, view_dir.dot(light_dir)), 32) * light_color;
        color += (ambient + diffuse + specular).cwiseProduct(albedo);
    }
    color = color / lights.size() * visibility;
    return color;
}
