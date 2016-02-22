#ifndef BATTLESHIP_CORE_H_INCLUDED
#define BATTLESHIP_CORE_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>

int runGame() {
	std::cout << "Welcome to Battleship!\n";
	return 0;
}
//Class Ship
// is a ship
// pos_x and pos_y keep track of position origin being the top/left most point
// direction keeps track of ship orenetation
// length keeps track of spaces the ship takes up
// health keeps track ofif a ship is dead
class Ship {
private:
    int pos_x;
    int pos_y;
    int direction;
    int length;
    int health;
public:
    //default ctor
    //creates a ship at 0,0 that is 1 space long
    Ship():pos_x(0),pos_y(0),direction(0),length(1),health(length)
    {}
    
    //4 param ctor
    //creates a ship that is at position x,y is in the direction d and thakes up l spaces
    Ship(int x, int y, int d, int l):pos_x(x),pos_y(y),direction(d),length(l),health(length)
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
    bool operator()(int x, int y, bool shot)
    {
        bool hit = false;
        for(int i=0; i<length; ++i)
        {
            if(direction==0)
            {
                if(x==pos_x && y==pos_y+i)
                {
                    hit=true;
                    if(shot)
                    {
                    --health;
                        //std::cout<< "HEALTH : " <<health <<std::endl;
                    }
                }
            }else if(direction==1)
            {
                if(x==pos_x+i && y==pos_y)
                {
                    hit=true;
                    if(shot)
                    {
                    --health;
                        //std::cout<< "HEALTH : " <<health <<std::endl;
                    }
                }
            }
        }
        return hit;
    }
    
    bool alive()
    {
        if(health<1)
            return false;
        return true;
    }
    
    int getLength()
    {
        return length;
    }
    
    
};

typedef std::pair<std::pair<int,int>,bool> Shot; //used for player shots


//Class Board
// board_x and board_y are the bounds of the board
// Vector boats holds all the currently placed and alive boats
// Vector shots holds all shots made on the board
class Board {
private:
	int board_x; // the x bound for the board
	int board_y; // the y  ound for the board
    std::vector<Ship> boats; //holds all the ships
    std::vector<Shot> shots; //holds all of the shots that have been made
    std::vector<std::string> eBoard;
public:
    //
    //  Board Constructors
    //
    
    //default ctor
    //creates a board that is size 0,0
	Board() {
		board_x = 10;
		board_y = 10;
        eBoard = eBoardCreator(board_x, board_y);
	}
    
    //2 param ctor
    //creates a board that is size x,y
	Board(int x, int y) {
		board_x = x;
		board_y = y;
        eBoard = eBoardCreator(board_x, board_y);
	}
    
    //
    //  Acsessors
    //
    
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
    
    //getShips
    //returns the vector that contains the ships
    std::vector<Ship> getShips()
    {
        return boats;
    }
    
    //howManyShips
    //returns number of ships in play
    int howManyShips()
    {
        return boats.size();
    }
    
    //clearShips
    //Empties the boats vector
    void clearShips()
    {
        boats.erase(boats.begin(),boats.end());
    }
    
    //getShots
    //returns the vector of shots
    std::vector<Shot> getShots()
    {
        return shots;
    }
    
    //howManyShots
    //returns number of shots made
    int howManyShots()
    {
        return shots.size();
    }
    
    //clearShots
    //empties the shots vector
    void clearShots()
    {
        shots.erase(shots.begin(),shots.end());
    }
    
    //getEBoard
    //gets the empty board
    std::vector<std::string> getEBoard()
    {
        return eBoard;
    }
    
    std::vector<std::string> getShotsBoard()
    {
        int pos;
        int line;
        std::vector<std::string> shotBoard = eBoard;
        
        for(int i=0; i<shots.size(); ++i)
        {
            pos = shots[i].first.first;
            line = shots[i].first.second;
            if(shots[i].second)
            {
                shotBoard[line][pos]='H';
            }else{
                shotBoard[line][pos]='M';
            }
            
            
        }
        return shotBoard;
    }
    
    std::vector<std::string> getShipsBoard()
    {
        std::vector<std::string> sBoard = eBoard;
        std::pair<int,int> cords;
        int l;
        
        for(int i=0; i<boats.size(); ++i)
        {
            cords=boats[i][1];
            l=boats[i].getLength();
            switch(l)
            {
                case 5:
                {
                    for(int j=0;j<l;++j)
                    {
                        sBoard[boats[i][j].second][boats[i][j].first]='B';
                    }
                    break;
                }
                case 4:
                {
                    for(int j=0;j<l;++j)
                    {
                        sBoard[boats[i][j].second]='S';
                    }
                    break;
                }
                case 3:
                {
                    for(int j=0;j<l;++j)
                    {
                        sBoard[boats[i][j].second]='C';
                    }
                    break;
                }
                case 2:
                {
                    for(int j=0;j<l;++j)
                    {
                        sBoard[boats[i][j].second]='D';
                    }
                    break;
                }
                default:
                {
                    std::cout <<"Error in getShipsBoard" <<std::endl;
                    break;
                }
                    
                    
            }
        }
        
        return sBoard;
    }
    
    std::vector<std::string> numberBoard(std::vector<std::string> b)
    {
        b.insert(b.begin(),"X0123456789");
        char place = 'A';
        for(int i=1; i<=board_y;++i)
        {
            b[i].insert(b[i].begin(), place);
            ++place;
        }
        return b;
    }
    
