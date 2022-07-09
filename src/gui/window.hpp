#pragma once

#include <Windows.h>
#include <string>
#include <Eigen/Eigen>

class Window
{
private:
    HDC hdc;
    HDC mdc;
    HWND wnd;
    HBITMAP bitmap;
    const char *wndClassName = "Renderer";

public:
    bool showFPS = true;
    int width;
    int height;
    std::string title;
    int frames;
    unsigned char *framebuffer; // BGRA
    bool is_close;

    std::function<void(int)> keyHandler;
    std::function<void(int, int,int)> mouseHandler;

    Window(int width, int height, std::string title);
    ~Window();
    void draw();
    void clear();
    void setTitle(std::string title);
    void setFramebuffer(const unsigned char *buffer);
    void setFramebuffer(const std::vector<Eigen::Vector4f> &buffer, bool invertY = true);
    double getSystemTime();

    void sendMessage();
    bool is_pressed(char key);
    void setKeyHandler(std::function<void(int)> handler);
    void setMouseHandler(std::function<void(int,int,int)> handler);
};