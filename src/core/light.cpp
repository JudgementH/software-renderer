#include "light.hpp"

Light::Light(Eigen::Vector3f &pos, Eigen::Vector3f &dir, Eigen::Vector4f &color_)
        : position(pos), direction(dir), color(color_) {

}


Light::Light(Eigen::Vector3f &pos, Eigen::Vector3f &dir)
        : position(pos), direction(dir) {

}

void Light::setPosition(const Eigen::Vector3f &position) {
    Light::position = position;
}

void Light::setDirection(const Eigen::Vector3f &direction) {
    Light::direction = direction.normalized();
}


Eigen::Vector3f DirectionLight::getDirection(const Eigen::Vector3f &view_pos) {
    return -direction;
}

Eigen::Vector3f DirectionLight::getPosition(const Eigen::Vector3f &view_pos) {
    return view_pos + -direction * 10;
}

Eigen::Vector3f PointLight::getDirection(const Eigen::Vector3f &view_pos) {
    return (position - view_pos).normalized();
}
