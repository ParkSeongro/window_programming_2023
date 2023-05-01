#pragma once
#include <windows.h>

class Bar {
  

 public:
     int x_, y_, width_, height_, dir_;
     COLORREF color_;
     void Clip(int max_x);
  Bar() {}
  Bar(int x, int y, int dir, int width, int height, const COLORREF& color)
      : x_{x},
        y_{y},
        width_{width},
        height_{height},
        dir_{dir},
        color_{color} {}
  ~Bar() {}
  int getx() const { return x_; }
  int gety() const { return y_; }
  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  int getDir() const { return dir_; }
  void setx(int x) { x_ = x; }
  // void sety(int y) { y_ = y; }
  // ...
  void setDir(int dir) { dir_ = dir; }
  void Move(int max_x);
  void Draw(const HDC& hdc);
};