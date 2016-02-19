#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "battleship_core.h"
#include <iostream>



TEST_CASE("test case of TEST_CASE") {
	REQUIRE(runGame() == 0);
}

TEST_CASE("Test Case: Board Bound") {

	Board testBoard(10,10);

	REQUIRE(testBoard.getX() == 10);

	REQUIRE(testBoard.getY() == 10);
}