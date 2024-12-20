#include "Planet.h"
#include <sgg/graphics.h>

#include "GameState.h"

#include "setcolor.h"
#include "Bullets.h"
#include "Level.h"
#include <cmath>
#include "util.h"
#include <ctime>
#include <string>
using namespace std;


void Planet::loadExplosionSprites() {
	for (int i = 1; i < 8; i++) {
		sprites_explosion.push_back(m_state->getFullAssetPath("explosion//explosion" +to_string(i)+ ".png"));
		sprites_explosion.push_back(m_state->getFullAssetPath("explosion//explosion" + to_string(i) + ".png"));
	}
}

float Planet::calcFireAngle(float player_m_pos_x, float player_m_pos_y)
{
	float Vx = player_m_pos_x - m_pos_x;
	float Vy = player_m_pos_y - m_pos_y;
	
	
	return degrees(atan2(Vy, Vx));
	
	
}

void Planet::setlevel(int i) {
	level = i;
}
int Planet::getlevel() {
	return level;
}

void Planet::isDead()
{
	if (returnHP() == 0) {
		delete this;
	}

}

void Planet::fire(float player_m_pos_x , float player_m_pos_y)
{
	if (!flag) {
		m_state->getLevel()->Pshoot(new Bullet(m_pos_x, m_pos_y, -calcFireAngle(player_m_pos_x, player_m_pos_y)));

		lastFramePlanetFire = m_state->framecounter;
		flag = true;
		return;

	}
	if (lastFramePlanetFire + 100  > m_state->framecounter){
		return;
	}
	m_state->getLevel()->Pshoot(new Bullet(m_pos_x , m_pos_y , -calcFireAngle(player_m_pos_x , player_m_pos_y )));

	lastFramePlanetFire = m_state->framecounter;
}

void Planet::update(float dt)
{
	//isDead();
 
}

void Planet::init()
{
	//init private values
	sprite_counter = 0;
	angle = 0;
	lastFramePlanetFire = 0;
	flag = false;
	hp = 5;

	// init public values;
	explosion = false;


	e = new Explosion(m_pos_x, m_pos_y);
	e->init();




	//m_pos_x = 5.0f;
	//m_pos_y = 5.0f;
	m_width = 3.0f;
	m_height = 3.0f;
	//m_height /= 7.0f;
	SETCOLOR(m_planet_brush.fill_color, 1.0f, 1.0f, 1.0f);
	m_planet_brush.fill_opacity = 1.0f;
	m_planet_brush.fill_secondary_opacity = 0.0f;
	m_planet_brush.outline_opacity = 1.0f;
	
	loadExplosionSprites();




	//m_planet_brush.texture = m_state->getFullAssetPath("3342944734.png");
}

void Planet::draw()
{ 
	
	int i = returnHP();
	m_planet_brush.outline_opacity = 0.0f;
	m_planet_healthbar.outline_opacity = 0.0f;

	m_planet_healthbar.texture = m_state->getFullAssetPath("healthbar//healthbar" + std::to_string(returnHP()) + ".png");

	
	
	
	
	
	
	
	
	//m_planet_brush.texture = m_state->getFullAssetPath("planet2.png");
	//graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 4.0f, 4.0f, m_planet_brush);
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x + 0.02f, m_pos_y + m_state->m_global_offset_y - 2.5f, 3.0f, 0.2f, m_planet_healthbar);


	//SETCOLOR(m_planet_healthbar.fill_color ,50, 205, 50)


	
	if (level == 1) {
		graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 3.0f, 3.0f, m_planet_brush);
	}
	if (level == 2)
	{
		graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 3.0f, 3.0f, m_planet_brush);
	}
	if (level == 3) {
		graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 3.0f, 3.0f, m_planet_brush);
	}
	



	m_planet_brush.texture = m_state->getFullAssetPath("planet" + std::to_string(level) + "motion//tile00" + std::to_string(motion) + ".png");
	if (motion >= 10) {
		m_planet_brush.texture = m_state->getFullAssetPath("planet" + std::to_string(level) + "motion//tile0" + std::to_string(motion) + ".png");

	}
	counter++;
	if (counter > 4) // counter gia to poses fores tha emfanizete h kathe eikona 
	{
		motion++;
		if (motion > 49)
		{
			motion = 0;

		}
		counter = 0;
	}

	if (explosion) {
		e->setXY(m_pos_x, m_pos_y); //explotion is implemented with the global offset on draw saw player gives his potition when he has to draw an e object on him

		if (e->sprite_counter > 14) {
			explosion = false;
			e->sprite_counter = 1;



		}
		else {
			e->draw();
		}








	}


	
	
		
	

	

}

void Planet::setoid(int oid)
{
	o_id = oid;
}

int Planet::getoid()
{
	return o_id;
}

Planet::~Planet()
{
	if (e) {
		delete e;
	}
}




