#include "Tokens.h"
#include "setcolor.h"


Tokens::Tokens(char t, float x , float y)
{
	this->type = t;
	m_pos_x = x;
	m_pos_y = y;
}

char Tokens::getType()
{
	return type;
}

void Tokens::update(float dt)
{

}

void Tokens::init()
{

	switch (type) {
	case 'B':
		m_brush_token.texture = m_state->getFullAssetPath("ammo.png");

		break;
	case 'H':
		m_brush_token.texture = m_state->getFullAssetPath("hp.png");

		break;
	case 'S':
		m_brush_token.texture = m_state->getFullAssetPath("shield.png");
		break;

	}


}

void Tokens::draw()
{
	m_brush_token.outline_opacity = 0.0f;
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brush_token);


}
