#include "iostream"
#include "camera.hpp"
#include "utils.hpp"

Camera::Camera(Window *window,
               Eigen::Vector3f position,
               Eigen::Vector3f lookat,
               Eigen::Vector3f worldUp,
               float fov)
    : window(window),
      position(position),
      lookat(lookat),
      worldUp(worldUp),
      fov(fov)
{
  if (window == nullptr)
  {
    std::cout << "window为空" << std::endl;
    throw std::exception();
  }

  aspect = window->width / float(window->height);
  window->setKeyHandler(std::bind(&Camera::keyHnadle, this, std::placeholders::_1));
  window->setMouseHandler(std::bind(&Camera::mouseHandle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  z = (position - lookat).normalized();
  x = worldUp.cross(z).normalized();
  y = z.cross(x).normalized();

  // FPS camera
  Eigen::Vector3f cameraZToXOZ = Eigen::Vector3f(z.x(), 0.0f, z.z()).normalized();

  float pitch_cos = z.dot(cameraZToXOZ);
  pitch_cos = max(-1.0f, min(1.0f, pitch_cos));
  pitch = math::Radian2Degree(acos(pitch_cos)); // the included angle between cameraZ and cameraZ_xoz_projection

  Eigen::Vector3f worldZ(0.0f, 0.0f, 1.0f);
  float yaw_cos = worldZ.dot(cameraZToXOZ);
  yaw_cos = max(-1.0f, min(1.0f, yaw_cos));
  yaw = math::Radian2Degree(acos(yaw_cos)); // the included angle between worldZ and cameraZ
}

void Camera::update()
{
  // update key

  // update mouse
}

Eigen::Matrix4f Camera::getViewMatrix()
{
  Eigen::Matrix4f translation = Eigen::Matrix4f::Identity();
  translation.col(3).head<3>() = -position;

  Eigen::Matrix4f rotate = Eigen::Matrix4f::Identity();
  rotate.col(0).head<3>() = x;
  rotate.col(1).head<3>() = y;
  rotate.col(2).head<3>() = z;

  Eigen::Matrix4f view = rotate.transpose() * translation;
  return view;
}

Eigen::Matrix4f Camera::getPerspectiveMatrix()
{
  /**
   * @brief map from [n,f] to [1, -1] NDC; n and f are negative
   *  camera cone is symmetric , so thera is no translation
   *
   */
  float top = -n * std::tan(math::Degree2Radian(fov / 2.0f));
  float bottom = -top;
  float right = top * aspect;
  float left = -right;
  Eigen::Matrix4f P;
  P << (2 * n) / (right - left), 0, -(right + left) / (right - left), 0,
      0, (2 * n) / (top - bottom), -(top + bottom) / (top - bottom), 0,
      0, 0, (n + f) / (n - f), -(2 * n * f) / (n - f),
      0, 0, 1, 0;
  return P;
}

void Camera::moveForward(float distance)
{
  position += -z * distance;
}

void Camera::moveRight(float distance)
{
  position += x * distance;
}

void Camera::moveUp(float distance)
{
  position += y * distance;
}

void Camera::rotatePitch(float angle)
{
  pitch += angle;
  pitch = min(89.0f, max(-89.0f, pitch));
  updateXYZ();
}

void Camera::rotateYaw(float angle)
{
  yaw += angle;
  yaw = min(360.0f, max(0.0f, yaw));
  updateXYZ();
}

void Camera::updateXYZ()
{
  z.y() = sin(math::Degree2Radian(pitch));
  z.x() = cos(math::Degree2Radian(pitch)) * sin(math::Degree2Radian(yaw));
  z.z() = cos(math::Degree2Radian(pitch)) * cos(math::Degree2Radian(yaw));
  z = z.normalized();

  x = worldUp.cross(z).normalized();
  y = z.cross(x).normalized();
}

void Camera::keyHnadle(int param)
{
  // std::cout << "pressed: " << param << std::endl;
  if (param == int('w'))
  {
    moveForward(moveSpeed);
  }
  else if (param == int('a'))
  {
    moveRight(-moveSpeed);
  }
  else if (param == int('s'))
  {
    moveForward(-moveSpeed);
  }
  else if (param == int('d'))
  {
    moveRight(moveSpeed);
  }
}

void Camera::mouseHandle(int param, int x, int y)
{
  // printf("param: %d, x=%d, y=%d\n", param, x, y);
}
