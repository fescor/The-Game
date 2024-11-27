#pragma once
#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"






class Asteroid :   public GameObject, public Box
{
	graphics::Brush m_brush_asteroid;
	float radius ;
	float angle ;
	float center_x;
	float center_y;
	int asteroid_id = 0;

	int counter = 0;

public:
	Asteroid(float x , float y);
	


	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;

	~Asteroid();



};