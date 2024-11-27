#include "Background.h"

void Background::update(float dt)
{
}

float Background::getWidth() {
	return width;
}

float Background::getHeight() {
	return height;
}

void Background::init()
{
	m_pos_x = m_state->getCanvasWidth() / 2.0f;
	m_pos_y = m_state->getCanvasHeight() / 2.0f;
	width = m_state->getCanvasWidth() *4.0f;
	height = m_state->getCanvasHeight() * 4.0f;
	
	m_brush_background.fill_opacity = 1.0f;
	m_brush_background.texture = m_state->getFullAssetPath("SpaceBackground2.png");

}

void Background::draw()
{
	
	graphics::drawRect(m_state->m_global_offset_x/ 3  + m_pos_x, m_state->m_global_offset_y /3 + m_pos_y, width, height, m_brush_background);


}
