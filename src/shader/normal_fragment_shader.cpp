#include <iostream>
#include "normal_fragment_shader.hpp"

Eigen::Vector4f NormalFragmentShader::shade(const Payload &payload) {
    Eigen::Vector3f color = (payload.normal.head<3>().normalized() + Eigen::Vector3f::Ones()) / 2.0;
    return Eigen::Vector4f(color.x(), color.y(), color.z(), 1.0f);
}