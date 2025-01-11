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
struct keystrokes {

	int key_A = 0;
	int key_W = 0;
	int key_S = 0;
	int key_D = 0;
	int space = 0;




};
struct potition
{
	float x = NULL;
	float y = NULL;
	int frame = NULL;
	float angle = NULL;
	float speed = NULL;
	bool fire = false;

	bool operator == (potition& b) {
		return (x == b.x && y == b.y && angle == b.angle);
	}

};


class Player : public GameObject, public Box, public Health
{
	const float tickrate = 50.0f;
	float timeStepCounter = .0f;
	float lastPacket_timeSend = .0;
	float maxTimeLerp = 50.0f; // this should be the 40ms of the tickrate + rtt/2 (round trip time of packet /2 cuz i want the time elapse in send bot send and  back)
	float elapsedTimeLerp = 0.0f;
	



	bool isAngleIdle = true;
	int testcounter = 0;
	graphics::Brush test;
	graphics::Brush m_brush_shield;
	graphics::Brush m_explosion_brush;
	graphics::Brush m_brush_player;
	int  m_bullets;
	graphics::Brush m_bullet_brush;
	Explosion* e;
	bool fire_flag;
	bool m_fire;
	void fire();
	void loadExplosionSprites();
	const float velocity = 2.0f * 12.0f;
	float angle;
	float PreviousFrameAngle;
	float speed;
	int sprite_counter;
	int lives = 3;

	int shieldFramecounter;
	int spaceship = 0;

	keystrokes input;

	potition prev_pos;
	potition new_pos;
	potition current_pos;

	bool updated_flag = false;


	potition spawn_pos;





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

	void getKeyStrokes();
	void resetKeyStrokes();
	int simulateNewPos();
	potition lerp(potition start_pos , potition goal_pos , float t);

	potition outOfBounds(potition pos );
	void outOfBounds();

	float fadeInPlayer = 0;



	//friend Level;

protected:

public:

	bool shield;
	bool explosion;
	void Activateshield();
	Player(std::string name) : GameObject(name) {}
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
	int getLives();
	void reduceLives();
	void respawn();



	void seto_id(int i);
	int* geto_id();
	///this is called by net to add a packet to the o_player's packet qeue
	void insertPlayerPacket(pMOVE packet);

	void setPrevPos(float x, float y, float speed , float angle  , int fc);
	potition getprevPos();
	void setSpawnPos(float x, float y);

	void setNewPos(float x, float y,float speed , float angle ,  int fc);
	potition getNewPos();
	void setPSpaceship(int spaceship);
	int getPSpaceship();

	void clearQueuedPackets();


	Player(int online_id);
	~Player();
};
