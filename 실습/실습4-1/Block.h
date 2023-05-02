#pragma once
#include <Windows.h>

struct Block {
  int x_, y_, xdir_, width_, height_, min_, max_;
  COLORREF color_;
  bool isCrashed_;

  static int nBlocks, nCrashed;
  Block() { ++nBlocks; }
  Block(int x, int y, int xdir, int width, int height, const COLORREF& color,
        bool isCrashed)
      : x_{x},
        y_{y},
        xdir_{xdir},
        width_{width},
        height_{height},
        color_{color},
        isCrashed_{isCrashed} {
    ++nBlocks;
    min_ = x;
    max_ = x + 150;
  }
  ~Block() { --nBlocks; }
  void Clip();
  void Move();
  void Draw(HDC hdc) const;
};