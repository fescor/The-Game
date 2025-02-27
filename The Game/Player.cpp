#include "Player.h"
#include <sgg/graphics.h>
#include "GameState.h"
#include "Net.h"
#include "setcolor.h"
#include <math.h>
#include "Bullets.h"
#include <iostream>
#include "Level.h"
#include "util.h"
#include <cmath>


// TODO : interpolate from prev potition to new potition with the fixed tickrate so between tickrates you neet to interpolate in the update,  smooth the renderin te quifsa



#define TURN_LEFT 1
#define TURN_RIGHT 2
#define IDLE 0

void Player::fire()
{
	float adjust = 0;
	
	if (m_bullets <= 0 ) {
		cout << "no bullets";
		return;
	}
	
	if (angle != 90)  {
		adjust += 0.4;
	}
	
	
	m_state->getLevel()->shoot( new Bullet( m_pos_x  , m_pos_y + adjust , angle));
	m_bullets--;


}


void Player::loadExplosionSprites()
{
	for (int i = 1; i < 8; i++) {
		sprites_explosion.push_back(m_state->getFullAssetPath("explosion//explosion" + to_string(i) + ".png"));
		sprites_explosion.push_back(m_state->getFullAssetPath("explosion//explosion" + to_string(i) + ".png"));
	}
}

void Player::update(float dt, bool online)
{

	float delta_time = dt / 2000.0f;
	float fixed_timeStep = tickrate / 2000.0f;


	m_pos_x += cos(radians(angle)) * (speed * delta_time);
	m_pos_y -= sin(radians(angle)) * (speed * delta_time);

	if (prev_pos.x < new_pos.x) {
		if (m_pos_x > new_pos.x) {
			m_pos_x = new_pos.x;
		}

	}
	else {
		if (m_pos_x < new_pos.x) {
			m_pos_x = new_pos.x;
		}
	}

	if (prev_pos.y < new_pos.y) {
		if (m_pos_y > new_pos.y) {
			m_pos_y = new_pos.y;
		}

	}
	else {
		if (m_pos_y < new_pos.y) {
			m_pos_y = new_pos.y;
		}
	}
	










	if (graphics::getKeyState(graphics::SCANCODE_H)) {
		cout << "breakpoint";
	}
	std::string s = "PACKET COUNT : " + std::to_string(q_packets.unsafe_size()) + "\n";
	timeStepCounter += dt;
	if (timeStepCounter < tickrate) {

		/*
		potition c = lerp(prev_pos, new_pos, timeStepCounter / tickrate);
		
		m_pos_x = c.x;
		m_pos_y = c.y;
		angle = c.angle;
		
		
		*/

		//return;


	}
	timeStepCounter = .0f;






	if (!q_packets.empty()) {


		pMOVE move;
		if (!q_packets.try_pop(move)) {
			cout << "failed to pop move" << endl;
		}
		/*
		std::string s =
			"DRAWING MOVE PACKET WITH ID : " + std::to_string(move.fc) + "\n" +
			"AT FRAME " + to_string(m_state->framecounter) + "\n" +
			"AT : " + std::to_string(graphics::getGlobalTime()) + "\n";
		cout << s;
		*/



		if (graphics::getKeyState(graphics::SCANCODE_B))
		{
			cout << "breakpoint" << endl;
		}

		
		prev_pos = new_pos;

		
		angle = prev_pos.angle;
		speed = prev_pos.speed;
		m_pos_x = prev_pos.x;
		m_pos_y = prev_pos.y;
		

		
		
		if (move.fire) {
			graphics::playSound(m_state->getFullAssetPath("shoot2.mp3"), 1.0f, false);
			fire();

		}
		





		new_pos.x = move.x;
		new_pos.y = move.y;
		new_pos.angle = move.angle;
		new_pos.speed = move.speed;
		new_pos.frame = m_state->framecounter;
		new_pos.fire = move.fire;
		updated_flag = true;

		return;
	}
	return;




}

void Player::getKeyStrokes()
{

	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		input.key_A = 1;


	}
	else {
		input.key_A = 0;

	}

	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		input.key_D = 1;
		
	}
	else {
		input.key_D = 0;
	}

	if (graphics::getKeyState(graphics::SCANCODE_W)) {

		input.key_W = 1;




	}
	else {
		input.key_W = 0;
	}






	if (graphics::getKeyState(graphics::SCANCODE_S)) {

		input.key_S = 1;

	
	}
	else {
		input.key_S = 0;
	}

	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		input.space = 1;

		//fire(dt);
	}
	else {
		input.space = 0;
		
	}





}

