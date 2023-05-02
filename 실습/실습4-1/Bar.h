#pragma once
#include <windows.h>

struct Bar {
  int x_, y_, width_, height_, dir_;
  COLORREF color_;

  Bar() {}
  Bar(int x, int y, int dir, int width, int height, const COLORREF& color)
      : x_{x},
        y_{y},
        width_{width},
        height_{height},
        dir_{dir},
        color_{color} {}
  ~Bar() {}
  void Clip(int max_x);
  void Move(int max_x);
  void Draw(const HDC& hdc);
};