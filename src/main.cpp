#include <iostream>
#include <eigen3/Eigen/Eigen>

#include "gui/window.hpp"
#include "core/renderer.hpp"
#include "shader/vertex_shader.hpp"

int main()
{
    int width = 1280;
    int height = 720;
    const char *title = "Software-renderer";

    std::string obj_file_path = "models/spot/spot_triangulated_good.obj";

    unsigned char *buffer_G = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 4);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int index = (j * width + i) * 4;
            buffer_G[index] = 0;
            buffer_G[index + 1] = 255;
            buffer_G[index + 2] = 0;
            buffer_G[index + 3] = 0;
        }
    }

    Rasterizer rasterizer(width, height);
    VertexShader *vs = new NaiveVertexShader();
    rasterizer.setVertexShader(vs);

    std::vector<Vertex> vertices;

    Eigen::Vector3f normal(0.0f, 0.0f, 0.1f);
    Eigen::Vector4f color_R(1.0f, 0.0f, 0.0f, 1.0f);
    Eigen::Vector4f color_G(0.0f, 1.0f, 0.0f, 1.0f);
    Eigen::Vector4f color_B(0.0f, 0.0f, 1.0f, 1.0f);

    vertices.emplace_back(Eigen::Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
                          color_R,
                          normal);
    vertices.emplace_back(Eigen::Vector4f(-0.5f, -0.5f, 0.0f, 1.0f),
                          color_G,
                          normal);
    vertices.emplace_back(Eigen::Vector4f(0.5f, -0.5f, 0.0f, 1.0f),
                          color_B,
                          normal);

    Window window(width, height, title);
    int frames = 0;
    double print_time = window.getSystemTime();
    while (!window.is_close)
    {
        window.clear();

        auto buffer = rasterizer.render(vertices);

        window.setFramebuffer(buffer);
        window.draw();
        window.sendMessage();

        // print FPS
        double current_time = window.getSystemTime();
        if (current_time - print_time > 1.0)
        {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            print_time = current_time;
        }
        frames++;
    }

    delete vs;
    delete[] buffer_G;
    return 0;
}