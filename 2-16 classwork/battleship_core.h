#ifndef BATTLESHIP_CORE_H_INCLUDED
#define BATTLESHIP_CORE_H_INCLUDED

#include <iostream>

int runGame() {
	std::cout << "Welcome to Battleship!\n";
	return 0;
}

class Board {
private:
	int board_x;
	int board_y;
public:
	Board() {
		board_x = 0;
		board_y = 0;
	}
	Board(int x, int y) {
		board_x = x;
		board_y = y;
	}
	int getX() {
		return board_x;
	}
	int getY() {
		return board_y;
	}
};

#endif