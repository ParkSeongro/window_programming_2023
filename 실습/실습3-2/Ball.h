#pragma once
#include <Windows.h>
class Ball {
  

 public:
     int x_, y_, diameter_, xdir_, ydir_;
     COLORREF color_;
     void Clip(int max_x, int max_y);
  Ball() {}
  Ball(int x, int y, int diameter, int xdir, int ydir, const COLORREF& color)
      : x_{x},
        y_{y},
        diameter_{diameter},
        xdir_{xdir},
        ydir_{ydir},
        color_{color} {}
  ~Ball() {}
  int getx() const { return x_; }
  int gety() const { return y_; }
  int getDiameter() const { return diameter_; }
  int getxdir() const { return xdir_; }
  int getydir() const { return ydir_; }
  COLORREF getColor() const { return color_; }
  void setx(int x) { x_ = x; }
  void sety(int y) { y_ = y; }
  void setxdir(int xdir) { xdir_ = xdir; }
  void setydir(int ydir) { ydir_ = ydir; }
  void setDiameter(int diameter) { diameter_ = diameter; }
  void setColor(const COLORREF& color) { color_ = color; }
  void Move(int max_x, int max_y);
  void Draw(const HDC& hdc) const;
};