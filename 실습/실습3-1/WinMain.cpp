#include <stdlib.h>
#include <time.h>
#include <windows.h>

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
enum Status {
  EMPTY,
  CIRCLE,
  FOOD
};  // 칸 상태 = 0: 빈 공간, 1: 원이 있는 자리 2: 먹이가 있는 자리
enum MovementType {
  DEFAULT,
  NONE,
  LEFTNRIGHT,
  TOPNBOTTOM,
  TRIANGLE
};  // 이동 유형
int map[dim][dim];

class Circle {
 private:
  int x_, y_;  // 보드 상의 (x, y) 칸
  COLORREF color_;

 public:
  static int nCircles;  // 원의 개수
  Circle() : x_{0}, y_{0}, color_{RGB(255, 255, 0)} { ++nCircles; }
  Circle(const int& x, const int& y, const COLORREF& color)
      : x_{x}, y_{y}, color_{color} {}
  ~Circle() {}
  int get_x() { return x_; }
  int get_y() { return y_; }
  void set_x(const int& x) { x_ = x; }
  void set_y(const int& y) { y_ = y; }
  void set_color(const COLORREF& color) { color_ = color; }
  void Move(const int& move_type = DEFAULT);
  bool isCrashed();
  void Eat();
  void Draw(const HDC& hDC);
};


int Circle::nCircles = 0;

void Circle::Move(const int& movement_type) {
  switch (movement_type) {
    case DEFAULT:
      if (y_ % 2 == 0) {
        ++x_;
        if (x_ == dim) {
          --x_;
          ++y_;
        }
      } else {
        --x_;
        if (x_ < 0) {
          ++x_;
          ++y_;
        }
      }
      if (y_ == dim) {
        x_ = 0;
        y_ = 0;
      }
      break;
    case NONE:
      break;
    case LEFTNRIGHT:
      break;
    case TOPNBOTTOM:
      break;
    case TRIANGLE:
      break;
  }
}

bool Circle::isCrashed() { return map[y_][x_] == FOOD; }

void Circle::Eat() { map[y_][x_] = CIRCLE; }

void Circle::Draw(const HDC& hDC) {
  HPEN hPen, oldPen;
  HBRUSH hBrush, oldBrush;

  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  Ellipse(hDC, x_ * size, y_ * size, (x_ + 1) * size, (y_ + 1) * size);
  SelectObject(hDC, oldBrush);
  DeleteObject(hBrush);
}

class Food {
 private:
  int x_, y_;  // 보드 상의 (x, y) 칸
  COLORREF color_;

 public:
  static int nFood_;
  Food()
      : x_{rand() % dim},
        y_{rand() % dim},
        color_{RGB(rand() % 256, rand() % 256, rand() % 256)} {}
  Food(const int& x, const int& y, const COLORREF color)
      : x_{x}, y_{y}, color_{color} {}
  ~Food() {}
  int get_x() { return x_; }
  int get_y() { return y_; }
  void set_x(const int& x) { x_ = x; }
  void set_y(const int& y) { y_ = y; }
  bool isCrashed();
  void Eaten();
  void setColor(const COLORREF& color) { color_ = color; }
  void Draw(HDC hDC);
};

int Food::nFood_ = 0;

bool Food::isCrashed() { return map[y_][x_] == CIRCLE; }
void Food::Eaten() { --nFood_; }

void Food::Draw(HDC hDC) {
  HPEN hPen, oldPen;
  HBRUSH hBrush, oldBrush;

  hPen = CreatePen(PS_SOLID, 5, color_);
  oldPen = (HPEN)SelectObject(hDC, hPen);
  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  Rectangle(hDC, x_ * size, y_ * size, (x_ + 1) * size, (y_ + 1) * size);
  SelectObject(hDC, oldPen);
  SelectObject(hDC, oldBrush);
  DeleteObject(hPen);
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
    MoveToEx(hDC, i * dim, 0, NULL);
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
    Circle* pCircles[max_food + 1];   
    Food* pFood[max_food];

    srand((unsigned int)time(NULL));

    switch (uMsg) {
        case WM_CREATE:
          pCircles[0] = new Circle; // 주인공 원 추가
          for (int i = 0; i < max_food; ++i) pFood[i] = new Food; // 먹이 추가
          SetTimer(hWnd, 1, 40, NULL);
          break;

        case WM_PAINT:
          hDC = BeginPaint(hWnd, &ps);          
          canvas.Draw(hDC); // 모든 것을 그린다
          EndPaint(hWnd, &ps);
          break;

        case WM_TIMER:
          pCircles[0]->Move();
          if (pCircles[0]->isCrashed()) {
        pCircles[0]->Eat();
        pCircles[1] = new Circle;
       
          };
          for (int i = 1; i < Circle::nCircles; ++i) {
        pCircles[i]->Move(rand() % 4 + 1);
        if (pCircles[i]->isCrashed()) pCircles[i]->Eat();
        pCircles[Circle::nCircles] = new Circle;
          }
          InvalidateRect(hWnd, NULL, TRUE);
          break;

        case WM_DESTROY:
          for (int i = 0; i < Food::nFood_; ++i) delete pFood[i];
          PostQuitMessage(0);
          break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}