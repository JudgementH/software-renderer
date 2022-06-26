#include <iostream>
#include <eigen3/Eigen/Eigen>

#include "gui/window.hpp"

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

    Window window(width, height, title);
    int frames = 0;
    double print_time = window.getSystemTime();
    while (!window.is_close)
    {
        window.clear();
        window.setFramebuffer(buffer_G);
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