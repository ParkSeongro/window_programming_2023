#include "Bar.h"

void Bar::Clip(int max_x) {
  if (x_ < 0)
    x_ = 0;
  else if (x_ + width_ > max_x)
    x_ = max_x - width_;
}

void Bar::Move(int max_x) {
  x_ += dir_;
  Clip(max_x);
}

void Bar::Draw(const HDC& hdc) {
  HBRUSH oldBrush, hBrush;
  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Rectangle(hdc, x_, y_, x_ + width_, y_ + height_);
  SelectObject(hdc, oldBrush);
  DeleteObject(hBrush);
}