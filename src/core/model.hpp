#pragma once

#include <Eigen/Eigen>
#include <vector>
#include <string>
#include <optional>
#include "triangle.hpp"
#include "texture.hpp"

struct Index {
    int position_index;
    int texcoord_index;
    int normal_index;
};

class Model {
private:
    Eigen::Vector3f position = Eigen::Vector3f{0, 0, 0};
    float scale = 1.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;
public:
    int shape_num;
    int face_num;
    std::vector<Eigen::Vector4f> position_list;
    std::vector<Eigen::Vector3f> normal_list;
    std::vector<Eigen::Vector2f> texcoord_list;
    std::vector<Index> indices;
    std::optional<Texture> texture = std::nullopt;

    Model(const std::string &filename);

    void setTexture(Texture tex);

    Triangle getFace(int i);

    void setPosition(const Eigen::Vector3f &position);

    void setScale(float scale);

    void setPitch(float pitch);

    void setYaw(float yaw);

    void setRoll(float roll);

    Eigen::Matrix4f getModelMatrix() const;
};