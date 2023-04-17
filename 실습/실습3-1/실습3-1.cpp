#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>  //--- 윈도우 헤더 파일
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 3-1";
const int window_width{ 1200 };
const int window_height{ 900 };
const int size{ 40 };
const int square_size{ 20 };

class Circle {
private:
    int x_, y_;

public:
    Circle() {}
    Circle(const int& x, const int& y) : x_(x), y_(y) {}
    ~Circle() {}
    int get_x() { return x_; }
    int get_y() { return y_; }
    void set_x(const int& x) { x_ = x; }
    void set_y(const int& y) { y_ = y; }
    void move(const int& x, const int& y) { set_x(x); set_y(y); }
    void Draw(HDC hDC, const int& x, const int& y, COLORREF color = RGB(255, 255, 0)) {
        Ellipse(hDC, x * square_size, y * square_size, (x + 1) * square_size, (y + 1) * square_size);
    }
};

class Food {
private:
    int x_, y_;

public:
    Food(const int& x, const int& y) : x_(x), y_(y) {}
    ~Food() {}
    int get_x() { return x_; }
    int get_y() { return y_; }
    void set_x(const int& x) { x_ = x; }
    void set_y(const int& y) { y_ = y; }
    void move(const int& x, const int& y) { set_x(x); set_y(y); }
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void CreateBoard(HDC hDC, const int& size, const int& square_size);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdParam, int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&WndClass);
    hWnd = CreateWindow(
        lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
        100, 50, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    Circle circle;
    int x, y;

    switch (uMsg) {
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 225, 225));
        oldPen = (HPEN)SelectObject(hDC, hPen);
        CreateBoard(hDC, size, square_size);
        circle.Draw(hDC, 0, 0);
        EndPaint(hWnd, &ps);
        break;
  
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CreateBoard(HDC hDC, const int& size, const int& square_size)
{
    for (int i = 0; i <= size; ++i)
    {
        MoveToEx(hDC, i * square_size, 0, NULL);
        LineTo(hDC, i * square_size, square_size * size);
    }
    for (int i = 0; i <= size; ++i)
    {
        MoveToEx(hDC, 0, i * square_size, NULL);
        LineTo(hDC, square_size * size, i * square_size);
    }
}