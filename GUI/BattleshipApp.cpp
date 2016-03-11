//Lucas Cheek, Max Hesser-Knoll, Nick Segler
//CS371
//Project 1, GUI module



#include "cinder/app/App.h"
#include "cinder/app/Window.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "battleship_core.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//Warning: I havent used Cinder or OpenGL in years, I have no idea what I'm doing, this code is a disgusting mess.

//BattleshipApp class. Contains loop functions and various shared variables between functions
class BattleshipApp : public App {
	public:
		void setup() override;
		void keyDown(app::KeyEvent event);
		void mouseDown(MouseEvent event);
		void mouseMove(MouseEvent event);
		void update() override;
		void draw() override;
		void drawBoard(int);
		void drawEnd(string);
		void drawTurnScreen(string);
		void miniCell(int, int, Board*);
		void cell(int, int, Board*);
		void button(Board*, Board*);
	private:
		bool shipPlaced;
		bool leftClick;
		bool pressSpace;
		bool initialShip;
		bool refreshShipHighlights;
		vec2 clickLocation;
		vec2 lastClickedCell;
		vec2 currentClickedCell;
		int shot_result;
		bool made_shot;
		vec2 movePoint;
		vector<vec2> shipHighlights;
		Board player1;
		Board player2;
		Path2d mPath;
		Font mFont;
		vector<int> ships;
		int currentShip;
		gl::TextureRef mTextTexture;
		int CELL_SIZE;
		int CELL_PAD;
		int gameState;
		int turn;
		bool endTurn;
		bool beginVictoryCheck;

};

//Battleship::Cell
//Draws and manages the cells in the main play grid
//in Gamestate 1: manages the shipHighlights vector and compares  current clicked cell and last clicked cell to draw cell highlights. Indicates ship placement
//in Gamestate 3: stores and manages shots fired. Displays players shots and whether they were a hit or miss
// in all gamestates: creates and draws paths for each cell.
void BattleshipApp::cell(int x, int y, Board* player){
	Color red = Color(1.0, 0.0, 0.0);
	Color highlight = Color(0.8, 1.0, 0.8);
	int current_pos_x = CELL_PAD + (CELL_SIZE + CELL_PAD)*x;
	int current_pos_y = CELL_PAD + (CELL_SIZE + CELL_PAD)*y;
	Path2d mPath;
	mPath.moveTo(current_pos_x, current_pos_y);
	mPath.lineTo(CELL_SIZE + current_pos_x, current_pos_y);
	mPath.lineTo(CELL_SIZE + current_pos_x, CELL_SIZE + current_pos_y);
	mPath.lineTo(current_pos_x, CELL_SIZE + current_pos_y);
	mPath.close();
	if (gameState == 1){
		if (mPath.contains(clickLocation)){
			if (!initialShip){
				lastClickedCell.x = x;
				lastClickedCell.y = y;
				initialShip = true;
			}
			else{
				if (refreshShipHighlights){
					shipHighlights.clear();
					refreshShipHighlights = false;
				}
				currentClickedCell.x = x;
				currentClickedCell.y = y;
				if (currentClickedCell.x == lastClickedCell.x && currentClickedCell.y > lastClickedCell.y){
					for (int i = 0; i < ships[currentShip]; i++){
						shipHighlights.push_back({ currentClickedCell.x, lastClickedCell.y + i });
						clickLocation.x = -10;
						clickLocation.y = -10;
						refreshShipHighlights = true;
					}
				}
				if (currentClickedCell.y == lastClickedCell.y && currentClickedCell.x > lastClickedCell.x){
					for (int i = 0; i < ships[currentShip]; i++){
						shipHighlights.push_back({ lastClickedCell.x + i, currentClickedCell.y });
						clickLocation.x = -10;
						clickLocation.y = -10;
						refreshShipHighlights = true;
					}
				}
				initialShip = false;
			}
		}
	}
	if (gameState == 3){
		if (mPath.contains(clickLocation)){
			currentClickedCell.x = x;
			currentClickedCell.y = y;
			made_shot = false;
		}
	}
	vector<Shot> shots = player->getShots();
	bool shot = false;
	bool hit = false;
	for (int i = 0; i < shots.size(); i++){
		if (shots[i].first.first == x && shots[i].first.second == y){
			shot = true;
			if(shots[i].second){
				hit = true;
			}
			else
				hit = false;
		}
	}
	bool shipLight = false;
	for (int i = 0; i < shipHighlights.size(); i++){
		if (shipHighlights[i].x == x && shipHighlights[i].y == y){
			shipLight = true;
		}
	}
	if (gameState == 3){
		if (shot){
			Color resultColor;
			if (hit){
				resultColor = Color(1.0, 0.0, 0.0);
			}
			else{
				resultColor = Color(0.2, 0.2, 0.2);
			}
			gl::color(resultColor);
			gl::pushModelMatrix();
			gl::translate(current_pos_x - 1, current_pos_y - 1);
			gl::drawSolidRect(Rectf(0, CELL_SIZE, CELL_SIZE, 0));
			gl::color(highlight);
			gl::popModelMatrix();
		}
	}
	if (gameState == 1){
		if (shipLight){
			gl::color(Color(0.8, 1.0, 0.8));
			gl::pushModelMatrix();
			gl::translate(current_pos_x - 1, current_pos_y - 1);
			gl::drawSolidRect(Rectf(0, CELL_SIZE, CELL_SIZE, 0));
			gl::color(highlight);
			gl::popModelMatrix();
		}
	}
	if (mPath.contains(clickLocation)){
		Color resultColor = Color(1.0, 0.0, 0.0);
		gl::color(resultColor);
		gl::pushModelMatrix();
		gl::translate(current_pos_x - 1, current_pos_y - 1);
		gl::drawSolidRect(Rectf(0, CELL_SIZE, CELL_SIZE, 0));
		gl::color(highlight);
		gl::popModelMatrix();
	}
	if (mPath.contains(movePoint)){
		gl::pushModelMatrix();
		gl::translate(current_pos_x, current_pos_y);
		gl::drawSolidRect(Rectf(0, CELL_SIZE, CELL_SIZE, 0));
		gl::popModelMatrix();
	}
	gl::color(highlight);
	gl::draw(mPath);
}

