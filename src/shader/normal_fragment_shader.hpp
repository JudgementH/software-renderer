#pragma once
#include "../core/shader.hpp"

class NormalFragmentShader : public FragmentShader
{
public:
    Eigen::Vector4f shade(const Payload &payload);
};