#pragma once
#include <Windows.h>

struct Ball {
  int x_, y_, diameter_, xdir_, ydir_, speed_;
  COLORREF color_;
  void Clip(int max_x, int max_y);
  Ball() {}
  Ball(int x, int y, int diameter, int xdir, int ydir, int speed,
       const COLORREF& color)
      : x_{x},
        y_{y},
        diameter_{diameter},
        xdir_{xdir},
        ydir_{ydir},
        speed_{speed},
        color_{color} {}
  ~Ball() {}
  void Move(int max_x, int max_y);
  void Draw(const HDC& hdc) const;
};