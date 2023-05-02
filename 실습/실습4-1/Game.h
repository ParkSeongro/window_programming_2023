#pragma once
#include <tchar.h>
#include "Ball.h"
#include "Bar.h"
#include "Block.h"

class Game {
 public:
  Game() {}
  Game(int width_, int height_) : width{width_}, height{height_} {}
  ~Game() {
    for (int i = 0; i < row; ++i)
      for (int j = 0; j < 10; ++j) {
        if (pBlocks[i][j] != nullptr) delete pBlocks[i][j];
      }
  }
  void setColor(COLORREF color) {
    for (int i = 0; i < row; ++i)
      for (int j = 0; j < 10; ++j) pBlocks[i][j]->color_ = color;
  }
  void setBallSize(int size) { ball.diameter_ = size; }
  void setRow(int row_) { row = row_; }
  void Play(const HWND& hWnd);
  void Init(const HWND& hWnd, int w, int h, int r);
  void Pause(const HWND& hWnd);
  void End(const HWND& hWnd);
  void SpeedUp();
  void SpeedDown();
  void Draw(const HDC& hDC);
  void NonClick(const HWND& hWnd, const LPARAM& lParam);
  void Click(const HWND& hWnd, const LPARAM& lParam);
  void MoveBar(const HWND& hWnd, const LPARAM& lParam);

 private:
  int row, width, height;
  bool play, pause, isCrashed, selection;
  Ball ball;
  Bar bar;
  Block* pBlocks[10][10];
  TCHAR str[30];
  bool BalltoBlock(const Ball& ball, const Block& block);
  bool BalltoBar(const Ball& ball, const Bar& bar);
  void DeleteBlock(Block* block);
  bool InBar(const Bar& bar, int mx, int my);
  void PrintStatus(const HDC& hdc);
};