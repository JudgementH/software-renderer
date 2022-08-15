#pragma once

#include <vector>
#include <Eigen/Eigen>
#include <memory>

class Texture {
public:
    int width;
    int height;
    int channels;
    unsigned char *data = nullptr;

    Texture() = default;

    Texture(const std::string &path);

    Texture(const Texture &t);

    Texture(const std::vector<Eigen::Vector4f> color);

    ~Texture();

    Eigen::Vector4f getColor(float u, float v);

    Texture &operator=(const Texture &t);

};
