#include "Player.h"
#include <sgg/graphics.h>

#include "GameState.h"

#include "setcolor.h"
#include <math.h>
#include "Bullets.h"
#include <iostream>
#include "Level.h"
#include "util.h"
#include <cmath>

using namespace std;

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
	bool f1 = false;
	float delta_time = dt / 2000.0f;
	
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		angle += pow(velocity, 2)/2 * delta_time;
		
	}
	
	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		
		angle -= pow(velocity, 2)/2 * delta_time;
		
		
	}
	
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		

		
		
		speed += delta_time * velocity;

		if (speed > 28.0f) {
			speed = 28.0f;
		}
		
	
	}
	else {		
				speed -= delta_time*velocity;
				if (speed < 0.0f) {
					speed = 0.0f;
				}
				
	}
		


		

	
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		speed -= 2*delta_time * velocity;
		if (speed < 0.0f) {
			speed = 0.0f;
		}
	}

	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		if (!flag) {
			graphics::playSound(m_state->getFullAssetPath("shoot2.mp3"), 1.0f, false);
			fire();
		}
		flag = true;
		
		//fire(dt);
	}
	else {
		flag = false;
	}
	//cout << angle ,  "/n";
	m_pos_y -= sin(radians(angle)) * (speed * delta_time);												//blocking player from going outside the map 
																										//those multipliers are given from a greater force of the universe
	m_pos_x += cos(radians(angle)) * (speed * delta_time);



	if (m_pos_x  > m_state->getLevel()->getBackground()->getWidth()*1.75 - m_state->getCanvasWidth() *2) { // deksia an thes na pigenei mexri gonia o pextis /2 anti *2

		m_pos_x = m_state->getLevel()->getBackground()->getWidth()* 1.75 - m_state->getCanvasWidth()*2;   //blocking player from going outside the map 
																										//those multipliers are given from a greater force of the universe
	}
	if (m_pos_x < -m_state->getLevel()->getBackground()->getWidth() * 1.5 + m_state->getCanvasWidth()*2) {

		m_pos_x = -m_state->getLevel()->getBackground()->getWidth() * 1.5 + m_state->getCanvasWidth() *2;
		
	}
	if (m_pos_y > m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() *2) {//kato 

		m_pos_y = m_state->getLevel()->getBackground()->getHeight() * 1.75 - m_state->getCanvasHeight() *2;
	}
	if (m_pos_y <  -m_state->getLevel()->getBackground()->getHeight()*1.5 + m_state->getCanvasHeight() *2) {

		m_pos_y = -m_state->getLevel()->getBackground()->getHeight() *1.5 + m_state->getCanvasHeight() *2;
	}
	
	
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;



	
		
	if (speed != 0.0f) {
		//graphics::playSound(m_state->getFullAssetPath("shoot2.mp3"), 1.0f, true);

	}
		
		
	
	
}

float Player::getAngle()
{
	return angle;
}

void Player::init()
{
	e = new Explosion(m_pos_x, m_pos_y);
	e->init();


	//init values public
	explosion = false;
	flag2 = false;
	//init values private
	m_bullets = 200;
	hp = 5;

	flag = false;
	

	angle = 90.0f;
	speed = 0;
	sprite_counter = 0;
	shield = false;
	shieldFramecounter = 0;
	hp = 5;


	//init player_box properties
	m_pos_x = 5.0f;
	m_pos_y = 5.0f;
	//m_width /= 1.5f;
	//m_height /= 1.5f;
	//player brush properties
	SETCOLOR(m_brush_player.fill_color, 1.0f, 1.0f, 1.0f);
	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.fill_secondary_opacity = 0.0f;
	m_brush_player.outline_opacity = 1.0f;
	m_brush_player.texture = m_state->getFullAssetPath("spaceship" + to_string(m_state->getSpaceship()) + ".png");
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

void Player::seto_id(int i)
{
	o_id = i;
}

int* Player::geto_id()
{
	return &o_id;
}

Player::Player(int online_id)
{
	o_id = online_id;
}

float Player::getY()
{
	return m_pos_y;
}

Player::~Player()
{
	if (e) {
		delete e;
	}
	
}
