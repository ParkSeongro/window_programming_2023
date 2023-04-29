#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include "Block.h"
#include "Bar.h"
#include "Ball.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"실습 3-2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
                         LPARAM lParam);
bool BalltoBlock(const Ball& ball, const Block& block);
bool BalltoBar(const Ball& ball, const Bar& bar);
void DeleteBlock(Block* block);


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
 static RECT winRect;
  PAINTSTRUCT ps;
  HDC hDC; // 3x10
  static Bar bar{300, 500, 0, 80, 20, RGB(128, 128, 128)};
  static Ball ball{310, 400, 20, 1, -1, RGB(225, 225, 0)};
  static Block* pBlocks[50][50];
  const int row{3};
  const int col{10};
  static int ball_elapse;
  static bool isPaused;
  static bool isStarted;
  static bool isCrashed;
  //static int i_index, j_index;

  switch (uMsg) {
    case WM_CREATE:
      ball_elapse = 1;
      isStarted = false;
      isPaused = false;
      isCrashed = false;
      GetClientRect(hWnd, &winRect);      
      for (int j = 0; j < row; ++j)
        for (int i = 0; i < col; ++i)
          pBlocks[j][i] = new Block{10 + i * 60, 30 + j * 30,       1, 60,
                                     30,          RGB(255, 255, 255), false};
      break;

    case WM_KEYDOWN:
      switch (wParam) { 
        case VK_ADD:
          ball_elapse -= 10;
          if (ball_elapse < 2) ball_elapse = 2;
          SetTimer(hWnd, 0, ball_elapse, NULL);
          break;
      case VK_SUBTRACT:  
        ball_elapse += 10;
        if (ball_elapse > 500) ball_elapse = 500;
        SetTimer(hWnd, 0, ball_elapse, NULL);
          break;      
      }
      
      break;
    case WM_CHAR:
      switch (wParam) {
        case 's':
          SetTimer(hWnd, 0, ball_elapse, NULL); // 0: 공
          SetTimer(hWnd, 1, 2, NULL); // 1: 나머지
          isStarted = true;
          break;
        case 'p':
          if (isStarted) {
            if (isPaused) {
              SetTimer(hWnd, 0, ball_elapse, NULL);
              SetTimer(hWnd, 1, 2, NULL);
              isPaused = false;
            } else {
              KillTimer(hWnd, 0);
              KillTimer(hWnd, 1);
              isPaused = true;
            }
          }
          break;
        case 'n':         
          // 초기화
          break;
        case 'q':
          for (int i = 0; i < row; ++i)
            for (int j = 0; i < col; ++j) {
              if (pBlocks[i][j] != nullptr) delete pBlocks[i][j];
            }
          PostQuitMessage(0);
          break;
      }
      break;
    case WM_LBUTTONUP:
      break;
    case WM_LBUTTONDOWN:
      break;

    case WM_TIMER:
      switch (wParam) {
        case 0:
          ball.Move(winRect.right, winRect.bottom + ball.getDiameter());
          break;
        case 1:
          bar.Move(winRect.right);
          for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j) {
              if (pBlocks[i][j] != nullptr) {
                pBlocks[i][j]->Move();
                if (BalltoBlock(ball, *pBlocks[i][j])) { // 공과 블록의 충돌체크
                 /* i_index = i;
                  j_index = j;
                  SetTimer(hWnd, 2, 1, NULL);    */ 
                   if (!pBlocks[i][j]->getIsCrashed()) {
                   pBlocks[i][j]->setColor(
                       RGB(rand() % 256, rand() % 256, rand() % 256));
                     pBlocks[i][j]->setHeight(pBlocks[i][j]->getHeight() - 10);
                   pBlocks[i][j]->setWidth(pBlocks[i][j]->getWidth() - 10);
                     pBlocks[i][j]->setIsCrashed(true);
                 }
                   else {
                     DeleteBlock(pBlocks[i][j]);
                     pBlocks[i][j] = nullptr;                 
                   }
                  ball.setydir(-ball.getydir());
                }
              }
            }
          if (BalltoBar(ball, bar)) ball.setydir(-ball.getydir());         
          break;   
        case 2:
         /* if (pBlocks[i_index][j_index]->getWidth() > 0)
            pBlocks[i_index][j_index]->setWidth(
                pBlocks[i_index][j_index]->getWidth() - 5);
          if (pBlocks[i_index][j_index]->getHeight() > 0)
            pBlocks[i_index][j_index]->setHeight(
                pBlocks[i_index][j_index]->getHeight() - 5);   */     
          break;

      }
      InvalidateRgn(hWnd, NULL, TRUE);
      break;

    case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      ball.Draw(hDC);
      bar.Draw(hDC);
      for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) {
          if (pBlocks[i][j] != nullptr)
          pBlocks[i][j]->Draw(hDC);
        }
      EndPaint(hWnd, &ps);
      break;

    case WM_DESTROY:
      for (int i = 0; i < row; ++i)
        for (int j = 0; i < col; ++j) {
          if (pBlocks[i][j] != nullptr) delete pBlocks[i][j];
        }
      PostQuitMessage(0);
      break;
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool BalltoBlock(const Ball& ball, const Block& block) {
  return (ball.getx() < block.getx() + block.getWidth()) &&
         (ball.gety() < block.gety() + block.getHeight()) &&
         (ball.getx() + ball.getDiameter() > block.getx()) &&
         (ball.gety() + ball.getDiameter() > block.gety());
}

bool BalltoBar(const Ball& ball, const Bar& bar) {
  return (ball.getx() < bar.getx() + bar.getWidth()) &&
         (ball.gety() < bar.gety() + bar.getHeight()) &&
         (ball.getx() + ball.getDiameter() > bar.getx()) &&
         (ball.gety() + ball.getDiameter() > bar.gety());
}

void DeleteBlock(Block* block) { delete block; }

bool isDone() { return !Block::nBlocks; }