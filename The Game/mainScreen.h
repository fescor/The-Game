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
	graphics::Brush m_lobby_gui;
	graphics::Brush test;
	graphics::MouseState mouse;
	float mouse_x = 0.f;
	float mouse_y = 0.f;

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

enum HOVER {
	PLAY,
	CDIFICULTY,
	CSPACESHIP,
	CONTROLS,
	ONLINE,
	CD_EASY,
	CD_HARD,
	CD_AREUSURE,
	CD_SURVAVAL,
	SELECT_SPACESHIP,
	SELECT_CONTROLS,
	CREATE_LOBBY,
	JOIN_LOBBY,
	DC,
	SEND_BROADCAST,
	LOBBY_SCREEN,








};



