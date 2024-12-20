#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>


class Tokens : public GameObject, public Box
{
	graphics::Brush m_brush_token;
	char type;
	int o_id; //online id of token
	

public:
	Tokens(char t , float x , float y , int o_id);


	char getType();
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	int getoid();





};
