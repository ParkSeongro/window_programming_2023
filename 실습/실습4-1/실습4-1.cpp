#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include "Game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 4-1";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);
bool BalltoBlock(const Ball& ball, const Block& block);
bool BalltoBar(const Ball& ball, const Bar& bar);
void DeleteBlock(Block* block);
bool InBar(const Bar& bar, int mx, int my);

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
  static RECT winRect;
  PAINTSTRUCT ps;
  HDC hDC;
  static Game game;

  switch (uMsg) {
    case WM_CREATE:
      GetClientRect(hWnd, &winRect);
      game.Init(hWnd, winRect.right, winRect.bottom, 3);
      break;

    case WM_KEYDOWN:
      switch (wParam) {
        case VK_ADD:
          game.SpeedUp();
          break;
        case VK_SUBTRACT:
          game.SpeedDown();
          break;
      }
      break;
    case WM_CHAR:
      switch (wParam) {
        case 's':          
          game.Play(hWnd);
          break;
        case 'p':
          game.Pause(hWnd);         
          break;
        case 'n':        
          game.Init(hWnd, winRect.right, winRect.bottom, 3);         
          break;
        case 'q':
          game.End(hWnd);
          break;
      }
      break;
    case WM_LBUTTONUP:
      game.NonClick(hWnd, lParam);    
      break;
    case WM_LBUTTONDOWN:
      game.Click(hWnd, lParam);     
      break;
    case WM_MOUSEMOVE:
      game.MoveBar(hWnd, lParam);    
      break;
    case WM_TIMER:  
      switch (wParam) {
        case 1:
          game.Play(hWnd);
          break;
      }     
      break;

    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      game.Draw(hDC);
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      game.End(hWnd);      
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}