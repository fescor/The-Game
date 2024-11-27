#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>
#include "GameState.h"

class Explosion : public GameObject, public Box
{
	graphics::Brush m_brush_explosion;
	//const float x = m_state->getCanvasWidth() / 2.0f ;
	//const float y = m_state->getCanvasHeight() / 2.0f - 0.5f;
	
	std::vector<std::string> sprites;





public:
	
	int sprite_counter;
	Explosion(float x, float y);
	void setXY(float x, float y);
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	






};