//Battleship::button
//draws and manages the button object
//in Gamestate 1: places ship in area defined by cell function, cycles through the ship array, and verifies that ships do not overlap or cross borders.
//in Gamestate 3: fires selected shot and checks whether hit or miss
//in all gamestates: draws the button path to define mouseclick area for button
void BattleshipApp::button(Board* player, Board* enemy){
	Path2d nPath;
	float starter_pos_x = getWindowWidth() / 3 * 2 + CELL_SIZE;
	float starter_pos_y = getWindowHeight() / 5;
	float buttonSize;
	if (gameState == 1)
		buttonSize = CELL_SIZE * 3.5;
	if (gameState == 3)
		buttonSize = CELL_SIZE * 2.8;
	nPath.moveTo(starter_pos_x, starter_pos_y);
	nPath.lineTo(buttonSize + starter_pos_x, starter_pos_y);
	nPath.lineTo(buttonSize + starter_pos_x, starter_pos_y + CELL_SIZE);
	nPath.lineTo(starter_pos_x, CELL_SIZE + starter_pos_y);
	nPath.close();
	gl::pushModelMatrix();
	gl::translate(starter_pos_x, starter_pos_y);
	gl::color(0.0f, 0.1f, 0.1f);
	gl::drawSolidRect(Rectf(0, CELL_SIZE+CELL_PAD, buttonSize+CELL_PAD, 0));
	gl::color(0.0f, 0.3f, 0.3f);
	gl::drawSolidRect(Rectf(0, CELL_SIZE, buttonSize, 0));
	gl::popModelMatrix();
	gl::color(0.8f, 1.0f, 0.8f);
	gl::draw(nPath);
	if (nPath.contains(movePoint)){
		gl::pushModelMatrix();
		gl::translate(starter_pos_x, starter_pos_y);
		gl::drawSolidRect(Rectf(0, CELL_SIZE, buttonSize, 0));
		gl::popModelMatrix();
	}
	if (nPath.contains(clickLocation)){
		if (gameState == 1){
			bool success;
			if (lastClickedCell.x == currentClickedCell.x){
				success = player->placeShip(lastClickedCell.x, lastClickedCell.y, 0, ships[currentShip]);
			}
			if (lastClickedCell.y == currentClickedCell.y){
				success = player->placeShip(lastClickedCell.x, lastClickedCell.y, 1, ships[currentShip]);
			}
			shipHighlights.clear();
			if (success){
				currentShip++;
			}
			clickLocation.x = -10;
			clickLocation.y = -10;
			lastClickedCell.x = -10;
			lastClickedCell.y = -10;
		}
		if(gameState == 3){
			shot_result = enemy->makeShot(currentClickedCell.x, currentClickedCell.y);
			if (shot_result != -1)
				made_shot = true;
			endTurn = true;
			clickLocation.x = -10;
			clickLocation.y = -10;
		}
		else{
			clickLocation.x = -10;
			clickLocation.y = -10;
		}
	}
	string buttonText;
	if (gameState == 1)
		buttonText = "DONE";
	if (gameState == 3)
		buttonText = "FIRE";
	TextBox tbox = TextBox().alignment(TextBox::LEFT).font(mFont).size(buttonSize, TextBox::GROW).text(buttonText);
	mTextTexture = gl::Texture2d::create(tbox.render());
	Color textColor = nPath.contains(movePoint) ? Color(0.0, 0.3, 0.3) : Color(0.8, 1.0, 0.8);
	gl::color(textColor);
	gl::pushModelMatrix();
	gl::translate(starter_pos_x+15, starter_pos_y+2);
	gl::draw(mTextTexture);
	gl::popModelMatrix();

}
//BattleshipApp::miniCell
//draws a smaller grid showing current players ships and where enemy shots landed
void BattleshipApp::miniCell(int x, int y, Board* player){
	float miniCellSize = CELL_SIZE / 3;
	int current_pos_x = getWindowWidth() / 3 * 2 + CELL_SIZE + miniCellSize * x;
	int current_pos_y = getWindowHeight() / 3 + miniCellSize * y;
	Path2d mPath;
	mPath.moveTo(current_pos_x, current_pos_y);
	mPath.lineTo(miniCellSize + current_pos_x, current_pos_y);
	mPath.lineTo(miniCellSize + current_pos_x, miniCellSize + current_pos_y);
	mPath.lineTo(current_pos_x, miniCellSize + current_pos_y);
	mPath.close();
	vector<Shot> shots = player->getShots();
	bool shot = false;
	bool hit = false;
	for (int i = 0; i < shots.size(); i++){
		if (shots[i].first.first == x && shots[i].first.second == y){
			shot = true;
			if (shots[i].second){
				hit = true;
			}
			else
				hit = false;
		}
	}
	Color cellColor = (player->checkSpace(x, y)) ? Color(0.8, 1.0, 0.8) : Color(0.0, 0.3, 0.3);
	if (shot)
		cellColor = Color(0.2, 0.2, 0.2);
	if (hit)
		cellColor = Color(1.0, 0.0, 0.0);
	gl::color(cellColor);
	gl::pushModelMatrix();
	gl::translate(current_pos_x, current_pos_y);
	gl::drawSolidRect(Rectf(0, miniCellSize, miniCellSize , 0));
	gl::popModelMatrix();
	gl::color(0.8, 1.0, 0.8);
	gl::draw(mPath);
}

