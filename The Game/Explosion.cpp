#include "Explosion.h"
#include <string>
using namespace std;

Explosion::Explosion( float x, float y)
{
	m_pos_x = x;
	m_pos_y = y;


}

void Explosion::setXY(float x, float y)
{
	m_pos_x = x;
	m_pos_y = y;
	
}

void Explosion::update(float dt)
{

}

void Explosion::init()
{
	sprite_counter = 1;
	for (int i = 1; i < 8; i++) {
		sprites.push_back("explosion//explosion" + to_string(i) + ".png");
		sprites.push_back("explosion//explosion" + to_string(i) + ".png");
	}
	
}

void Explosion::draw()
{
	m_brush_explosion.outline_opacity = 0.0f;
	m_brush_explosion.texture = m_state->getFullAssetPath(sprites[sprite_counter - 1]);
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 3.0f, 3.0f, m_brush_explosion);
	sprite_counter++;
	
}
