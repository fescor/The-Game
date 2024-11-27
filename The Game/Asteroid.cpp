#include "Asteroid.h"
#include <sgg/graphics.h>
#include <enet/enet.h>


#include "GameState.h"

#include "util.h"


#include "setcolor.h"
using namespace std;





Asteroid::Asteroid(float x, float y)
{
	center_x = x;
	center_y = y;
}

void Asteroid::update(float dt)
{
	float delta_time = dt / 1000.0f;
	const float v = 12.0f;
	angle += v*delta_time;

	m_pos_x = center_x +  radius * cos(radians(angle));
	m_pos_y = center_y + radius * sin(radians(angle));

	

}

void Asteroid::init()
{
	int lb = 0;
	int ub = 15;
	//init private values
	
	radius = 5.0f + rand()%(ub - lb + 1);
	

	

	

	//center_x = 0;
	//center_y = 0;
	
	m_pos_x = center_x + radius * cos(radians(angle));
	m_pos_y = center_y + radius * sin(radians(angle));
	
	m_brush_asteroid.fill_opacity = 1.0f;
	m_brush_asteroid.fill_secondary_opacity = 0.0f;
	m_brush_asteroid.outline_opacity = 1.0f;





	m_brush_asteroid.texture = m_state->getFullAssetPath("asteroid.png");


}

void Asteroid::draw()
{
	
	m_brush_asteroid.outline_opacity = 0.0f;
	//graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.0f , 1.0f, m_brush_asteroid);
	m_brush_asteroid.texture = m_state->getFullAssetPath("//asteroids//asteroids" + to_string(asteroid_id) + ".png");
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.5f, 1.5f, m_brush_asteroid);
	
	counter++;
	if (counter > 2) // counter gia to poses fores tha emfanizete h kathe eikona 
	{
		asteroid_id++;
		if (asteroid_id > 19)
		{
			asteroid_id = 0;

		}
		counter = 0;
	}

	
}

Asteroid::~Asteroid()
{
	
}


