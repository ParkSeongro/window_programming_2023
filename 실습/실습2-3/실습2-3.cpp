#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>  //--- 윈도우 헤더 파일
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습2-3";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);
COLORREF SetRandRGB();
void SetRandColorText(const HDC&);
void SetRandColorBk(const HDC&);
void SetRandPos(RECT& rect, const int&, const int&);
int RandNum(const int&, const int&);
int RandCount(const int&, const int&);
LPCWSTR ConverttoStr(const int&, LPCWSTR&);

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
  hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 50,
                      WINDOW_WIDTH, WINDOW_HEIGHT, NULL, (HMENU)NULL, hInstance,
                      NULL);
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
  RECT rect;
  LPCWSTR str;
  int num;
  int count;
  srand((unsigned int)time(NULL));

  switch (uMsg) {
    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      num = RandNum(0, 9);
      count = RandCount(10, 50);
      SetRandColorText(hDC);
      SetRandColorBk(hDC);
      SetRandPos(rect, WINDOW_WIDTH, WINDOW_HEIGHT);
      str = ConverttoStr(num, str);
      for (int i = 0; i < count; ++i) {
        rect.left += 8;
        rect.right = rect.left + 8;
        DrawText(hDC, str, lstrlen(str), &rect, DT_WORDBREAK | DT_EDITCONTROL);
      }
      
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

COLORREF SetRandRGB() {
  COLORREF r{(COLORREF)rand() % 256};
  COLORREF g{(COLORREF)rand() % 256};
  COLORREF b{(COLORREF)rand() % 256};
  return RGB(r, g, b);
}

void SetRandColorText(const HDC& hdc) {
  COLORREF text_color = SetRandRGB();
  SetTextColor(hdc, text_color);
}

void SetRandColorBk(const HDC& hdc) {
  COLORREF bk_color = SetRandRGB();
  SetBkColor(hdc, bk_color);
}

void SetRandPos(RECT& rect, const int& width, const int& height) {
  rect.left = rand() % (width-100 + 1);
  rect.top = rand() % (height-100 + 1);
  rect.bottom = rect.top + 15;
}

int RandNum(const int& min, const int& max) {
  int num = rand() % (max - min + 1) + min;
  return num;
}

int RandCount(const int& min, const int& max) {
  int count = rand() % (max - min + 1) + min;
  return count;
}

LPCWSTR ConverttoStr(const int& num, LPCWSTR& str) {  // 변환 방법을 몰라서 꼼수
  switch (num) {
  case 0:
      str = L"0";
      return str;
      break;
    case 1:
      str = L"1";
      return str;
      break;
    case 2:
      str = L"2";
      return str;
      break;
    case 3:
      str = L"3";
      return str;
      break;
    case 4:
      str = L"4";
      return str;
      break;
    case 5:
      str = L"5";
      return str;
      break;
    case 6:
      str = L"6";
      return str;
      break;
    case 7:
      str = L"7";
      return str;
      break;
    case 8:
      str = L"8";
      return str;
      break;
    case 9:
      str = L"9";
      return str;
      break;
  }
}