void Player::resetKeyStrokes()
{
	input.key_A = 0;
	input.key_D = 0;
	input.key_S = 0;
	input.key_W = 0;
	input.space = 0;

}

int Player::simulateNewPos()
{
	
	//float delta_time = dt / 2000.0f;
	float fixed_timeStep = tickrate / 2000.0f;
	int o_angle = IDLE;

	
	

	if (input.key_A) {

		PreviousFrameAngle = angle;
		//angle += pow(velocity, 2) / 2 * fixed_timeStep;
		new_pos.angle = prev_pos.angle +  pow(velocity, 2) / 2 * fixed_timeStep;
		
		isAngleIdle = false;

		o_angle = TURN_LEFT;
		keystroke_a = true;

	}

	if (input.key_D) {

		PreviousFrameAngle = angle;
		//angle -= pow(velocity, 2) / 2 * fixed_timeStep;
		new_pos.angle = prev_pos.angle - pow(velocity, 2) / 2 * fixed_timeStep;
		isAngleIdle = false;
		o_angle = TURN_RIGHT;

	}



	if (input.key_W) {
		
		//speed += fixed_timeStep * velocity;
		new_pos.speed = prev_pos.speed + fixed_timeStep * velocity;
		//speed += velocity;
		if (new_pos.speed > 28.0f) {
			new_pos.speed = 28.0f;
		}


	}
	else {
		//speed -= fixed_timeStep * velocity;
		new_pos.speed = prev_pos.speed - fixed_timeStep * velocity;
		if (new_pos.speed < 0.0f) {
			new_pos.speed = 0.0f;
		}

	}

	if (input.key_S) {
		//speed -= 2 * fixed_timeStep * velocity;
		new_pos.speed = prev_pos.speed - 3* (fixed_timeStep * velocity);
		if (new_pos.speed < 0.0f) {
			new_pos.speed = 0.0f;
		}
	}
	

	if (input.space) {

		if (!fire_flag) {
			graphics::playSound(m_state->getFullAssetPath("shoot2.mp3"), 1.0f, false);
			fire();
			m_fire = true;
		}
		fire_flag = true;
		

		//fire(dt);
	}
	else {
		fire_flag = false;

	}
	new_pos.x = prev_pos.x + cos(radians(new_pos.angle)) * (new_pos.speed * fixed_timeStep);
	new_pos.y = prev_pos.y - sin(radians(new_pos.angle)) * (new_pos.speed * fixed_timeStep);

	new_pos = outOfBounds(new_pos);

	


	
	new_pos.frame = m_state->framecounter;
	return o_angle;
}

