#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>


class Tokens : public GameObject, public Box
{
	graphics::Brush m_brush_token;
	char type;
	

public:
	Tokens(char t , float x , float y );


	char getType();
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;





};
