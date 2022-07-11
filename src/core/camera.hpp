#include <Eigen/Eigen>
#include "../gui/window.hpp"

class Camera
{
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
    float f = 50;   // far

    float pitch;
    float yaw;

    float keySensitivity = 0.1f;
    float mouseSensitivity = 0.05f;

    Camera(Window *window,
           Eigen::Vector3f position,
           Eigen::Vector3f lookat,
           Eigen::Vector3f up,
           float fov);

    Eigen::Matrix4f getViewMatrix();
    Eigen::Matrix4f getPerspectiveMatrix();

    void setFov(float fov);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    void rotatePitch(float distance);
    void rotateYaw(float angle);

    void update();
};