#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <windows.h>  //--- 윈도우 헤더 파일
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"windows programming 1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);
COLORREF SetRandRGB();

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
  WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
  TCHAR str[100] = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  RECT rect;
  COLORREF text_color, bk_color;
  const int rect_width = WINDOW_WIDTH / 3;
  const int rect_height = WINDOW_HEIGHT / 2;
  srand((unsigned int)time(NULL));

  switch (uMsg) {
    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      for (int col = 0; col < 3; ++col)
        for (int row = 0; row < 2; ++row) {
          rect.left = rect_width * col;
          rect.top = rect_height * row;
          rect.right = rect.left + rect_width;
          rect.bottom = rect.top + rect_height;
          text_color = SetRandRGB();
          bk_color = SetRandRGB();
          SetTextColor(hDC, text_color);
          SetBkColor(hDC, bk_color);
          DrawText(hDC, str, lstrlen(str), &rect,
                   DT_EDITCONTROL | DT_WORDBREAK);
        }
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam,
                       lParam);  //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}

COLORREF SetRandRGB() {
  COLORREF r{(COLORREF)rand() % 256};
  COLORREF g{(COLORREF)rand() % 256};
  COLORREF b{(COLORREF)rand() % 256};
  return RGB(r, g, b);
}