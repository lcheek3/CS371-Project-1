#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "battleship_core.h"
#include <iostream>
#include <vector>

Board testBoard(10,10);

TEST_CASE("test case of TEST_CASE") {
	REQUIRE(runGame() == 0);
}

TEST_CASE("Test Case: Board Bound") {

	REQUIRE(testBoard.getX() == 10);

	REQUIRE(testBoard.getY() == 10);
}

TEST_CASE("Checks if inBounds works")
{
    REQUIRE(testBoard.inBounds(-1, 0)==false);
    REQUIRE(testBoard.inBounds(10, 0)==false);
    REQUIRE(testBoard.inBounds(0, -1)==false);
    REQUIRE(testBoard.inBounds(0, 10)==false);
    
    REQUIRE(testBoard.inBounds(5, 5)==true);
}

TEST_CASE("Test Case: Ship creation and placement and clear")
{
    REQUIRE(testBoard.placeShip(2,0,0,3)==true);
    REQUIRE(testBoard.placeShip(4,4,1,4)==true);
    REQUIRE(testBoard.placeShip(2,4,0,5)==true);
    
    
    REQUIRE(testBoard.placeShip(0, 0, 1, 3)==false);
    
    REQUIRE(testBoard.placeShip(-1, 3, 0, 3)==false);
    REQUIRE(testBoard.placeShip(10, 3, 0, 3)==false);
    
    REQUIRE(testBoard.howManyShips()==3);
    
    testBoard.clearShips();
    
    REQUIRE(testBoard.howManyShips()==0);
}

TEST_CASE("Test Case: Checks shooting"){
    int t_x =2;
    int t_y= 2;
    int t_d =0;
    int t_l= 3;
    
    REQUIRE(testBoard.placeShip(t_x, t_y, t_d, t_l)==true);
    
    //check shots accros a ship and around it
    //shot total 6
    REQUIRE(testBoard.makeShot(t_x,  t_y-1)==0);
    REQUIRE(testBoard.makeShot(t_x,  t_y  )==1);
    REQUIRE(testBoard.makeShot(t_x,  t_y+1)==1);
    REQUIRE(testBoard.makeShot(t_x,  t_y+2)==1);
    REQUIRE(testBoard.makeShot(t_x,  t_y+3)==0);
    REQUIRE(testBoard.makeShot(t_x+1, t_y )==0);
    
    //out of bounds tests
    REQUIRE(testBoard.makeShot(0, -1)==-2);
    REQUIRE(testBoard.makeShot(0, 10)==-2);
    REQUIRE(testBoard.makeShot(-1, 0)==-2);
    REQUIRE(testBoard.makeShot(10, 0)==-2);
    
    //check repeat shot
    REQUIRE(testBoard.makeShot(t_x,  t_y+2)==-1);
    
    //checks number of shots made
    REQUIRE(testBoard.howManyShots()==6);
    
    testBoard.clearShots();
    
    REQUIRE(testBoard.howManyShots()==0);
    
    testBoard.clearShips();
}

//TEST_CASE("Test Case: Setup")
//{
//    testBoard.defaultSetup();
//}

TEST_CASE("Test Case: Printing"){
    
    testBoard.placeShip(2, 2, 0, 5);
    
    std::vector<std::string> good;
    for(int i=0; i<10; ++i)
        good.push_back("~~~~~~~~~~");
    
    std::vector<std::string> good2;
    good2.push_back("M~~~~~~~~~");//
    good2.push_back("~~~~~~~~~~");//
    good2.push_back("~~~~~~~~~~");
    good2.push_back("~~~~~~~~~~");
    good2.push_back("~~~~~~~~~~");
    good2.push_back("~~~~~M~~~~");
    good2.push_back("~~~~~~~~~~");
    good2.push_back("~~~~~~~~~~");
    good2.push_back("~~~~~~~~~~");
    good2.push_back("M~~~~~~~~M");
    
    REQUIRE(testBoard.eBoardCreator(10, 10)==good);
    
    testBoard.makeShot(5, 5);
    testBoard.makeShot(0, 0);
    testBoard.makeShot(9, 9);
    testBoard.makeShot(0, 9);
    
    REQUIRE(testBoard.getShotsBoard()==good2);
    
    //
    // A few visual tests
    //
    testBoard.printBoard(2);
    testBoard.printBoard(1);
    
    testBoard.makeShot(2, 2);
    
    testBoard.printBoard(1);
    
    testBoard.clearShips();
    testBoard.clearShots();
    
}

TEST_CASE("Test Case: Game loss")
{
    testBoard.placeShip(0, 0, 1, 2);
    
    REQUIRE(testBoard.makeShot(0, 0)==1);
    REQUIRE(testBoard.makeShot(1, 0)==1);
    
    testBoard.turn();
    
    REQUIRE(testBoard.lost()==true);
    
}


