#include "battleship_core.h"
#include <string>
#include <iostream>
#include <vector>

int main() {
    std::string name;
	Board player1(10, 10,"Player1");
	Board player2(10, 10,"Player2");
    player1.nameSetup();
	player1.defaultSetup();
    std::cout <<"Please pass the game and the press ENTER." <<std::endl;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
    player2.nameSetup();
	player2.defaultSetup();
	while (true)
	{
		player1.turn(player2);
		if (player2.lost())
        {
            std::cout <<"\n\n\n\n\n\n\n\n\n" << player1.getName() << ": You have won! Congraduations" <<std::endl;
            name = player1.getName();
            break;
        }
        std::cout <<player1.getName() <<" pass to " <<player2.getName() <<std::endl;
        player2.turn(player1);
        if (player1.lost())
        {
            std::cout <<"\n\n\n\n\n\n\n\n\n" << player2.getName() << ": You have won! Congraduations" <<std::endl;
            name = player2.getName();
            break;
        }
	}
    
	return 0;
}