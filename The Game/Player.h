#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>

#include "Health.h"
#include "Explosion.h"
#include "Minimap.h"


class Player : public GameObject, public Box , public Health
{
	graphics::Brush m_brush_shield;
	graphics::Brush m_explosion_brush;
	graphics::Brush m_brush_player;
	int  m_bullets ;
	graphics::Brush m_bullet_brush;
	Explosion* e;
	bool flag;
	void fire();
	void loadExplosionSprites();
	const float velocity = 2.0f*12.0f;
	float angle;
	float speed ;
	int sprite_counter ;
	
	int shieldFramecounter ;


	std::vector<std::string> sprites_explosion;

	////////NET VAL ////////
	int o_id = 0;


protected:
	
public:
	
	bool shield;
	bool explosion;
	void Activateshield();
	Player(std::string name) : GameObject(name){}
	bool flag2;
	float getAngle();
	float getX();
	float getY();
	std::string returnBulletsCount();
	void changeBulletCount(int v);
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	void seto_id(int i);
	int* geto_id();
	Player(int online_id);
	~Player();
};