    //
    //  Functions used durring gameplay
    //
    
    //defaultSetup
    //gets the player to place their ships on the board
    bool defaultSetup()
    {
        int i=0;
        int x = -1;
        int y = -1;
        int d =  0;
        int l[5] =  {5,4,3,3,2};
        while (i<5)
        {
            std::cout <<" ship to place: ";
            switch(i)
            {
                case 0:
                {
                    std::cout <<"Battleship 5 spaces" <<std::endl;
                    break;
                }
                case 1:
                {
                    std::cout <<"Submarine 4 spaces" <<std::endl;
                    break;
                }
                case 2:
                {
                    std::cout <<"Cruiser 3 spaces" <<std::endl;
                    break;
                }
                case 3:
                {
                    std::cout <<"Carrier 3 spaces" <<std::endl;
                    break;
                }
                case 4:
                {
                    std::cout <<"Destroyer 2 spaces" <<std::endl;
                    break;
                }
                default:
                {
                    std::cout <<"error in board.defaultSetup()";
                    break;
                }
                
            }
            std::cout <<" x Pos for the ship :";
            std::cin >> x;
            std::cout <<" y Pos for the ship :";
            std::cin >> y;
            std::cout <<" 0 for vertical 1 for horizontal :";
            std::cin >> d;
            if(placeShip(x,y,d,l[i]))
            {
                ++i;
                x=-1;
                y=-1;
            }else
            {
                std::cout << " The ship was placed in an invalid position" <<std::endl;
                
            }
        }
        return true;
    }
    
    //placeShip
    //takes 4 ints
    //places a ship at x,y that is in the direction d and is length l
    //makes sure that it is not out of bounds or overlaping another ship
    // returns true if ship is placed
    bool placeShip(int x, int y, int d, int l)
    {
        if(!inBounds(x,y) || l<1)
        {
            return false;
        }
        if(d==1)
        {
            if(!inBounds(x+l, y))
            {
                return false;
            }
            for(int i=0; i<l; ++i)
            {
                if(checkSpace(x+i, y))
                {
                    return false;
                }
            }
        }else if(d==0)
        {
            if(!inBounds(x, y+l))
            {
                return false;
            }
            for(int i=0; i<l; ++i)
            {
                if(checkSpace(x, y+i))
                {
                    return false;
                }
            }
        }
        
        Ship temp(x,y,d,l);
        boats.push_back(temp);
        return true;
    }
    
    //checkSpace
    //takes two ints
    //returns true if the space is ocupied by a ship
    bool checkSpace(int x, int y,bool shot=false)
    {
        for(int i=0; i<boats.size(); ++i)
        {
            if(boats[i](x,y,shot))
            {
                return true;
            }
        }
        return false;
    }
    
    //checkShot
    //returns true if the shot has been made already
    bool checkShot(int x, int y)
    {
        for(int i=0; i<shots.size(); ++i)
        {
            if(x==shots[i].first.first && y==shots[i].first.second)
                return true;
        }
        return false;
    }
    
    //addShot
    //places a shot in the Shots vector at location x,y
    // hit tracks if it was a hit or miss
    void addShot(int x, int y, bool hit)
    {
        shots.push_back(std::make_pair(std::make_pair(x,y),hit));
    }
    
    //makeShot
    //checks if a shot is a hit or miss and records it
    // return of  0 means miss
    // return of  1 means hit
    // return of -1 means shot is a repeat
    // return of -2 means out of bounds
    int makeShot(int x, int y)
    {
        if(!inBounds(x,y))
        {
            return -2;
        }
        if(checkShot(x,y))
        {
            return -1;
        }
        if(checkSpace(x,y,true))
        {
            addShot(x,y,true);
            return 1;
        }
        addShot(x,y,false);
        return 0;
    }
    
    //inBounds
    //Checks if the x,y point is in the board
    bool inBounds(int x, int y)
    {
        if(x<0 || y<0 || x>=board_x || y>=board_y)
        {
            return false;
        }
        return true;
    }
    
    //eBoardCreator
    //makes a empty board of the entered size
    std::vector<std::string> eBoardCreator(int x, int y)
    {
        std::vector<std::string> pBoard;
        std::string line;
        for(int i=0; i<x; ++i)
        {
            line.append("~");
        }
        for(int i=0;i<y;++i)
        {
            pBoard.push_back(line);
        }
        
        return pBoard;
    }
    
    void turn()
    {
        //shot function
        //std::cout<<boats.size() <<std::endl;
        for(int i=0; i<boats.size(); ++i)
        {
            if(!boats[i].alive())
            {
                boats.erase(boats.begin()+i);
            }
        }
        //std::cout<<boats.size() <<std::endl;
    }
    
    bool lost()
    {
        if(boats.size()<1)
            return true;
        return false;
    }
    
    //
    //Output functions
    //
    
    //printBoard
    //displays the board
    void printBoard(int mode)
    {
        switch(mode)
        {
            case 1: //prints shots made
            {
                for(int i=0; i<=board_y; ++i)
                {
                    std::cout<<numberBoard(getShotsBoard())[i] <<std::endl;
                }
                break;
            }
            case 2: //prints ships
            {
                for(int i=0; i<=board_y; ++i)
                {
                    std::cout<<numberBoard(getShipsBoard())[i] <<std::endl;
                }
                break;
            }
            default:
            {
                std::cout <<"Incorrect call to board.printBoard(int)" <<std::endl;
                break;
            }
        }
    }
    
};



#endif