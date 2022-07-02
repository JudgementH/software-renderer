#include "payload.hpp"
#include "utils.hpp"

Payload Payload::Lerp(const Payload &payload1, const Payload &payload2, const float &t)
{
    Payload payload;
    payload.worldPos = math::LinearLerp<Eigen::Vector4f>(payload1.worldPos, payload2.worldPos, t);
    payload.windowPos = math::LinearLerp<Eigen::Vector4f>(payload1.windowPos, payload2.windowPos, t);
    payload.color = math::LinearLerp<Eigen::Vector4f>(payload1.color, payload2.color, t);
    payload.noraml = math::LinearLerp<Eigen::Vector3f>(payload1.noraml, payload2.noraml, t);
    payload.texcood = math::LinearLerp<Eigen::Vector2f>(payload1.texcood, payload2.texcood, t);
    return payload;
}