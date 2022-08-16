#pragma once

#include <Eigen/Eigen>
#include "texture.hpp"

class Payload {
public:
    Eigen::Vector4f worldPos;
    Eigen::Vector4f viewPos;
    Eigen::Vector4f clipPos;
    Eigen::Vector4f NDCPos;
    Eigen::Vector4f windowPos;
    Eigen::Vector4f color = Eigen::Vector4f::Ones();
    Eigen::Vector3f normal = Eigen::Vector3f::Zero();
    Eigen::Vector2f texcood = Eigen::Vector2f::Zero();
    Texture *texture = nullptr;
    Texture *shadow = nullptr;
    Eigen::Vector4f lightPos;

    Payload() = default;

    Payload(Eigen::Vector4f worldPos,
            Eigen::Vector4f viewPos,
            Eigen::Vector4f clipPos,
            Eigen::Vector4f NDCPos,
            Eigen::Vector4f windowPos,
            Eigen::Vector4f color,
            Eigen::Vector3f normal);

    Payload(Eigen::Vector4f worldPos,
            Eigen::Vector4f viewPos,
            Eigen::Vector4f clipPos,
            Eigen::Vector4f NDCPos,
            Eigen::Vector4f windowPos,
            Eigen::Vector4f color,
            Eigen::Vector3f normal,
            Eigen::Vector2f texcood);

    static Payload Lerp(const Payload &payload1, const Payload &payload2, const float &t);

    Payload operator*(const float &num) const;

    friend Payload operator*(const float &num, const Payload &payload);

    Payload operator/(const float &num) const;

    Payload operator+(const Payload &payload) const;

    Payload operator-(const Payload &payload) const;
};