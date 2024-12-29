#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <string>
#include "Health.h"
#include "Explosion.h"
#include "Minimap.h"
#include "Structs.h"
#include <ppl.h>
#include <concurrent_queue.h>


class Player : public GameObject, public Box , public Health
{
	const float tickrate = 50.0f;
	float timeStepCounter = .0f;
	float lastPacket_timeSend = .0;

	bool isAngleIdle = true;
	int testcounter = 0;
	graphics::Brush test;
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
	float PreviousFrameAngle;
	float speed ;
	int sprite_counter ;
	
	int shieldFramecounter ;
	

	std::vector<std::string> sprites_explosion;

	////////NET VAL////////
	int o_id = 0;
	bool o_obj = false;
	//std::list<pMOVE> q_packets;
	concurrency::concurrent_queue<pMOVE> q_packets;
	//std::atomic< std::list<pMOVE> > buffer;

	unsigned long prev_framecounter = 0;
	unsigned long packetcounter_send = 0;


	bool keystroke_s = false;
	bool keystroke_w = false;
	bool keystroke_a = false;
	bool keystroke_d = false;


	//friend Level;

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
	float getSpeed();
	std::string returnBulletsCount();
	void changeBulletCount(int v);
	void update(float dt)  override;
	void update(float dt, bool online) override;
	void init(bool online) override;
	void draw() 	override;
	void draw(bool online) override;



	void seto_id(int i);
	int* geto_id();
	///this is called by net to add a packet to the o_player's packet qeue
	void insertPlayerPacket(pMOVE packet);
	

	Player(int online_id);
	~Player();
};