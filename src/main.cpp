#include <iostream>
#include <eigen3/Eigen/Eigen>
#include <memory>

#include "core/model.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"
#include "gui/window.hpp"
#include "shader/vertex_shader.hpp"
#include "shader/normal_fragment_shader.hpp"
#include "shader/blinn_phong_fragment_shader.hpp"

int main() {

    int width = 1280;
    int height = 720;
    std::string title = "Software-renderer";
    Window window(width, height, title);

    /**
     * create camera
     */
    Eigen::Vector3f pos(4.0f, 1.5f, 4.0f);
//    Eigen::Vector3f pos(0.0f, 0.0f, 0.4f);
    Eigen::Vector3f lookat(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f up(0.0f, 1.0f, 0.0f);
    float fov = 60.0f;
    Camera camera(&window, pos, lookat, up, fov);


    /**
     * create model
     */
    std::string obj_file_path = "models/spot/spot_triangulated_good.obj";
//    std::string obj_file_path = "models/spot/spot_control_mesh.obj";
//    std::string obj_file_path = "models/bunny/bunny.obj";
//    std::string obj_file_path = "models/Crate/Crate1.obj";
//    std::string obj_file_path = "models/box/box.obj";

    std::string floor_file_path = "models/floor/floor.obj";

    Model model(obj_file_path);
    Texture texture("models/spot/spot_texture.png");
//    Texture texture("models\\Crate\\test.jpg");
//    Texture texture("models/Crate/crate_1.jpg");
    model.setTexture(texture);
    model.setPosition({1.0, 1.0, 1.0});

    Model floor(floor_file_path);
    floor.setTexture(Texture{"models/floor/checker.png"});
    floor.setScale(0.1);

    Model test_floor(floor_file_path);
    test_floor.setTexture(Texture{"models/floor/checker.png"});
    test_floor.setPosition({0.0,0.2,0.0});
    test_floor.setScale(0.1);

    /**
     * create light
     */
    DirectionLight dir_light;
    dir_light.setDirection({-1.0f, -1.0f, 1.0f});

    /**
     * create scene
     */
    Scene scene;
    scene.add(&model);
    scene.add(&floor);
    scene.add(&dir_light);

    /**
     * create rasterizer
     */
    Rasterizer rasterizer(width, height, &camera);
    rasterizer.setRenderMode(RenderMode::FACE);

    std::unique_ptr<VertexShader> vs = std::make_unique<NaiveVertexShader>();
    rasterizer.setVertexShader(vs);

    std::unique_ptr<FragmentShader> bpfs = std::make_unique<BlinnPhongFragmentShader>();
    rasterizer.setFragmentShader(bpfs);

    while (!window.is_close) {
        window.clear();
        rasterizer.clearDepthBuffer();
        rasterizer.clearFrameBuffer();

        camera.update();

        auto view = camera.getViewMatrix();
        rasterizer.setViewMatrix(view);

        auto project = camera.getPerspectiveMatrix();
        rasterizer.setProjectMatrix(project);

//        auto buffer = rasterizer.render(model);
        auto buffer = rasterizer.render(scene);

        window.setFramebuffer(buffer);
        window.draw();
        window.sendMessage();
    }

    return 0;
}
