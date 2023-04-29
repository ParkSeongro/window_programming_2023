#include "Block.h"

int Block::nBlocks = 0;

void Block::Clip() { // 이동 범위 제한
  if (x_ < min_) {
    x_ = min_;
    xdir_ = -xdir_;    
  } else if (x_ + width_ > max_) {
    x_ = max_ - width_;
    xdir_ = -xdir_;   
  }  
}

void Block::Move() {
  x_ += xdir_;
  Clip();
}

void Block::Draw(const HDC hdc) const {
  HBRUSH oldBrush, hBrush;
  hBrush = CreateSolidBrush(color_);
  oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Rectangle(hdc, x_, y_, x_ + width_, y_ + height_);
  SelectObject(hdc, oldBrush);
  DeleteObject(hBrush);
}