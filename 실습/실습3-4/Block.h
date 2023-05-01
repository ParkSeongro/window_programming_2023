#pragma once
#include <Windows.h>

class Board {
	int nx, ny, dim;

public:
	void Draw(HDC hdc);
};

class Tile {
	int x, y, xDir, yDir, type;
	bool accumulated;

public:
	Tile() {}
	Tile(int xx, int yy, int xxDir, int yyDir, bool acc)
		: x{ xx }, y{ yy }, xDir{ xxDir }, yDir{ yyDir }, accumulated{ acc }, type {}
	~Tile() {}
};

class Block {
	Tile Tiles[3];
	int type;

public:
};