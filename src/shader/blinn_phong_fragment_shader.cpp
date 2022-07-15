#include "blinn_phong_fragment_shader.hpp"

Eigen::Vector4f BlinnPhongFragmentShader::shade(const Payload &payload) {
    return payload.texture->getColor(payload.texcood.x(),payload.texcood.y());
}
