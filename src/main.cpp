#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <memory>

#include "core/model.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "gui/window.hpp"
#include "shader/vertex_shader.hpp"
#include "shader/normal_fragment_shader.hpp"

int main() {


    int width = 1280;
    int height = 720;
    std::string title = "Software-renderer";
    Window window(width, height, title);

    Eigen::Vector3f pos(0.4f, 0.5f, 2.0f);
    // Eigen::Vector3f lookat(0.8f, 0.5f, 4.0f);
    Eigen::Vector3f lookat(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
    float fov = 60.0f;
    Camera camera(&window, pos, lookat, up, fov);

    std::string obj_file_path = "models/spot/spot_triangulated_good.obj";
//     std::string obj_file_path = "models/spot/spot_control_mesh.obj";
//    std::string obj_file_path = "models/bunny/bunny.obj";

    Model model(obj_file_path);

    Rasterizer rasterizer(width, height, &camera);
    rasterizer.setRenderMode(RenderMode::FACE);

    std::unique_ptr<VertexShader> vs = std::make_unique<NaiveVertexShader>();
    rasterizer.setVertexShader(vs);


    std::unique_ptr<FragmentShader> fs = std::make_unique<NormalFragmentShader>();
    rasterizer.setFragmentShader(fs);

    while (!window.is_close) {

//        camera.update();
        window.clear();
        rasterizer.clearDepthBuffer();
        rasterizer.clearFrameBuffer();

        auto view = camera.getViewMatrix();
        rasterizer.setViewMatrix(view);

        auto project = camera.getPerspectiveMatrix();
        rasterizer.setProjectMatrix(project);

        auto buffer = rasterizer.render(model);

        window.setFramebuffer(buffer);
        window.draw();
        window.sendMessage();
    }

    return 0;
}
