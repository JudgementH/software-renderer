#include "iostream"
#include "camera.hpp"
#include "utils.hpp"

#undef max
#undef min

Camera::Camera(Window *window,
               const Eigen::Vector3f &position,
               const Eigen::Vector3f &lookat,
               const Eigen::Vector3f &worldUp,
               float fov)
        : window(window),
          position(position),
          lookat(lookat),
          worldUp(worldUp),
          fov(fov) {
    if (window == nullptr) {
        std::cout << "window为空" << std::endl;
        throw std::exception();
    }

    aspect = window->width / float(window->height);
    window->setKeyHandler(std::bind(&Camera::keyHnadle, this, std::placeholders::_1));
    window->setMouseHandler(
            std::bind(&Camera::mouseHandle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    z = (position - lookat).normalized();
    x = worldUp.cross(z).normalized();
    y = z.cross(x).normalized();

    // FPS camera
    Eigen::Vector3f cameraZToXOZ = Eigen::Vector3f(z.x(), 0.0f, z.z()).normalized();

    float pitch_sin = z.y();

    pitch_sin = std::max(-1.0f, std::min(1.0f, pitch_sin));
    pitch = math::Radian2Degree(asin(pitch_sin)); // the included angle between cameraZ and cameraZ_xoz_projection

    yaw = math::Radian2Degree(
            atan2(cameraZToXOZ.x(), cameraZToXOZ.z())); // the included angle between worldZ and cameraZ
}

void Camera::update() {
    // update key
//    std::cout << "pressed: " << window->keyState << std::endl;
    if (window->is_pressed('w')) {
        moveForward(keySensitivity);
    } else if (window->is_pressed('a')) {
        moveRight(-keySensitivity);
    } else if (window->is_pressed('s')) {
        moveForward(-keySensitivity);
    } else if (window->is_pressed('d')) {
        moveRight(keySensitivity);
    } else if (window->is_pressed('x')) {
        moveUp(keySensitivity);
    } else if (window->is_pressed('z')) {
        moveUp(-keySensitivity);
    }


    // update mouse
    int centerX = window->width / 2.0f;
    int centerY = window->height / 2.0f;
    int dx = window->cursor_x - centerX;
    int dy = centerY - window->cursor_y;
    rotatePitch(-dy * mouseSensitivity);
    rotateYaw(-dx * mouseSensitivity);
    window->setCursorPosition(centerX, centerY);

}

void Camera::setFov(const float &degree) {
    fov = degree;
}

Eigen::Matrix4f Camera::getViewMatrix() {
    Eigen::Matrix4f translation = Eigen::Matrix4f::Identity();
    translation.col(3).head<3>() = -position;

    Eigen::Matrix4f rotate = Eigen::Matrix4f::Identity();
    rotate.col(0).head<3>() = x;
    rotate.col(1).head<3>() = y;
    rotate.col(2).head<3>() = z;

    Eigen::Matrix4f view = rotate.transpose() * translation;
    return view;
}

Eigen::Matrix4f Camera::getPerspectiveMatrix() const {
    /**
     * @brief map from [n,f] to [-w, w] space; n and f are negative
     *  camera cone is symmetric , so thera is no translation
     *
     */
    return math::GetPerspectiveMatrix(n, f, fov, aspect);
}

void Camera::moveForward(float distance) {
    position += -z * distance;
}

void Camera::moveRight(float distance) {
    position += x * distance;
}

void Camera::moveUp(float distance) {
    position += worldUp * distance;
}

void Camera::rotatePitch(float angle) {
    pitch += angle;
    pitch = std::min(89.0f, std::max(-89.0f, pitch));
    updateXYZ();
}

void Camera::rotateYaw(float angle) {
    yaw += angle;
    if (yaw < 0.0) {
        yaw += 360;
    }
    if (yaw > 360.0) {
        yaw -= 360;
    }
    updateXYZ();
}

void Camera::updateXYZ() {
    z.y() = sin(math::Degree2Radian(pitch));
    z.x() = cos(math::Degree2Radian(pitch)) * sin(math::Degree2Radian(yaw));
    z.z() = cos(math::Degree2Radian(pitch)) * cos(math::Degree2Radian(yaw));
    z = z.normalized();

    x = worldUp.cross(z).normalized();
    y = z.cross(x).normalized();
}

void Camera::keyHnadle(int param) {
//    std::cout << "pressed: " << param << std::endl;
    if (param == int('w')) {
        moveForward(keySensitivity);
    } else if (param == int('a')) {
        moveRight(-keySensitivity);
    } else if (param == int('s')) {
        moveForward(-keySensitivity);
    } else if (param == int('d')) {
        moveRight(keySensitivity);
    } else if (param == int('x')) {
        moveUp(keySensitivity);
    } else if (param == int('z')) {
        moveUp(-keySensitivity);
    }
}

void Camera::mouseHandle(int param, int x, int y) {
    // printf("param: %d, x=%d, y=%d\n", param, x, y);
    // TODO: 按住左键移动
    int centerX = window->width / 2.0f;
    int centerY = window->height / 2.0f;
    int dx = x - centerX;
    int dy = centerY - y;
    rotatePitch(-dy * mouseSensitivity);
    rotateYaw(-dx * mouseSensitivity);
    window->setCursorPosition(centerX, centerY);
}


bool Camera::inFrustum(const Eigen::Vector4f &clipPos0, const Eigen::Vector4f &clipPos1,
                       const Eigen::Vector4f &clipPos2) const {
    /**
     * clipPos0,clipPos1,clipPos2 are the points in clip space;
     */
    if (clipPos0.w() > n && clipPos1.w() > n && clipPos2.w() > n) {
        return false;
    }
    if (clipPos0.w() < f && clipPos1.w() < f && clipPos2.w() < f) {
        return false;
    }

    if (clipPos0.x() < clipPos0.w() && clipPos1.x() < clipPos1.w() && clipPos2.x() < clipPos2.w()) {
        return false;
    }
    if (clipPos0.x() > -clipPos0.w() && clipPos1.x() > -clipPos1.w() && clipPos2.x() > -clipPos2.w()) {
        return false;
    }

    if (clipPos0.y() < clipPos0.w() && clipPos1.y() < clipPos1.w() && clipPos2.y() < clipPos2.w()) {
        return false;
    }
    if (clipPos0.y() > -clipPos0.w() && clipPos1.y() > -clipPos1.w() && clipPos2.y() > -clipPos2.w()) {
        return false;
    }

    if (clipPos0.z() < clipPos0.w() && clipPos1.z() < clipPos1.w() && clipPos2.z() < clipPos2.w()) {
        return false;
    }
    if (clipPos0.z() > -clipPos0.w() && clipPos1.z() > -clipPos1.w() && clipPos2.z() > -clipPos2.w()) {
        return false;
    }

    return true;


}
