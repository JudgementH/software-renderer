#include <iostream>
#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

Texture::Texture(const std::string &path) {
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (channels != 3) {
        std::cout << "channels" << channels << std::endl;
        throw std::exception();
    }
}

Texture::Texture(const Texture &t) {
    width = t.width;
    height = t.height;
    channels = t.channels;
    if (t.data) {
        data = (unsigned char *) malloc(width * height * channels);
        memcpy(data, t.data, width * height * channels);

    }
}

Texture::~Texture() {
    if (data != nullptr) {
        stbi_image_free(data);
    }
}

Eigen::Vector4f Texture::getColor(float u, float v) {
    int x_img = u * width;
    int y_img = (1 - v) * height;
    if (x_img < 0 || x_img > width || y_img < 0 || y_img > height) {
        return Eigen::Vector4f::Zero();
    }
    int index = channels * (x_img + y_img * width);
    return Eigen::Vector4f(static_cast<float>(data[index]) / 255.0f,
                           static_cast<float>(data[index + 1]) / 255.0f,
                           static_cast<float>(data[index + 2]) / 255.0f,
                           1.0f);
}

Texture &Texture::operator=(const Texture &t) {
    width = t.width;
    height = t.height;
    channels = t.channels;
    if (t.data) {
        data = (unsigned char *) malloc(width * height * channels);
        memcpy(data, t.data, width * height * channels);
    }
    return *this;
}
