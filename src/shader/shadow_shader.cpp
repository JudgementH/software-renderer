#include "shadow_shader.hpp"

Eigen::Vector4f ShadowShader::shade(const Payload &payload) {
    float depth = payload.windowPos.z();
    depth = (depth + 1) / 2.0;
    Eigen::Vector4f color{depth, depth, depth, 1.0f};
    return color;
}
