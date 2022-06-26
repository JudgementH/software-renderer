#include <iostream>
#include "gui/window.hpp"

int main()
{
    int width = 1280;
    int height = 720;
    const char *title = "Software-renderer";

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * width * height * 4);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int index = (j * width + i) * 4;
            buffer[index] = 255;
            buffer[index + 1] = 0;
            buffer[index + 2] = 0;
            buffer[index + 3] = 0;
        }
    }

    Window window(width, height, title);

    while (!window.is_close)
    {
        window.clear();
        window.setFramebuffer(buffer);
        window.draw();
        window.sendMessage();
    }

    return 0;
}