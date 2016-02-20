#ifndef BATTLESHIP_CORE_H_INCLUDED
#define BATTLESHIP_CORE_H_INCLUDED

#include <iostream>
#include <vector>

int runGame() {
	std::cout << "Welcome to Battleship!\n";
	return 0;
}
//Class Ship
// is a ship
// pos_x and pos_y keep track of position
// direction keeps track of ship orenetation
// length keeps track of spaces the ship takes up
class Ship {
private:
    int pos_x;
    int pos_y;
    int direction;
    int length;
public:
    //default ctor
    //creates a ship at 0,0 that is 1 space long
    Ship():pos_x(0),pos_y(0),direction(0),length(1)
    {}
    
    //4 param ctor
    //creates a ship that is at position x,y is in the direction d and thakes up l spaces
    Ship(int x, int y, int d, int l):pos_x(x),pos_y(y),direction(d),length(l)
    {}
    
    //operator[]
    //returns the cords of ship section indx
    std::pair<int,int> operator[] (int indx)
    {
        if(indx>=0 && indx<length)
        {
            if(direction==0)
                return std::make_pair(pos_x, pos_y+indx);
        
            if(direction==1)
                return std::make_pair(pos_x+indx, pos_y);
        }
        
        return std::make_pair(-1,-1);
    }
    
    //operator()
    //takes two ints
    //returns true if the ship ocupies the space x,y
    bool operator()(int x, int y)
    {
        bool hit = false;
        for(int i=0; i<length; ++i)
        {
            if(direction==0)
            {
                if(x==pos_x && y==pos_y+i)
                {
                    hit=true;
                }
            }else if(direction==1)
            {
                if(x==pos_x+i && y==pos_y)
                {
                    hit=true;
                }
            }
        }
        return hit;
    }
};

class Board {
private:
	int board_x;
	int board_y;
    std::vector<Ship> boats;
public:
    //default ctor
    //creates a board that is size 0,0
	Board() {
		board_x = 0;
		board_y = 0;
	}
    
    //2 param ctor
    //creates a board that is size x,y
	Board(int x, int y) {
		board_x = x;
		board_y = y;
	}
    
    //getX
    //returns the size of the board in the x direction
	int getX() {
		return board_x;
	}
    
    //getY
    //returns the size of the board in the y direction
	int getY() {
		return board_y;
	}
    
    //placeShip
    //takes 4 ints
    //places a ship at x,y that is in the direction d and is length l
    void placeShip(int x, int y, int d, int l)
    {
        Ship temp(x,y,d,l);
        boats.push_back(temp);
    }
    
    //getShips
    //returns the vector that contains the ships
    std::vector<Ship> getShips()
    {
        return boats;
    }
    
    //checkShot
    //takes two ints
    //returns true if the space is ocupied by a ship
    bool checkShot(int x, int y)
    {
        if(boats[0](x,y))
            return true;
        return false;
    }
};



#endif