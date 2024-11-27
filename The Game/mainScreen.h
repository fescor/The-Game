#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>

#include "Background.h"

#include <chrono>




class MainScreen : public GameObject {
	graphics::Brush m_main_background;
	graphics::Brush m_main_text;
	graphics::Brush m_spaceship;
	graphics::Brush m_keys;
	int selector;
	Background* m_background;
	int framecounter1 = 0;

	bool flag1 = false;

	int spaceshipSelector = 0;
	float timecounter = 0;


	
	void drawSpaceShip(int i );
	void hover();
	void moveup();
	void movedown();
	void moveleft();
	void moveright();
	
	void select();


public:
	int getSelector();
	void setSelector(int s);
	MainScreen();
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;







};