potition Player::lerp(potition start_pos, potition goal_pos, float t)// fix lerp from medium forum u are doing it wrong 
{
	float speedDiff = goal_pos.speed - current_pos.speed;
	float angleDiff = goal_pos.angle - current_pos.angle;

	float fixed_timeStep = tickrate / 2000.0f;

	potition cur;


	if (graphics::getKeyState(graphics::SCANCODE_G)) {
		cout << "breakpoint";
	}

	/*
	cur.speed = start_pos.speed + (goal_pos.speed - start_pos.speed) * t;
	if (goal_pos.speed > start_pos.speed && cur.speed > goal_pos.speed) {
		cur.speed = goal_pos.speed;
	}
	else if (goal_pos.speed < start_pos.speed && cur.speed < goal_pos.speed) {
		cur.speed = goal_pos.speed;
	}

	if (start_pos.angle > goal_pos.angle) {
		cur.angle = start_pos.angle - (abs(goal_pos.angle) - abs(start_pos.angle)) * t;


	}
	else {
		cur.angle = start_pos.angle + (abs(goal_pos.angle) - abs(start_pos.angle)) * t;
	}
	cur.x = m_pos_x + cos(radians(cur.angle)) * (cur.speed * fixed_timeStep);
	cur.y = m_pos_y - sin(radians(cur.angle)) * (cur.speed * fixed_timeStep);

	return cur;
	*/
	if (t > 1.0f) {
		t = 1.0f;
	}
	else if (t < 0.0f) {
		t = .0f;
	}
	cur.x = std::lerp(start_pos.x, goal_pos.x, t);
	cur.y = std::lerp(start_pos.y, goal_pos.y, t);
	cur.angle = std::lerp(start_pos.angle, goal_pos.angle, t);
	return cur;
	


	if (t <=0.0f) {
		cur.x = start_pos.x;
		cur.y = start_pos.y;
		cur.angle = start_pos.angle;

	}
	else if(t < 1.f){

		cur.x = start_pos.x + (goal_pos.x - start_pos.x) * t;
		cur.y = start_pos.y + (goal_pos.y - start_pos.y) * t;
		cur.angle = start_pos.angle + (goal_pos.angle - start_pos.angle) * t;

	}
	else{
		cur.x = goal_pos.x;
		cur.y = goal_pos.y;
		cur.angle = goal_pos.angle;

	}
	
	return cur;


	/*



	if (current_pos.angle < goal_pos.angle) {

		if (angle + pow(velocity, 2) / 2 * fixed_timeStep > goal_pos.angle) {
			angle = goal_pos.angle;

		}
		else {
			angle += pow(velocity, 2) / 2 * dt / 2000;
		}


	}
	else if (current_pos.angle > goal_pos.angle) {
		if (angle + pow(velocity, 2) / 2 * fixed_timeStep < goal_pos.angle) {
			angle = goal_pos.angle;

		}
		else {
			angle -= pow(velocity, 2) / 2 * dt / 2000;
		}

	}



	if (speedDiff > 0) {
		speed += dt/2000;
		if (cos(radians(angle)) * (speed * fixed_timeStep) < 0) {
			if (goal_pos.x < m_pos_x + cos(radians(angle)) * (speed * fixed_timeStep)) {
				m_pos_x += cos(radians(angle)) * (speed * fixed_timeStep);

			}
		}
		else {
			if (goal_pos.x > m_pos_x + cos(radians(angle)) * (speed * fixed_timeStep)) {
				m_pos_x += cos(radians(angle)) * (speed * fixed_timeStep);
			}
		}
		if (sin(radians(angle)) * (speed * fixed_timeStep) > 0) {
			if (goal_pos.y < m_pos_y - sin(radians(angle)) * (speed * fixed_timeStep)) {
				m_pos_y -= sin(radians(angle)) * (speed * fixed_timeStep);
			}
		}
		else {
			if (goal_pos.y > m_pos_y - sin(radians(angle)) * (speed * fixed_timeStep)) {
				m_pos_y -= sin(radians(angle)) * (speed * fixed_timeStep);
			}
		}
		
		
	}
	else if(speedDiff < -dt) {
		speed -= dt;
		m_pos_x += cos(radians(angle)) * (speed * fixed_timeStep);
		m_pos_y -= sin(radians(angle)) * (speed * fixed_timeStep);
	}
	else {
		return;
	}
	*/



	












}

potition Player::outOfBounds(potition pos)
{


	if (pos.x > m_state->getLevel()->getBackground()->getWidth() * 1.75 - m_state->getCanvasWidth() * 2) { // deksia an thes na pigenei mexri gonia o pextis /2 anti *2

		pos.x = m_state->getLevel()->getBackground()->getWidth() * 1.75 - m_state->getCanvasWidth() * 2;   //blocking player from going outside the map 
		//those multipliers are given from a greater force of the universe
	}
	if (pos.x < -m_state->getLevel()->getBackground()->getWidth() * 1.5 + m_state->getCanvasWidth() * 2) {

		pos.x = -m_state->getLevel()->getBackground()->getWidth() * 1.5 + m_state->getCanvasWidth() * 2;

	}
	if (pos.y > m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() * 2) {//kato 

		pos.y = m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() * 2;
	}
	if (pos.y < -m_state->getLevel()->getBackground()->getHeight() * 1.5 + m_state->getCanvasHeight() * 2) {

		pos.y = -m_state->getLevel()->getBackground()->getHeight() * 1.5 + m_state->getCanvasHeight() * 2;
	}
	return pos;

}
void Player::outOfBounds()
{


	if (m_pos_x > m_state->getLevel()->getBackground()->getWidth() * 1.75 - m_state->getCanvasWidth() * 2) { // deksia an thes na pigenei mexri gonia o pextis /2 anti *2

		m_pos_x = m_state->getLevel()->getBackground()->getWidth() * 1.75 - m_state->getCanvasWidth() * 2;   //blocking player from going outside the map 
		//those multipliers are given from a greater force of the universe
	}
	if (m_pos_x < -m_state->getLevel()->getBackground()->getWidth() * 1.5 + m_state->getCanvasWidth() * 2) {

		m_pos_x = -m_state->getLevel()->getBackground()->getWidth() * 1.5 + m_state->getCanvasWidth() * 2;

	}
	if (m_pos_y > m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() * 2) {//kato 

		m_pos_y = m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() * 2;
	}
	if (m_pos_y < -m_state->getLevel()->getBackground()->getHeight() * 1.5 + m_state->getCanvasHeight() * 2) {

		m_pos_y = -m_state->getLevel()->getBackground()->getHeight() * 1.5 + m_state->getCanvasHeight() * 2;
	}




}

