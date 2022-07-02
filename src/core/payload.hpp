#pragma once
#include <Eigen/Eigen>

class Payload
{
public:
    Eigen::Vector4f worldPos;
    Eigen::Vector4f windowPos;
    Eigen::Vector4f color;
    Eigen::Vector3f noraml;
    Eigen::Vector2f texcood;

    Payload() = default;

    static Payload Lerp(const Payload &payload1, const Payload &payload2, const float &t);
};