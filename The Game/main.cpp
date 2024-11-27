#include "sgg/graphics.h"
#include "sgg/scancodes.h"
#include "GameState.h"
#include <stdio.h>
#include "setcolor.h"
#include <string>
#include "cereal/archives/portable_binary.hpp"
#include "Structs.h"



void init() {

	/*
	graphics::preloadBitmaps("assets");
	graphics::preloadBitmaps("assets//planet1motion");
	graphics::preloadBitmaps("assets//planet2motion");
	graphics::preloadBitmaps("assets//planet3motion");
	graphics::preloadBitmaps("assets//asteroids");
	graphics::preloadBitmaps("assets//healthbar");
	*/
	GameState::getInstance()->init();
	


}



void draw() {

	
		GameState::getInstance()->draw();
		
	
}


void update(float dt){

	GameState::getInstance()->update(dt);





}

int main(int argc, char** argv) {















}


int main(int argc, char** argv)
{

	
	
	graphics::createWindow(800, 800, "IsWindow");

	
	init();


	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);
	graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), GameState::getInstance()->getCanvasHeight());
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
	//graphics::playMusic("assets//backgroundmain.mp3", 1.0f, true, 0);
	
	graphics::startMessageLoop(); // creates a loop so thw window stay open


	return 0;

}
