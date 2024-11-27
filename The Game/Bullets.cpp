#include "Bullets.h"

#include "Player.h"
#include "setcolor.h"
#include "util.h"
#include "Level.h"
#include "GameState.h"


bool Bullet::outOfBounds()
{
	if (m_pos_x  > m_state->getLevel()->getBackground()->getWidth() * 1.75 - m_state->getCanvasWidth() / 2) { // deksia an thes na pigenei mexri gonia o pextis /2 anti *2

		return true;
	}
	if (m_pos_x  < -m_state->getLevel()->getBackground()->getWidth() * 1.5  + m_state->getCanvasWidth() / 2) {

		return true;
	}
	if (m_pos_y  > m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() / 2) {//kato 

		return true;
	}
	if (m_pos_y < -m_state->getLevel()->getBackground()->getHeight() * 1.5 + m_state->getCanvasHeight() / 2) {

		return true;
	}
	return false;
	
}

Bullet::Bullet(float x, float y, float angle)
{
	m_pos_x = x;
	m_pos_y = y;
	this->angle = angle;
	
}

void Bullet::update(float dt)
{
	float delta_time = dt / 2000.0f;
	const float v = 80.0f;

	m_pos_y -= sin(radians(angle)) * (v * delta_time);
	m_pos_x += cos(radians(angle)) * (v * delta_time);
	
		
	
	


}

void Bullet::init()
{
	
	m_width = 0.80f;
	m_height = 0.80f;
	SETCOLOR(m_bullet_brush.fill_color, 1.0f, 1.0f, 1.0f);
	m_bullet_brush.fill_secondary_opacity = 0.0f;
	m_bullet_brush.texture = m_state->getFullAssetPath("bullet.png");


}

void Bullet::draw()
{
	m_bullet_brush.outline_opacity = 0.0f;
	graphics::setOrientation(angle + 90.0f);
	graphics::drawRect(  m_pos_x + m_state->m_global_offset_x ,   m_pos_y - 0.5 + m_state->m_global_offset_y, 2.0f , 2.0f , m_bullet_brush);
	graphics::resetPose();
}


Bullet::~Bullet()
{
	
}


