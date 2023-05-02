# include "Game.h"

void Game::Init(const HWND& hWnd, int w, int h, int r) {
  width = w;
  height = h;
  row = r;
  KillTimer(hWnd, 1);
  bar.x_ = 300;
  bar.y_ = 500;
  bar.dir_ = 0;
  bar.width_ = 80;
  bar.height_ = 20;
  bar.color_ = RGB(128, 128, 128);
  ball.x_ = 310;
  ball.y_ = 400;
  ball.diameter_ = 20;
  ball.xdir_ = 1;
  ball.ydir_ = -1;
  ball.color_ = RGB(225, 225, 0);
  ball.speed_ = 2;
  play = false;
  pause = false;
  isCrashed = false;
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < 10; ++j) {
      if (pBlocks[i][j] != nullptr) delete pBlocks[i][j];
    }
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < 10; ++j)
      pBlocks[i][j] = new Block{10 + j * 60, 30 + i * 30,        1,    60,
                                30,          RGB(255, 255, 255), false};
  InvalidateRgn(hWnd, NULL, TRUE);
}

void Game::Play(const HWND& hWnd) { 
  SetTimer(hWnd, 1, 1, NULL);
  play = true;
  ball.Move(width, height + ball.diameter_);
  bar.Move(width);
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < 10; ++j) {
      if (pBlocks[i][j] != nullptr) {
        pBlocks[i][j]->Move();
        if (BalltoBlock(ball,
                        *pBlocks[i][j])) {                                            
          if (!pBlocks[i][j]->isCrashed_) {
            pBlocks[i][j]->color_ =
                RGB(rand() % 256, rand() % 256, rand() % 256);
            pBlocks[i][j]->height_ = pBlocks[i][j]->height_ - 10;
            pBlocks[i][j]->width_ = pBlocks[i][j]->width_ - 10;
            pBlocks[i][j]->isCrashed_ = true;
            ++Block::nCrashed;
          } else {
            DeleteBlock(pBlocks[i][j]);
            pBlocks[i][j] = nullptr;
            --Block::nCrashed;
          }
          ball.ydir_ = -ball.ydir_;
        }
      }
    }
  if (BalltoBar(ball, bar)) ball.ydir_ = -ball.ydir_;
  InvalidateRgn(hWnd, NULL, TRUE);
}

void Game::End(const HWND& hWnd) {
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < 10; ++j) {
      if (pBlocks[i][j] != nullptr) delete pBlocks[i][j];
    }
  KillTimer(hWnd, 1);
  PostQuitMessage(0);
}

bool Game::BalltoBlock(const Ball& ball, const Block& block) {
  return (ball.x_ < block.x_ + block.width_) &&
         (ball.y_ < block.y_ + block.height_) &&
         (ball.x_ + ball.diameter_ > block.x_) &&
         (ball.y_ + ball.diameter_ > block.y_);
}

bool Game::BalltoBar(const Ball& ball, const Bar& bar) {
  return (ball.x_ < bar.x_ + bar.width_) && (ball.y_ < bar.y_ + bar.height_) &&
         (ball.x_ + ball.diameter_ > bar.x_) &&
         (ball.y_ + ball.diameter_ > bar.y_);
}

bool Game::InBar(const Bar& bar, int mx, int my) {
  return (mx < bar.x_ + bar.width_) && (my < bar.y_ + bar.width_) &&
         (mx > bar.x_) && (my > bar.y_);
}

void Game::DeleteBlock(Block* block) { delete block; }

void Game::NonClick(const HWND& hWnd, const LPARAM& lParam) {  
  selection = false;
  bar.x_ = 300;
  InvalidateRect(hWnd, NULL, TRUE);
}

void Game::Click(const HWND& hWnd, const LPARAM& lParam) {
  int mx = LOWORD(lParam);
  int my = HIWORD(lParam);
  if (InBar(bar, mx, my)) selection = true;
}

void Game::MoveBar(const HWND& hWnd, const LPARAM& lParam) {
  int mx = LOWORD(lParam);
  int my = HIWORD(lParam);
  if (selection) {
    bar.x_ = mx;
    bar.Clip(800);
    InvalidateRect(hWnd, NULL, TRUE);
  }
  
}

void Game::Draw(const HDC& hDC) {
  
  ball.Draw(hDC);
  bar.Draw(hDC);
  for (int i = 0; i < row; ++i)
    for (int j = 0; j < 10; ++j) {
      if (pBlocks[i][j] != nullptr) pBlocks[i][j]->Draw(hDC);
    }
  if (pause) PrintStatus(hDC);
  
}

void Game::SpeedUp() {
  ball.speed_ += 2;
  if (ball.speed_ > 10) ball.speed_ = 10;
}

void Game::SpeedDown() {
  ball.speed_ -= 2;
  if (ball.speed_ < 1) ball.speed_ = 1;
}

void Game::Pause(const HWND& hWnd) { 
   if (play) {
    if (pause) {
      SetTimer(hWnd, 1, 2, NULL);
      pause = false;
    } else {
      KillTimer(hWnd, 1);
      pause = true;
    }
  }
   InvalidateRect(hWnd, NULL, TRUE);
}

void Game::PrintStatus(const HDC& hdc) {
   wsprintf(str, L"colored: %d, destroyed: %d", Block::nCrashed,
            row * 10 - Block::nBlocks);
   TextOut(hdc, 300, 300, str, lstrlen(str)); 
   TextOut(hdc, 300, 316, L"s: 게임 시작", lstrlen(L"s: 게임 시작"));
   TextOut(hdc, 300, 332, L"+/-: 속도 조절", lstrlen(L"+/-: 속도 조절"));
   TextOut(hdc, 300, 348, L"n: 게임 리셋", lstrlen(L"n: 게임 리셋"));
   TextOut(hdc, 300, 364, L"p: 게임 일시중지/해제",
           lstrlen(L"p: 게임 일시중지/해제"));
}