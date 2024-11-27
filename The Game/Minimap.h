#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <vector>






class Minimap : public GameObject   , public  Box{



	graphics::Brush m_brush_minimap;
	graphics::Brush m_brush_minimapP;
	std::vector<class Planet*> planets;
	std::vector<float> distances;
	std::vector<float> angles;


public:
	
	void calculatevector(float planet_x , float planet_y);
	Minimap();
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	~Minimap();






};