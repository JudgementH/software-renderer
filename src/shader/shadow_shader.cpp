#include "shadow_shader.hpp"

#define near -1.0f
#define far -50.0f

Eigen::Vector4f ShadowShader::shade(const Payload &payload) {
    if ((payload.worldPos - Eigen::Vector4f{2.77638, 0, 2.7866, 1}).norm() < 1e-2) {
        int a = 1;
    }
    float depth = payload.NDCPos.z();
    depth = (depth + 1) / 2;
    Eigen::Vector4f color{depth, depth, depth, 1.0f};
    return color;
}
