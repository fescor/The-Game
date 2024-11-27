#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"



class Bullet : public GameObject, public Box
{
	graphics::Brush m_bullet_brush;
	//const float x = m_state->getCanvasWidth() / 2.0f ;
	//const float y = m_state->getCanvasHeight() / 2.0f - 0.5f;
	float angle ;
	



public:
	bool outOfBounds();
	Bullet(float x, float y, float angle);
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	~Bullet();





	
};
