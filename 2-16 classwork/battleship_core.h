#ifndef BATTLESHIP_CORE_H_INCLUDED
#define BATTLESHIP_CORE_H_INCLUDED

#include <iostream>
#include <vector>

int runGame() {
	std::cout << "Welcome to Battleship!\n";
	return 0;
}

class Ship {
private:
    int pos_x;
    int pos_y;
    int direction;
    int length;
public:
    Ship():pos_x(0),pos_y(0),direction(0),length(1)
    {}
    
    Ship(int x, int y, int d, int l):pos_x(x),pos_y(y),direction(d),length(l)
    {}
    
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
    
    void placeShip(int x, int y, int d, int l)
    {
        Ship temp(x,y,d,l);
        boats.push_back(temp);
    }
    
    std::vector<Ship> getShips()
    {
        return boats;
    }
    
    bool checkShot(int x, int y)
    {
        if(boats[0](x,y))
            return true;
        return false;
    }
};



#endif