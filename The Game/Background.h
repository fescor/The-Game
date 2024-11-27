#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"




class Background : public GameObject, public Box
{
	graphics::Brush m_brush_background;
	float width ;
	float height;

public:
	float getWidth();
	float getHeight();

 

	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;





};




