#include "Minimap.h"
#include "GameState.h"
#include "Player.h"
#include "util.h"
#include "Level.h"
#include <cmath>




void Minimap::calculatevector(float planet_x , float planet_y) // ypologizw to mikos kai tin gonia tou dianismatos pexti-planiti
{

	float distance = powf(powf((m_state->getPlayer()->getX() - planet_x), 2) + powf((m_state->getPlayer()->getY() - planet_y), 2), 0.5);

	float angle = degrees(atan2( planet_y - m_state->getPlayer()->getY(),  planet_x -m_state->getPlayer()->getX()));

	distances.push_back(distance);
	angles.push_back(angle);

		

}

Minimap::Minimap()
{


}

void Minimap::update(float dt)
{

	
	//for (auto planet : m_state->getLevel()->m_planets) {
	for(auto iter = m_state->getLevel()->m_planets.begin(); iter != m_state->getLevel()->m_planets.end(); iter++){
		//if (m_state->getPlayer()->inMinimapRange(*planet, m_state->getCanvasWidth(), m_state->getCanvasHeight())) {// amma einai mesa sto range emfanishs planiti sto minimap ypologizw dianisma
		if (m_state->getPlayer()->inMinimapRange(*iter->second, m_state->getCanvasWidth(), m_state->getCanvasHeight())) {
			calculatevector(iter->second->m_pos_x, iter->second->m_pos_y);// not iter->first-> but planet->
			
			planets.push_back(iter->second);
		}


	}
	

}

void Minimap::init()
{
}

void Minimap::draw()
{

	m_brush_minimap.texture = m_state->getFullAssetPath("minimap.png");
	m_brush_minimap.outline_opacity = 0.0f;
	graphics::drawRect(m_state->getCanvasWidth() -3.5f, m_state->getCanvasHeight() - 3.5f, 7.0f , 7.0f, m_brush_minimap);
	m_brush_minimap.texture = m_state->getFullAssetPath("minimapPlyaer.png");
	graphics::setOrientation(m_state->getPlayer()->getAngle());
	graphics::drawRect(m_state->getCanvasWidth() - 3.5f, m_state->getCanvasHeight() - 3.5f, 0.3f, 0.3f, m_brush_minimap);
	graphics::resetPose();

	
	for (int i = 0; i < distances.size(); i++) {
		

			float x = m_state->getCanvasWidth() - 3.5f + 0.05 * distances[i] * cos(radians(angles[i])); // mathimatika
			float y = m_state->getCanvasHeight() - 3.5f + 0.05 * distances[i] * sin(radians(angles[i]));

			if (0.05*distances[i] <= 3.5) {// amma h apostash einai mikroteri apo tin aktina tou kiklou 
				graphics::drawRect(x, y, 0.1, 0.1, m_brush_minimapP);
			}
			

		
		
	}
	planets.clear(); // se kathe frame thelw na ksanatsekarei ti einai mesa sto range tou minimap opote katharizw ta vectors(not the best optimise option)
	distances.clear();
	angles.clear();
		

}

Minimap::~Minimap()
{
	for (auto p : planets) {
		if (p) {
			delete p;
		}
	}
}


