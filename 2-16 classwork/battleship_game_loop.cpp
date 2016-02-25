#include "battleship_core.h"
#include <iostream>
#include <vector>

int main() {
	Board player1(10, 10,"Player1");
	Board player2(10, 10,"Player2");
    player1.nameSetup();
	player1.defaultSetup();
    std::cout <<"Please pass the game" <<std::endl;
    player2.nameSetup();
	player2.defaultSetup();
	while (true)
	{
		player1.turn(player2);
		if (player2.lost())
        {
            std::cout <<player1.getName() <<" Wins!" <<std::endl;
            break;
        }
        std::cout <<player1.getName() <<" pass to " <<player2.getName() <<std::endl;
        player2.turn(player1);
        if (player1.lost())
        {
            std::cout <<player2.getName() <<" Wins!" <<std::endl;
            break;
        }
        
	}

	return 0;
}