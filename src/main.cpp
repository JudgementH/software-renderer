#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <memory>

#include "core/utils.hpp"
#include "core/model.hpp"
#include "core/renderer.hpp"
#include "gui/window.hpp"
#include "shader/vertex_shader.hpp"
#include "loader/obj_loader.hpp"

int main()
{
    int width = 1280;
    int height = 720;
    const char *title = "Software-renderer";

    std::string obj_file_path = "models/spot/spot_triangulated_good.obj";
    // std::string obj_file_path = "models/bunny/bunny.obj";

    // loader::OBJLoader l(obj_file_path);
    Model model(obj_file_path);
    auto allTri = model.allVertices;

    Rasterizer rasterizer(width, height);
    rasterizer.setRenderMode(RenderMode::VERTEX);
    std::unique_ptr<VertexShader> vs = std::make_unique<NaiveVertexShader>();
    rasterizer.setVertexShader(vs);

    if (false)
    {
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
    }

    Window window(width, height, title);
    int frames = 0;
    double print_time = window.getSystemTime();
    while (!window.is_close)
    {
        window.clear();

        auto buffer = rasterizer.render(allTri);

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

    return 0;
}