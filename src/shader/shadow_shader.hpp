#include "../core/shader.hpp"

class ShadowShader : public FragmentShader {
public:
    Eigen::Vector4f shade(const Payload &payload) override;
};