#include "cinder/app/App.h"
#include "cinder/app/Window.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "battleship_core.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class BattleshipApp : public App {
	public:
		void setup() override;
		void keyDown(app::KeyEvent event);
		void mouseDown(MouseEvent event);
		void mouseMove(MouseEvent event);
		void update() override;
		void draw() override;
		void drawBoard();
		void cell(int, int);
		void button();
	private:
		bool leftClick;
		vec2 clickLocation;
		vec2 last_clicked_cell;
		int grid_x, grid_y;
		int shot_result;
		bool made_shot;
		vec2 movePoint;
		Board player1;
		Board player2;
		Path2d mPath;
		Font mFont;
		gl::TextureRef mTextTexture;
		int CELL_SIZE;
		int CELL_PAD;

};
void BattleshipApp::cell(int x, int y){
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
	if (mPath.contains(clickLocation)){
		grid_x = x;
		grid_y = y;
		last_clicked_cell = clickLocation;
		made_shot = false;
	}
	if (mPath.contains(last_clicked_cell)){
		Color resultColor;
		if (made_shot){
			if (shot_result == 0)
				resultColor = Color(0.2, 0.2, 0.2);
			else if (shot_result == 1)
				resultColor = Color(0.0, 1.0, 0.0);
			else if (shot_result == -1)
				resultColor = Color(0.0, 0.0, 1.0);
			else if (shot_result == -2)
				resultColor = Color(0.0, 1.0, 1.0);
			else
				resultColor = Color(1.0, 1.0, 1.0);
		}
		else
			resultColor = Color(1.0, 0.0, 0.0);
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

void BattleshipApp::button(){
	Path2d nPath;
	float starter_pos_x = getWindowWidth() / 3 * 2 + CELL_SIZE;
	nPath.moveTo(starter_pos_x, CELL_PAD);
	nPath.lineTo(CELL_SIZE * 3.5 + starter_pos_x, CELL_PAD);
	nPath.lineTo(CELL_SIZE * 3.5 + starter_pos_x, CELL_PAD + CELL_SIZE);
	nPath.lineTo(starter_pos_x, CELL_PAD + CELL_SIZE);
	nPath.close();
	gl::pushModelMatrix();
	gl::translate(starter_pos_x, CELL_PAD);
	gl::color(0.0f, 0.1f, 0.1f);
	gl::drawSolidRect(Rectf(0, CELL_SIZE+CELL_PAD, CELL_SIZE*3.5+CELL_PAD, 0));
	gl::color(0.0f, 0.3f, 0.3f);
	gl::drawSolidRect(Rectf(0, CELL_SIZE, CELL_SIZE*3.5, 0));
	gl::popModelMatrix();
	gl::color(0.8f, 1.0f, 0.8f);
	gl::draw(nPath);
	if (nPath.contains(movePoint)){
		gl::pushModelMatrix();
		gl::translate(starter_pos_x, CELL_PAD);
		gl::drawSolidRect(Rectf(0, CELL_SIZE, CELL_SIZE*3.5, 0));
		gl::popModelMatrix();
	}
	if (nPath.contains(clickLocation)){
		shot_result = player1.makeShot(grid_x, grid_y);
		made_shot = true;
		clickLocation.x = 0;
		clickLocation.y = 0;
	}
	string buttonText = "FIRE";
	TextBox tbox = TextBox().alignment(TextBox::LEFT).font(mFont).size(CELL_SIZE*3.5, TextBox::GROW).text(buttonText);
	mTextTexture = gl::Texture2d::create(tbox.render());
	Color textColor = nPath.contains(movePoint) ? Color(0.0, 0.3, 0.3) : Color(0.8, 1.0, 0.8);
	gl::color(textColor);
	gl::pushModelMatrix();
	gl::translate(starter_pos_x+15, CELL_PAD-3);
	gl::draw(mTextTexture);
	gl::popModelMatrix();

}

void BattleshipApp::drawBoard(){
	int background_size = (CELL_SIZE + CELL_PAD) * player1.getX() + CELL_PAD;
	gl::color(0.0f, 0.1f, 0.1f);
	gl::drawSolidRect(Rectf(0, background_size, background_size+CELL_PAD, 0));
	gl::color(0.0f, 0.3f, 0.3f);
	gl::drawSolidRect(Rectf(0, background_size, background_size, 0));
	button();
	gl::color(0.8f, 1.0f, 0.8f);
	for (int y = 0; y < player1.getY(); y++){
		for (int x = 0; x < player1.getX(); x++){
			cell(x, y);
		}
	}
	button();
}
void BattleshipApp::setup()
{
	made_shot = false;
	mFont = Font("Arial-BoldMT", 72);
	player1 = Board(10, 10, "Player1");
	player2 = Board(10, 10, "Player2");
	player1.placeShip(0, 0, 0, 2);
	CELL_SIZE = 50;
	CELL_PAD = 5;
	
}
void BattleshipApp::mouseDown(MouseEvent event){
	if (leftClick){
		if (!event.isLeftDown()){
			clickLocation = event.getPos();
		}
	}
}
void BattleshipApp::mouseMove( MouseEvent event ){
	movePoint = event.getPos();
}
void BattleshipApp::keyDown(KeyEvent event)
{
	if (event.getCode() == KeyEvent::KEY_ESCAPE) {
		quit();
	}
}

void BattleshipApp::update()
{
}

void BattleshipApp::draw()
{
	setWindowSize(((CELL_SIZE + CELL_PAD)*player1.getX()+CELL_PAD)*1.5, (CELL_SIZE + CELL_PAD)*player1.getY()+CELL_PAD);
	gl::clear( Color( 0.2, 0.2, 0.2 ) ); 
	gl::pushModelMatrix();
	drawBoard();
	gl::color(1.0f, 0.5f, 0.25f);
	gl::popModelMatrix();
}

CINDER_APP( BattleshipApp, RendererGl )