void Player::Activateshield()
{
	shield = true;
}

std::string Player::returnBulletsCount()
{
	return to_string(m_bullets);
}

void Player::changeBulletCount(int v)
{
	m_bullets += v;

}

void Player::update(float dt)
{
	timeStepCounter += dt;
	if (timeStepCounter < tickrate) {
		
		
		potition c = lerp(prev_pos, new_pos, timeStepCounter/tickrate);
		c  = outOfBounds(c);
		m_pos_x = c.x;
		m_pos_y = c.y;
		angle = c.angle;
		
		m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
		m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

		//m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
		//m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;
		
		return;


	}
	
	timeStepCounter = 0.0f;
	//timeStepCounter -= tickrate;

	///HERE IT ENTERS EVERY 40ms ////

	
	float delta_time = dt / 2000.0f;
	float fixed_timeStep = tickrate / 2000.0f;
	int o_angle = IDLE;

	
	prev_pos = new_pos;
	
	m_pos_y = prev_pos.y;
	m_pos_x = prev_pos.x;
	speed = prev_pos.speed;
	angle = prev_pos.angle;
	
	


	getKeyStrokes();
	o_angle = simulateNewPos();
	resetKeyStrokes();

	
	



	outOfBounds();


	
	
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;
	


	
		
	
	
	testcounter++;
	if (m_state->getOnline() && !(speed == 0.0f && isAngleIdle && !m_fire) ) {
		if (graphics::getGlobalTime() - lastPacket_timeSend >= tickrate) {

			
			m_state->getNet()->addpMOVEToQueue(o_id, angle, speed, m_pos_x, m_pos_y, testcounter ,m_fire);
			lastPacket_timeSend = graphics::getGlobalTime();
			if (m_fire) {
				
				m_fire = false;
			}
		}
	}
	/*
	cout << std::to_string(dt) + "\n";

	
	if (m_state->getOnline() && !(speed == 0.0f && isAngleIdle)) {
		m_state->getNet()->addpMOVEToQueue(o_id, o_angle, speed, m_pos_x, m_pos_y , testcounter);
		packetcounter_send++;
	}
	*/
	isAngleIdle = true;
	

		
	
	
}

float Player::getAngle()
{
	return angle;
}

void Player::init(bool online)
{
	e = new Explosion(m_pos_x, m_pos_y);
	e->init();


	//init values public
	explosion = false;
	flag2 = false;
	//init values private
	m_bullets = 200;
	hp = 5;

	fire_flag = false;
	m_fire = false;

	angle = 90.0f;
	PreviousFrameAngle = 90.0f; 
	
	speed = 0;
	sprite_counter = 0;
	shield = false;
	shieldFramecounter = 0;
	hp = 5;


	//init player_box properties
	m_pos_x = 5.0;
	m_pos_y = 5.0;
	
	prev_pos.x = m_pos_x;
	prev_pos.y = m_pos_y;
	prev_pos.angle = 90.0f;
	prev_pos.speed = 0.0f;
	prev_pos.frame = m_state->framecounter;

	new_pos.x = m_pos_x;
	new_pos.y = m_pos_y;
	new_pos.angle = 90.0f;
	new_pos.speed = 0.0f;
	new_pos.frame = m_state->framecounter;

	current_pos = new_pos;

	

	//m_width /= 1.5f;
	//m_height /= 1.5f;
	
	//player brush properties
	SETCOLOR(m_brush_player.fill_color, 1.0f, 1.0f, 1.0f);
	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.fill_secondary_opacity = 0.0f;
	m_brush_player.outline_opacity = 1.0f;
	m_brush_player.texture = m_state->getFullAssetPath("spaceship" + to_string(spaceship) + ".png");

	//shield brush properties
	SETCOLOR(m_brush_shield.fill_secondary_color, 0, 0, 255);
	SETCOLOR(m_brush_shield.fill_color, 0, 0, 255);
	m_brush_shield.fill_secondary_opacity = 1.0f;
	m_brush_shield.fill_opacity = 0.2;
	
	
	
	
	loadExplosionSprites();

}


