#include "Ball.h"

void Ball::Clip(int max_x, int max_y) {
  if (x_ < 0) {
    x_ = 0;
    xdir_ = -xdir_;
  } else if (x_ + diameter_ > max_x) {
    x_ = max_x - diameter_;
    xdir_ = -xdir_;
  } else if (y_ < 0) {
    y_ = 0;
    ydir_ = -ydir_;
  } else if (y_ + diameter_ > max_y) {
    y_ = max_y - diameter_;
    y_ = 0;
  }
}

void Ball::Move(int max_x, int max_y) {
  Clip(max_x, max_y);
  x_ += speed_ * xdir_;
  y_ += speed_ * ydir_;
}

void Ball::Draw(const HDC& hdc) const {
  HBRUSH oldBrush, hBrush;
  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Ellipse(hdc, x_, y_, x_ + diameter_, y_ + diameter_);
  SelectObject(hdc, oldBrush);
  DeleteObject(hBrush);
}