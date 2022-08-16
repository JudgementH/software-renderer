#include <iostream>
#include <thread>
#include <mutex>
#include "window.hpp"

LRESULT WINAPI msg_callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window *window = static_cast<Window *>(GetProp(hWnd, "currentWindow"));

    if (!window)
        return DefWindowProc(hWnd, msg, wParam, lParam);
    switch (msg) {
        case WM_MOUSEMOVE: {
//            window->mouseHandler(int(wParam), int(LOWORD(lParam)), int(HIWORD(lParam)));
            window->cursor_x = int(LOWORD(lParam));
            window->cursor_y = int(HIWORD(lParam));
            break;
        }
        case WM_CHAR: {
            if (wParam == 3) { //key ctrl+c
                window->is_close = true;
                break;
            }
            break;
        }
        case WM_KEYDOWN: {
            window->keyState = int(wParam);
            break;
        }
        case WM_KEYUP: {
            window->keyState = -1;
            break;
        }
        case WM_CLOSE: {
            window->is_close = true;
            break;
        }
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

Window::Window(int width, int height, std::string title) : width(width), height(height), title(title), is_close(false) {
    WNDCLASSEX wndClass;
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = (WNDPROC) msg_callback;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = GetModuleHandle(NULL);
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = wndClassName;
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndClass);
    wnd = CreateWindow(wndClassName, title.c_str(), WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, wndClass.hInstance, NULL);

    SetProp(wnd, "currentWindow", this);

    hdc = GetDC(wnd);
    mdc = CreateCompatibleDC(hdc);

    BITMAPINFO bitMapInfo;
    bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitMapInfo.bmiHeader.biWidth = width;
    bitMapInfo.bmiHeader.biHeight = -height;
    bitMapInfo.bmiHeader.biPlanes = 1;
    bitMapInfo.bmiHeader.biBitCount = 32;
    bitMapInfo.bmiHeader.biCompression = BI_RGB;
    bitMapInfo.bmiHeader.biSizeImage = width * height * 4;

    bitmap = CreateDIBSection(mdc, &bitMapInfo, DIB_RGB_COLORS, (void **) &framebuffer, 0, 0);
    SelectObject(mdc, bitmap);

    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, GetWindowLong(wnd, GWL_STYLE), 0);
    int wx, wy, sx, sy;
    wx = rect.right - rect.left;
    wy = rect.bottom - rect.top;
    sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
    sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
    if (sy < 0)
        sy = 0;

    SetWindowPos(wnd, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
    SetForegroundWindow(wnd);

    ShowWindow(wnd, SW_NORMAL);

    if (showFPS) {
        std::thread([this, title]() -> void {
            frames = 0;
            while (true) {
                Sleep(1000);
                std::string fps = " FPS: " + std::to_string(frames);
                frames = 0;
                setTitle(title + fps);
            }
        })
                .detach();
    }
    // hide the cursor
    ShowCursor(false);
}

Window::~Window() {
    if (mdc) {
        DeleteDC(mdc);
        mdc = nullptr;
    }

    if (bitmap) {
        DeleteObject(bitmap);
        bitmap = nullptr;
    }
    if (wnd) {
        CloseWindow(wnd);
        wnd = nullptr;
    }
}

void Window::draw() {
    frames++;
    BitBlt(hdc, 0, 0, width, height, mdc, 0, 0, SRCCOPY);
}

void Window::clear() {
    memset(framebuffer, 0, sizeof(unsigned char) * width * height * 4);
}

void Window::setFramebuffer(const unsigned char *buffer) {
    /**
     * buffer: (R[0~255], G[0~255], B[0~255], A[0~255])
     *
     */
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (j * width + i) * 4;
            framebuffer[index] = buffer[index + 2];
            framebuffer[index + 1] = buffer[index + 1];
            framebuffer[index + 2] = buffer[index];
            framebuffer[index + 3] = buffer[index + 3];
        }
    }
}

void Window::setFramebuffer(const std::vector<Eigen::Vector4f> &buffer, bool invertY) {
    if (buffer.size() != width * height) {
        std::cout << "缓冲大小和屏幕大小不匹配" << std::endl;
        throw std::exception();
    }
    for (int i = 0; i < buffer.size(); i++) {
        int x = i % width;
        int y = i / width;
        if (invertY) {
            y = height - 1 - y;
        }
        int index = (x + width * y) * 4;
        framebuffer[index] = static_cast<int>(buffer[i].z() * 255);     // B
        framebuffer[index + 1] = static_cast<int>(buffer[i].y() * 255); // G
        framebuffer[index + 2] = static_cast<int>(buffer[i].x() * 255); // R
        framebuffer[index + 3] = static_cast<int>(buffer[i].w() * 255); // A
    }
}

void Window::sendMessage() {
    MSG msg;
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

double Window::getSystemTime() {
    static double period = -1;
    LARGE_INTEGER counter;
    if (period < 0) {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        period = 1 / (double) frequency.QuadPart;
    }
    QueryPerformanceCounter(&counter);
    return counter.QuadPart * period;
}

void Window::setTitle(std::string title) {
    this->title = title;
    SetWindowText(wnd, title.c_str());
}

void Window::setKeyHandler(std::function<void(int)> handler) {
    keyHandler = handler;
}

void Window::setMouseHandler(std::function<void(int, int, int)> handler) {
    mouseHandler = handler;
}

void Window::setCursorPosition(int x, int y) {
    POINT p = {x, y};
    ClientToScreen(wnd, &p);
    SetCursorPos(p.x, p.y);
}

bool Window::is_pressed(char key) const {
    int code = static_cast<int>(key);
    if (97 <= code && code <= 122) {
        code -= 32;
    }
    return code == keyState;
}

bool Window::is_pressed(int key_code) const {
    return key_code == keyState;
}