float Player::getX()
{
	return m_pos_x;
}

void Player::draw()
{	




	if (shield && shieldFramecounter < 1000) {
		graphics::drawDisk(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5f, 2.0f, m_brush_shield);
		shieldFramecounter++;
	}
	else {
		shieldFramecounter = 0;
		shield = false;
	}

	SETCOLOR(test.fill_color, 255, 0, 0);

	m_brush_player.outline_opacity = 0.0f;

	graphics::setOrientation(angle - 90.0f);
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 2.0f, 2.0f, m_brush_player);
	graphics::resetPose();

	if (explosion){
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

void Player::draw(bool online)
{
	
	potition c;
	c.x = m_pos_x;
	c.y = m_pos_y;
	c.angle = angle;
	
	if (timeStepCounter < tickrate) {

		cout << "T = : " << timeStepCounter / tickrate;
		c = lerp(prev_pos, new_pos, timeStepCounter / tickrate);





	}







	if (shield && shieldFramecounter < 1000) {
		graphics::drawDisk(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 2.0f, m_brush_shield);
		shieldFramecounter++;
	}
	else {
		shieldFramecounter = 0;
		shield = false;
	}


	m_brush_player.outline_opacity = 0.0f;
	graphics::setOrientation(angle - 90.0f);
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, 2.0f, 2.0f, m_brush_player);
	graphics::resetPose();

		SETCOLOR(test.fill_color, 255, 255, 0);
	

	graphics::drawText(this->m_pos_x + m_state->m_global_offset_x, this->m_pos_y + m_state->m_global_offset_y, 1.0f, " X : " + std::to_string(this->m_pos_x) + " ,  Y :" + std::to_string(this->m_pos_y) + " , SP : " + std::to_string(speed), test);
	graphics::drawText(this->m_pos_x + m_state->m_global_offset_x, this->m_pos_y + m_state->m_global_offset_y + 3, 1.0f, "angle : " + std::to_string(angle), test);
	if (explosion) {// implement it corectly for online players
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

int Player::getLives()
{
	return lives;
}

void Player::reduceLives()
{
	lives--;
}

void Player::resetLives()
{
	lives = 3;
}

void Player::respawn()
{
	this->changeHP(5);// reset hp
	m_bullets = 200;//reset bullets 
	m_pos_x = spawn_pos.x;
	m_pos_y = spawn_pos.y;
	prev_pos.x = spawn_pos.x;
	prev_pos.y = spawn_pos.y;
	prev_pos.angle = 90;
	prev_pos.speed = 0;
	new_pos.x = spawn_pos.x;
	new_pos.y = spawn_pos.y;
	new_pos.angle = 90;
	new_pos.speed = 0;

	speed = 0;
	angle = 90;

	if (m_state->getOnline()) {
		for (auto& o_player : m_state->geto_playersmap()) {
			o_player.second->clearQueuedPackets();
		}

	}

}

void Player::seto_id(int i)
{
	o_id = i;
}

int* Player::geto_id()
{
	return &o_id;
}

void Player::insertPlayerPacket(pMOVE packet)// this is called by net thread.
{
	
	//m_state->getMutex().lock();
	q_packets.push(packet);
	//m_state->getMutex().unlock();

}

void Player::setPrevPos(float x , float y ,float speed , float angle ,  int fc)
{
	prev_pos.x = x;
	prev_pos.y = y;
	prev_pos.angle = angle;
	prev_pos.speed  = speed;
	prev_pos.frame = fc;




}

potition Player::getprevPos()
{
	return prev_pos;
}

void Player::setSpawnPos(float x, float y)
{
	spawn_pos.x = x;
	spawn_pos.y = y;
}

void Player::setNewPos(float x, float y,float speed , float angle , int fc)
{
	new_pos.x = x;
	new_pos.y = y;
	new_pos.angle = angle;
	new_pos.speed = speed;
	new_pos.frame = fc;

}

potition Player::getNewPos()
{
	return new_pos;
}

void Player::setPSpaceship(int ss)
{
	spaceship = ss;

}

int Player::getPSpaceship()
{
	return spaceship;
}

void Player::clearQueuedPackets()
{
	q_packets.clear();

}

potition Player::getSpawnPos()
{
	return spawn_pos;
}



Player::Player(int online_id)
{
	o_id = online_id;
	
}

float Player::getY()
{
	return m_pos_y;
}

float Player::getSpeed()
{
	return speed;
}

Player::~Player()
{
	if (e) {
		delete e;
	}
	
}

