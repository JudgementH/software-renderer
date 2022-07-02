#include "payload.hpp"
#include "utils.hpp"

Payload::Payload(Eigen::Vector4f worldPos,
                 Eigen::Vector4f windowPos,
                 Eigen::Vector4f color,
                 Eigen::Vector3f normal,
                 Eigen::Vector2f texcood)
    : worldPos(worldPos),
      windowPos(windowPos),
      color(color),
      normal(normal),
      texcood(texcood)
{
}

Payload Payload::Lerp(const Payload &payload1, const Payload &payload2, const float &t)
{
  Payload payload;
  payload.worldPos = math::LinearLerp<Eigen::Vector4f>(payload1.worldPos, payload2.worldPos, t);
  payload.windowPos = math::LinearLerp<Eigen::Vector4f>(payload1.windowPos, payload2.windowPos, t);
  payload.color = math::LinearLerp<Eigen::Vector4f>(payload1.color, payload2.color, t);
  payload.normal = math::LinearLerp<Eigen::Vector3f>(payload1.normal, payload2.normal, t);
  payload.texcood = math::LinearLerp<Eigen::Vector2f>(payload1.texcood, payload2.texcood, t);
  return payload;
}

Payload Payload::operator*(const float &num) const
{
  Payload payload;
  payload.worldPos *= num;
  payload.windowPos *= num;
  payload.color *= num;
  payload.normal *= num;
  payload.texcood *= num;
  return payload;
}

Payload operator*(const float &num, const Payload &payload)
{
  Payload p = payload;
  p.worldPos *= num;
  p.windowPos *= num;
  p.color *= num;
  p.normal *= num;
  p.texcood *= num;
  return p;
}

Payload Payload::operator/(const float &num) const
{
  Payload payload = *this;
  payload.worldPos /= num;
  payload.windowPos /= num;
  payload.color /= num;
  payload.normal /= num;
  payload.texcood /= num;
  return payload;
}

Payload Payload::operator+(const Payload &payload) const
{
  Payload res = *this;
  res.worldPos += payload.worldPos;
  res.windowPos += payload.windowPos;
  res.color += payload.color;
  res.normal += payload.normal;
  res.texcood += payload.texcood;
  return res;
}

Payload Payload::operator-(const Payload &payload) const
{
  Payload res = *this;
  res.worldPos -= -payload.worldPos;
  res.windowPos -= payload.windowPos;
  res.color -= payload.color;
  res.normal -= payload.normal;
  res.texcood -= payload.texcood;
  return res;
}