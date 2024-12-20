
#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>
#include "Explosion.h"
#include "Health.h"

class Planet : public GameObject , public Box , public Health
{
	graphics::Brush m_planet_brush;
	graphics::Brush m_explosion_brush;
	graphics::Brush m_planet_healthbar;
	std::vector<std::string> sprites_explosion;
	Explosion* e;

	int level;

	int motion = 0;
	int counter = 0;
	int sprite_counter;
	float angle;
	int lastFramePlanetFire ;
	bool flag ;
	int o_id; // online id of planet

	
	void loadExplosionSprites();
	float calcFireAngle(float player_m_pos_x, float player_m_pos_y);
	
	
public:
	void isDead();
	int getlevel();
	void setlevel(int i);
	void fire(float player_m_pos_x , float player_m_pos_y );
	Planet(std::string name) : GameObject(name) {}
	bool explosion;

	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	void setoid(int oid);
	int getoid();
	
	~Planet();
	


};

	