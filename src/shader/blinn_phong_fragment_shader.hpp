#pragma once
#include "../core/shader.hpp"

class BlinnPhongFragmentShader : public FragmentShader
{
public:
    Eigen::Vector4f shade(const Payload &payload);
};
