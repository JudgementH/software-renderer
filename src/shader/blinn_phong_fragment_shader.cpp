#include "blinn_phong_fragment_shader.hpp"

Eigen::Vector4f BlinnPhongFragmentShader::shade(const Payload &payload) {
    Eigen::Vector4f albedo = payload.texture->getColor(payload.texcood.x(), payload.texcood.y());
    if (lights.empty()) {
        return albedo;
    }
    Eigen::Vector4f color = Eigen::Vector4f::Zero();

    Eigen::Vector3f view_dir = (view_pos - payload.worldPos.head<3>()).normalized();
    Eigen::Vector3f fragment_pos = payload.worldPos.head<3>();
//
    for (int i = 0; i < lights.size(); i++) {
        Eigen::Vector4f &light_color = lights[i]->color;
        Eigen::Vector3f light_dir = lights[i]->getDirection(fragment_pos);

        Eigen::Vector4f ambient =  Eigen::Vector4f(0.2f, 0.2f, 0.2f, 1.0f);
        Eigen::Vector4f diffuse = std::max(payload.normal.dot(light_dir), 0.0f) * light_color;
        Eigen::Vector4f specular = std::pow(std::max(0.0f, view_dir.dot(light_dir)), 32) * light_color;
        color += (ambient + diffuse + specular).cwiseProduct(albedo);
    }
    color = color / lights.size();
    return color;
}
