#pragma once

#include <Windows.h>

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
    unsigned char *framebuffer;
    bool is_close;
    const char *wndClassName = "Renderer";
    Window(int width, int height, const char *title);
    ~Window();
    void draw();
    void clear();
    void setFramebuffer(const unsigned char *buffer);
    void sendMessage();
};