#pragma once

#include <Eigen/Eigen>
#include "../gui/window.hpp"

class Camera {
private:
    void updateXYZ();

    void keyHnadle(int param);

    void mouseHandle(int param, int x, int y);

public:
    Window *window;

    Eigen::Vector3f x;
    Eigen::Vector3f y;
    Eigen::Vector3f z;

    Eigen::Vector3f position;
    Eigen::Vector3f lookat;
    Eigen::Vector3f worldUp;
    float fov; // field of vertical view
    float aspect;
    float n = -0.1; // near
    float f = -50.0;   // far

    float pitch;
    float yaw;

    float keySensitivity = 0.1f;
    float mouseSensitivity = 0.05f;

    Camera(Window *window,
           const Eigen::Vector3f &position,
           const Eigen::Vector3f &lookat,
           const Eigen::Vector3f &up,
           float fov);

    Eigen::Matrix4f getViewMatrix();

    Eigen::Matrix4f getPerspectiveMatrix() const;

    void setFov(const float &degree);

    void moveForward(float distance);

    void moveRight(float distance);

    void moveUp(float distance);

    void rotatePitch(float angle);

    void rotateYaw(float angle);

    void update();

    bool inFrustum(const Eigen::Vector4f &clipPos0, const Eigen::Vector4f &clipPos1, const Eigen::Vector4f &clipPos2) const;
};