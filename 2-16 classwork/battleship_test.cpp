#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "battleship_core.h"
#include <iostream>
#include <vector>


TEST_CASE("test case of TEST_CASE") {
	REQUIRE(runGame() == 0);
}

TEST_CASE("Test Case: Board Bound") {

	Board testBoard(10,10);

	REQUIRE(testBoard.getX() == 10);

	REQUIRE(testBoard.getY() == 10);
}

TEST_CASE("Test Case: Ship creation and placement") {
    
    std::vector<int> t_x;
    std::vector<int> t_y;
    std::vector<int> t_d;
    std::vector<int> t_l;
    
    t_x.push_back(0);
    t_y.push_back(0);
    t_d.push_back(0);
    t_l.push_back(3);
    
    t_x.push_back(4);
    t_y.push_back(4);
    t_d.push_back(1);
    t_l.push_back(4);
    
    t_x.push_back(2);
    t_y.push_back(4);
    t_d.push_back(0);
    t_l.push_back(5);
    
    Board testBoard;
    std::vector<Ship> testShips;
    
    for(int i=0; i<t_x.size(); ++i)
        testBoard.placeShip(t_x[i], t_y[i], t_d[i], t_l[i]);
    
    testShips = testBoard.getShips();
    
    REQUIRE(testShips[0][0]==std::make_pair(0, 0));
    REQUIRE(testShips[0][1]==std::make_pair(0, 1));
    REQUIRE(testShips[0][2]==std::make_pair(0, 2));
    REQUIRE(testShips[0][3]==std::make_pair(-1, -1));
    
    REQUIRE(testShips[1][0]==std::make_pair(4, 4));
    REQUIRE(testShips[1][1]==std::make_pair(5, 4));
    REQUIRE(testShips[1][2]==std::make_pair(6, 4));
    REQUIRE(testShips[1][3]==std::make_pair(7, 4));
    REQUIRE(testShips[1][4]==std::make_pair(-1,-1));
    
    REQUIRE(testShips[2][0]==std::make_pair(2, 4));
    REQUIRE(testShips[2][1]==std::make_pair(2, 5));
    REQUIRE(testShips[2][2]==std::make_pair(2, 6));
    REQUIRE(testShips[2][3]==std::make_pair(2, 7));
    REQUIRE(testShips[2][4]==std::make_pair(2, 8));
    REQUIRE(testShips[2][5]==std::make_pair(-1,-1));
    
}

TEST_CASE("Test Case: Check a fired shot"){
    std::vector<int> t_x;
    std::vector<int> t_y;
    std::vector<int> t_d;
    std::vector<int> t_l;
    
    t_x.push_back(0);
    t_y.push_back(0);
    t_d.push_back(0);
    t_l.push_back(3);
    
    Board testBoard;
    
    for(int i=0; i<t_x.size(); ++i)
        testBoard.placeShip(t_x[i], t_y[i], t_d[i], t_l[i]);
    
    REQUIRE(testBoard.checkShot(t_x[0], t_y[0]-1)==false);
    REQUIRE(testBoard.checkShot(t_x[0], t_y[0])==true);
    REQUIRE(testBoard.checkShot(t_x[0], t_y[0]+1)==true);
    REQUIRE(testBoard.checkShot(t_x[0], t_y[0]+2)==true);
    REQUIRE(testBoard.checkShot(t_x[0], t_y[0]+3)==false);
    REQUIRE(testBoard.checkShot(t_x[0]+1, t_y[0])==false);
    
}

