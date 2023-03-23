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
      lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
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
  TCHAR str[100];
  srand((unsigned int)time(NULL));
  int x, y;

  switch (uMsg) {
    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      for (int i = 0; i < 4; ++i) {
        x = rand() % 800 / 10 * 10;
        y = rand() % 600 / 10 * 10;
        wsprintf(str, L"%dst sentence: (%d, %d)", i + 1, x, y);
        TextOut(hDC, x, y, str, lstrlen(str));
      }
      TextOut(hDC, 400, 300, L"center (400, 300)",
              lstrlen(L"center (400, 300)"));
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam,
                       lParam);  //--- 위의 세 메시지 외의 나머지 메시지는 OS로
}