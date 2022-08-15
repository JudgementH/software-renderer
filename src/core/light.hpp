#pragma once

#include <Eigen/Eigen>

class Light {
protected:
    Eigen::Vector3f position;
    Eigen::Vector3f direction;
public:
    Eigen::Vector4f color = Eigen::Vector4f::Ones();
    float intensity = 1.0f;

    Light() = default;

    ~Light() = default;

    Light(Eigen::Vector3f &pos, Eigen::Vector3f &dir);

    Light(Eigen::Vector3f &pos, Eigen::Vector3f &dir, Eigen::Vector4f &color_);

    /**
     * get light direction from view_pos to light_pos
     * @param view_pos
     * @return
     */
    virtual Eigen::Vector3f getDirection(const Eigen::Vector3f &view_pos) = 0;

    /**
     * get light position form view_pos
     * @param view_pos
     * @return
     */
    virtual Eigen::Vector3f getPosition(const Eigen::Vector3f &view_pos) = 0;

    void setPosition(const Eigen::Vector3f &position);

    void setDirection(const Eigen::Vector3f &direction);
};

class DirectionLight : public Light {
    Eigen::Vector3f getDirection(const Eigen::Vector3f &view_pos) override;

    Eigen::Vector3f getPosition(const Eigen::Vector3f &view_pos) override;
};

class PointLight : public Light {
    Eigen::Vector3f getDirection(const Eigen::Vector3f &view_pos) override;
};