//BattleshipApp::drawBoard
//controller draw function that calls above functions to populate the game board
void BattleshipApp::drawBoard(int playerNum){
	Board* player;
	Board* enemy;
	if (playerNum == 1){
		player = &player1;
		enemy = &player2;
	}
	else{
		player = &player2;
		enemy = &player1;
	}
	int background_size = (CELL_SIZE + CELL_PAD) * player->getX() + CELL_PAD;
	gl::color(0.0f, 0.1f, 0.1f);
	gl::drawSolidRect(Rectf(0, background_size, background_size+CELL_PAD, 0));
	gl::color(0.0f, 0.3f, 0.3f);
	gl::drawSolidRect(Rectf(0, background_size, background_size, 0));
	gl::color(0.8f, 1.0f, 0.8f);
	for (int y = 0; y < player->getY(); y++){
		for (int x = 0; x < player->getX(); x++){
			cell(x, y, enemy);
			miniCell(x, y, player);
		}
	}
	button(player, enemy);
}

//BattleshipApp::drawEnd
//draws the end game screen
void BattleshipApp::drawEnd(string in){
	gl::clear(Color(0.2, 0.2, 0.2));
}
//BattleshipApp:drawTurnScreen
//draws a blank screen to allow passing without opponent seeing game board. Contains instructions on how to start next turn
void BattleshipApp::drawTurnScreen(string in){
	gl::clear(Color(0.2, 0.2, 0.2));
	if (pressSpace){
		endTurn = false;
		if (turn == 1){
			turn = 2;
		}
		else{
			turn = 1;
		}
	}
	TextBox tbox = TextBox().alignment(TextBox::LEFT).font(mFont).size(500, TextBox::GROW).text(in);
	mTextTexture = gl::Texture2d::create(tbox.render());
	gl::color(Color(0.8,1.0,0.8));
	gl::pushModelMatrix();
	gl::translate(getWindowWidth()/10, getWindowHeight()/3 );
	gl::draw(mTextTexture);
	gl::popModelMatrix();
}

