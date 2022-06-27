#pragma once

#include <Windows.h>
#include <Eigen/Eigen>

class Window
{
private:
    HDC hdc;
    HDC mdc;
    HWND wnd;
    HBITMAP bitmap;

public:
    int width;
    int height;
    const char *title;
    unsigned char *framebuffer; // BGRA
    bool is_close;
    const char *wndClassName = "Renderer";
    Window(int width, int height, const char *title);
    ~Window();
    void draw();
    void clear();
    void setFramebuffer(const unsigned char *buffer);
    void setFramebuffer(const std::vector<Eigen::Vector4f> &buffer, bool invertY = true);
    void sendMessage();
    double getSystemTime();
};