#include <stdlib.h>
#include <time.h>
#include <windows.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 3-1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);
const int window_width{800};
const int window_height{800};

class Board {
 private:
  int row_num_, square_side_; // 가로 세로 개수가 같음
  COLORREF color_;

 public:
  Board() : row_num_{40}, square_side_{18}, color_{RGB(80, 188, 223)} {}
  Board(const int& row_num, const int& square_side, const COLORREF& color)
      : row_num_{row_num}, square_side_{square_side}, color_{color} {}
  ~Board() {}
  int get_row_num() { return row_num_; }
  int get_square_side() { return square_side_; }
  void Draw(const HDC& hDC);
};

void Board::Draw(const HDC& hDC) {
  HPEN hPen, oldPen;
  hPen = CreatePen(PS_SOLID, 1, color_);
  oldPen = (HPEN)SelectObject(hDC, hPen);
  for (int i = 0; i <= row_num_; ++i) {
    MoveToEx(hDC, i * square_side_, 0, NULL);
    LineTo(hDC, i * square_side_, square_side_ * row_num_);
  }
  for (int i = 0; i <= row_num_; ++i) {
    MoveToEx(hDC, 0, i * square_side_, NULL);
    LineTo(hDC, square_side_ * row_num_, i * square_side_);
  }
  SelectObject(hDC, oldPen);
  DeleteObject(hPen);
}

class Circle {
 private:
  int x_, y_;  // 보드 상의 (x, y) 칸
  COLORREF color_;

 public:
  Circle() : x_{0}, y_{0}, color_{RGB(255, 255, 0)} {}
  Circle(const int& x, const int& y, const COLORREF& color)
      : x_{x}, y_{y}, color_{color} {}
  ~Circle() {}
  int get_x() { return x_; }
  int get_y() { return y_; }
  void MoveTo(const int& x, const int& y);
  void Draw(HDC hDC, const int& square_side);
};

void Circle::MoveTo(const int& x, const int& y) {
  x_ = x;
  y_ = y;
}

void Circle::Draw(HDC hDC, const int& square_side) {
  HPEN hPen, oldPen;
  HBRUSH hBrush, oldBrush;
  
  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
  Ellipse(hDC, x_ * square_side, y_ * square_side, (x_ + 1) * square_side,
          (y_ + 1) * square_side);  
  SelectObject(hDC, oldBrush);  
  DeleteObject(hBrush);
}

class Food {
private:
  int x_, y_;  // 보드 상의 (x, y) 칸
 COLORREF color_;

public:
 Food() : x_{0}, y_{0}, color_{RGB(rand() % 256, rand() % 256, rand() % 256)} {}
 Food(const int& x, const int& y, const COLORREF color)
      : x_{x}, y_{y}, color_{color} {}
 ~Food() {}
 int get_x() { return x_; }
 const int get_y() { return y_; }
 const
 void setColor(const COLORREF& color) { color_ = color; }
    void MoveTo(const int& x, const int& y);
    void Draw(HDC hDC, const int& square_side);
    bool isEaten(Circle circle);
};

void Food::MoveTo(const int& x, const int& y) {
    x_ = x;
    y_ = y;
}

void Food::Draw(HDC hDC, const int& square_side) {
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;

    hPen = CreatePen(PS_SOLID, 5, color_);
    oldPen = (HPEN)SelectObject(hDC, hPen);
    hBrush = CreateSolidBrush(color_);
    oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Rectangle(hDC, x_ * square_side, y_ * square_side, (x_ + 1) * square_side,
              (y_ + 1) * square_side);
    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}

bool Food::isEaten(Circle circle) {    
if (get_x() == circle.get_x() && get_y() == circle.get_y()) return true;
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
    Board board;
    static Circle circle;
    static int x, y;
    static bool isCircled[40][40];
    const int food_num{20};
    static Food food[food_num];

    srand((unsigned int)time(NULL));

    switch (uMsg) {
        case WM_CREATE:
          x = 0;
          y = 0;
          for (int j = 0; j < board.get_row_num(); ++j)
            for (int i = 0; i < board.get_row_num(); ++i) isCircled[j][i] = false;
          for (int i = 0; i < food_num; ++i) {
            food[i].MoveTo(rand() % board.get_row_num(),
                           rand() % board.get_row_num());           
          }
          SetTimer(hWnd, 1, 40, NULL);
          break;

        case WM_PAINT:
          hDC = BeginPaint(hWnd, &ps);
          board.Draw(hDC);
          for (int i = 0; i < food_num; ++i) {
            if (!isCircled[food[i].get_y()][food[i].get_x()])
            food[i].Draw(hDC, board.get_square_side());
          }                   
          circle.Draw(hDC, board.get_square_side());          
          EndPaint(hWnd, &ps);
          break;

        case WM_TIMER:
          if (y % 2 == 0) {
            ++x;
            if (x == board.get_row_num()) {
              --x;
              ++y;
            }
          } else {
            --x;
            if (x < 0) {
              ++x;
              ++y;
            }
          }
          if (y == board.get_row_num()) {
            x = 0;
            y = 0;
          }          
          circle.MoveTo(x, y);
          isCircled[y][x] = true;
          InvalidateRect(hWnd, NULL, TRUE);
          break;

        case WM_DESTROY:               
          PostQuitMessage(0);
          break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}