//BattleshipApp::setup
//initializes various variables for the start of the game
void BattleshipApp::setup()
{
	made_shot = false;
	pressSpace = false;
	endTurn = false;
	initialShip = false;
	refreshShipHighlights = false;
	beginVictoryCheck = false;
	turn = 1;
	gameState = 1;
	ships = { 2, 3, 3, 4, 5 };
	currentShip = 0;
	mFont = Font("Arial-BoldMT", 56);
	player1 = Board(10, 10, "Player1");
	player2 = Board(10, 10, "Player2");
	CELL_SIZE = 50;
	CELL_PAD = 5;
	
}
//BattleshipApp::mouseDown
//manages mouseclick events
void BattleshipApp::mouseDown(MouseEvent event){
	if (leftClick){
		if (!event.isLeftDown()){
			clickLocation = event.getPos();
		}
	}
}
//BattleshipAp::mouseMove
//manages mouse move events
void BattleshipApp::mouseMove( MouseEvent event ){
	movePoint = event.getPos();
}
void BattleshipApp::keyDown(KeyEvent event)
{
	if (event.getCode() == KeyEvent::KEY_ESCAPE) {
		quit();
	}
	if (event.getChar() == 'n'){
		pressSpace = true;
	}
}

void BattleshipApp::update()
{
}

//BattleshipApp::draw()
//main draw function. controls game state switches
void BattleshipApp::draw()
{
	setWindowSize(((CELL_SIZE + CELL_PAD)*player1.getX()+CELL_PAD)*1.5, (CELL_SIZE + CELL_PAD)*player1.getY()+CELL_PAD);
	gl::clear( Color( 0.2, 0.2, 0.2 ) ); 
	gl::pushModelMatrix();
	if (beginVictoryCheck){
		player1.filterShips();
		player2.filterShips();
	}
	if (currentShip >= ships.size() && turn != 2){
		turn = 2;
		currentShip = 0;
	}
	if (currentShip >= ships.size() && turn == 2){
		gameState = 3;
		beginVictoryCheck = true;
		endTurn = true;
	}
	if (player1.lost() && beginVictoryCheck){
		drawEnd("Player 2 wins!");
	}
	else if (player2.lost() && beginVictoryCheck){
		drawEnd("Player 1 wins!");
	}
	else if (endTurn){
		drawTurnScreen("Turn end! Pass to your opponent. Press n to continue.");
	}
	else{
		drawBoard(turn);
	}
	gl::color(1.0f, 0.5f, 0.25f);
	gl::popModelMatrix();
	pressSpace = false;
}

CINDER_APP( BattleshipApp, RendererGl )
