#pragma once
#include <Windows.h>

class Block {
  int x_, y_, xdir_, width_, height_, min_, max_;
  COLORREF color_;
  bool isCrashed_;

  void Clip();
  
 public:
  static int nBlocks;
  Block() { ++nBlocks; }
  Block(int x, int y, int xdir, int width, int height, const COLORREF& color, bool isCrashed)
      : x_{x},
        y_{y},
        xdir_{xdir},
        width_{width},
        height_{height},
        color_{color},
        isCrashed_{isCrashed} {
    ++nBlocks;
    min_ = x;
    max_ = x + 200;
  }
  ~Block() { --nBlocks; }
  int getx() const { return x_; }
  int gety() const { return y_; }
  int getxdir() const { return xdir_; }
  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  bool getIsCrashed() const { return isCrashed_; }
  void setx(int x) { x_ = x; }
  void sety(int y) { y_ = y; }  
  void setWidth(int width) { width_ = width; }
  void setHeight(int height) { height_ = height; }
  void setColor(const COLORREF& color) { color_ = color; }
  void setIsCrashed(bool isCrashed) { isCrashed_ = isCrashed; }

  
  void Move();
  void Draw(HDC hdc) const;
};