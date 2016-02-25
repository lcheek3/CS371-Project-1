#include "battleship_core.h"
#include <iostream>
#include <vector>

int main() {
	bool over=false;
	Board player1(10, 10);
	Board player2(10, 10);
	player1.defaultSetup();
	std::cout << "player 2: time to setup!" << std::endl;
	player2.defaultSetup();
	while (!over)
	{
		player1.turn(player2);
		if (player2.lost)
			over = true;
	}

	return 0;
}