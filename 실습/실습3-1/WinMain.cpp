#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <algorithm>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 3-1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);
const int window_width{800};   // 윈도우 가로
const int window_height{800};  // 윈도우 세로
const int dim{40};             // 칸의 수
const int size{18};            // 칸 사이즈
const int max_food{20};        // 최대 먹이 수
enum MovementType { DEFAULT, NONE, LEFTNRIGHT, TOPNBOTTOM, TRIANGLE }; // 이동 유형

class Food {
 private:
  int x_, y_;
  COLORREF color_;

 public:
  Food()
      : x_{rand() % dim},
        y_{rand() % dim},
        color_{RGB(rand() % 256, rand() % 256, rand() % 256)} {
  }
  ~Food() { }
  int get_x() const { return x_; }
  int get_y() const { return y_; }
  COLORREF get_color() const { return color_; }
  void set_x(const int& x) { x_ = x; }
  void set_y(const int& y) { y_ = y; }
  void setColor(const COLORREF& color) { color_ = color; }
  void Draw(HDC hDC) const;
};


void Food::Draw(HDC hDC) const {
  HBRUSH hBrush, oldBrush;

  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  Rectangle(hDC, x_ * size, y_ * size, (x_ + 1) * size, (y_ + 1) * size);
  SelectObject(hDC, oldBrush);
  DeleteObject(hBrush);
}

class Circle {
 private:
  int x_, y_;
  int x_dir_, y_dir_;
  int movement_;
  COLORREF color_;

 public:
  Circle()
      : x_{0},
        y_{0},
        x_dir_{1},
        y_dir_{1},
        movement_{rand() % 4 + 1},
        color_{RGB(255, 255, 0)} {}
  Circle(const int& movement) : movement_(movement) {
   x_ = 0;
   y_ = 0;
   x_dir_ = 1;
   y_dir_ = 1;
   color_ = RGB(255, 255, 0);
  }
  Circle(const Food& food)
      : x_{food.get_x()},
        y_{food.get_y()},
        x_dir_{1},
        y_dir_{1},
        movement_{rand() % 4 + 1},
        color_{food.get_color()} {}
  ~Circle() {}
  int get_x() const { return x_; }
  int get_y() const { return y_; }
  int get_x_dir() const { return x_dir_; }
  int get_movement() const { return movement_; }
  void set_x(const int& x) { x_ = x; }
  void set_y(const int& y) { y_ = y; }
  void set_movement(const int& movement) { movement_ = movement; }
  void set_color(const COLORREF& color) { color_ = color; }
  void Move();
  bool isCrashed(const Food& food) const;
  void Join(Circle& c) const;
  void Draw(const HDC& hDC) const;
};

void Circle::Move() { 
  switch (movement_) {
      case DEFAULT:
        x_ += x_dir_;
        if (x_ == dim) {
          --x_;
          ++y_;
          x_dir_ = -1;
        }
        if (x_ < 0) {
          ++x_;
          ++y_;
          x_dir_ = 1;
        }
        if (y_ == dim) {
          y_ = 0;
          x_ = 0;
        }
        break;
      case NONE:
        break;
      case LEFTNRIGHT:
        x_ += x_dir_;
        if (x_ >= dim) {
          --x_;
          x_dir_ = -1;
        }
        if (x_ < 0) {
          ++x_;
          x_dir_ = 1;
        }
        break;
      case TOPNBOTTOM:
        y_ += y_dir_;
        if (y_ >= dim) {
          --y_;
          y_dir_ = -1;
        }
        if (y_ < 0) {
          ++y_;
          y_dir_ = 1;
        }
        break;
      case TRIANGLE:
        x_ += x_dir_;
        y_ += y_dir_;
        if (x_ >= dim ) {
          --x_;         
          y_dir_ = 0;
          x_dir_ = -1;
        }

        if (y_ < 0) {
          ++y_;
          y_dir_ = 1;
          x_dir_ = 1;
        }

        if (x_ < 0) {
          ++x_;
          x_dir_ = 1;
          y_dir_ = -1;
        }

        if (y_ >= dim) {
          --y_;
          y_dir_ = -1;
          x_dir_ = -1;
        }

        break;
    }
  }

bool Circle::isCrashed(const Food& food) const {
  return x_ == food.get_x() && y_ == food.get_y();
}

void Circle::Join(Circle& c) const {
  if ((c.get_x() - 1 == x_ || c.get_x() + 1 == x_) && c.get_y() == y_) {
      c.set_x(x_ - x_dir_);
      c.set_y(y_);
      c.set_movement(movement_);
  }
  if((c.get_y() - 1 == y_ || c.get_y() + 1 == y_) && c.get_x() == x_) {
      c.set_x(x_ - x_dir_);
      c.set_y(y_);
      c.set_movement(movement_);
  }
}


void Circle::Draw(const HDC& hDC) const {  
  HBRUSH hBrush, oldBrush;
  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  Ellipse(hDC, x_ * size, y_ * size, (x_ + 1) * size, (y_ + 1) * size);
  SelectObject(hDC, oldBrush);
  DeleteObject(hBrush);
}

class Canvas {
 private:
  COLORREF color_;

 public:
  Canvas() : color_{RGB(80, 188, 223)} {}
  ~Canvas() {}
  void Draw(const HDC& hDC);
};

void Canvas::Draw(const HDC& hDC) {
  HPEN hPen, oldPen;
  hPen = CreatePen(PS_SOLID, 1, color_);
  oldPen = (HPEN)SelectObject(hDC, hPen);
  for (int i = 0; i <= dim; ++i) {
    MoveToEx(hDC, i * size, 0, NULL);
    LineTo(hDC, i * size, size * dim);
  }
  for (int i = 0; i <= dim; ++i) {
    MoveToEx(hDC, 0, i * size, NULL);
    LineTo(hDC, size * dim, i * size);
  }
  SelectObject(hDC, oldPen);
  DeleteObject(hPen);
}

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
        100, 50, window_width, window_height, NULL, (HMENU)NULL, hInstance, NULL);
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
    Canvas canvas;
    static Circle main_circle{DEFAULT};
    static std::vector<Circle> tail_circles;
    static std::vector<Food> food;
   // static int movement[3];

    switch (uMsg) {
        case WM_CREATE:        
        for (int i = 0; i < max_food; ++i) food.push_back({});
        SetTimer(hWnd, 1, 50, NULL);
        break;

        case WM_KEYDOWN:
        break;

        case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        canvas.Draw(hDC);
        main_circle.Draw(hDC);
        for (int i = 0; i < tail_circles.size(); ++i) tail_circles[i].Draw(hDC);        
          for (const auto& e : food) e.Draw(hDC);
          EndPaint(hWnd, &ps);
          break;

          case WM_TIMER:
            main_circle.Move();
          for (int j = 0; j < food.size(); ++j) {
          if (main_circle.isCrashed(food[j])) {
            tail_circles.push_back(food[j]);
            food.erase(food.begin() + j);
            main_circle.Join(tail_circles[j]);
          }
          }
          
          for (int i = 0; i < tail_circles.size(); ++i) {
          tail_circles[i].Move();
          for (int j = 0; j < tail_circles.size(); ++j) {
            if (i > 0) {
              tail_circles[i].Join(tail_circles[j]);
            }
          }
          }                     
            InvalidateRect(hWnd, NULL, TRUE);
            break;

          case WM_DESTROY:            
            PostQuitMessage(0);
            break;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }