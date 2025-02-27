#include "Level.h"
#include "Player.h"

#include "GameState.h"
#include "mainScreen.h"
#include "setcolor.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <math.h>
#include "util.h"
#include "Net.h"
#include <vector>
using namespace std;

void Level::readMap(const std::string& filePath)
{
	//dimiourgia
	std::ifstream maptxt(filePath);

	if (!maptxt.is_open()) {
		std::cerr << "Error opening file: " << filePath << std::endl;
		return;
	}
	//pou apothikeuw

	std::string line;
	//loop diabasmatos 
	while (std::getline(maptxt, line)) { 
		std::istringstream iss(line);
		int x, y, planetlvl;
		//an den diabazei 
		if (!(iss >> x >> y >> planetlvl)) {
			std::cerr << "Error reading values from line: " << line << std::endl;
			continue;
		}
		//apothikeush 
		map1x.push_back(x);
		map1y.push_back(y);
		planet_level.push_back(planetlvl);

	}

	maptxt.close();
}

void Level::readTokens(const std::string& tokensFilePath)
{
	std::ifstream tokenstxt(tokensFilePath);

	if (!tokenstxt.is_open()) {
		std::cerr << "Error opening file: " << tokensFilePath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(tokenstxt, line)) {
		std::istringstream iss(line);
		int x, y;
		char tokenType;

		if (!(iss >> x >> y >> tokenType)) {
			std::cerr << "Error reading values from line: " << line << std::endl;
			continue;
		}

		token_x.push_back(x);
		token_y.push_back(y);
		token_type.push_back(tokenType);
	}

	tokenstxt.close();
}

bool Level::inView(float x, float y)// checking for collisions is very expensive  i just check objects that are in players view
{
	
	if ( abs(fmax(m_state->getPlayer()->m_pos_x, x) - fmin(m_state->getPlayer()->m_pos_x, x)) < 2.0f + m_state->getCanvasWidth() * 0.5 && abs(fmax(m_state->getPlayer()->m_pos_y, y) - fmin(m_state->getPlayer()->m_pos_y, y)) < 2.0f + m_state->getCanvasHeight() * 0.5) {
		return true;
	}

	return false;
}



bool Level::allPlayersLoadedLevel()
{
	return m_state->geto_playersmap().size() == m_state->getPlayersLoadedLevel();
}








void Level::createGameObjects()
{
	
	for (int i = 0; i < map1x.size(); i++) {
		/*
		m_asteroids.push_back( new Asteroid((float)map1x[i], (float)map1y[i]));

		m_planets.push_back(new Planet("planet" + std::to_string(i) + ".png"));
		m_planets[i]->setlevel(planet_level[i]);
		*/
		m_planets[i] = new Planet("planet" + std::to_string(i) + ".png");
		m_planets[i]->setlevel(planet_level[i]);

		m_asteroids[i] = new Asteroid((float)map1x[i], (float)map1y[i]);
		

	}
	
	
	
	
	
	
		
	if (m_state->amHost()) {
		for (int i = 0; i < token_x.size(); i++) {
			//m_tokens.push_back(new Tokens(token_type[i], token_x[i], token_y[i]));
			m_tokens[i] = new Tokens(token_type[i], token_x[i], token_y[i], i);
		}

	}
	else {
		for (int i = 0; i < token_x.size(); i++) {
			//m_tokens.push_back(new Tokens(token_type[i], token_x[i], token_y[i]));
			m_tokens[i] = new Tokens(token_type[i], token_x[i], token_y[i], m_state->getMapInfo().token_oid[i]);
		}

	}
	
	
	
	


	
	
	
	
	

	


}

void Level::collectToken()
{
	//unordered_map<int, Tokens>::iterator  iter;

	for (auto it = m_tokens.begin(); it != m_tokens.end(); it++ ) {
		if (inView(it->second->m_pos_x, it->second->m_pos_y)){
			if (m_state->getPlayer()->intersect(*it->second)) {

				switch (it->second->getType()) {
				case 'B':
					m_state->getPlayer()->changeBulletCount(2);

						break;
				case 'H':
					m_state->getPlayer()->changeHP(2);

						break;
				case 'S':
					m_state->getPlayer()->Activateshield();

   
					break; 

				}
				if (m_state->getOnline() && m_state->getNet()) {
					m_state->getNet()->addLevelPacketToQueue(it->first, 1);
				}


				delete it->second;
				m_tokens.erase(it);
				return;
			}
		}

	}
}

void Level::planetFireRange()// if the player is inside the fire range around a planet then the planets shoots to the diraction of the player
{
	auto player = m_state->getPlayer();

	//for (int i = 0; i < m_planets.size(); i++)
	for(auto iter = m_planets.begin(); iter != m_planets.end(); iter++)
	{
		//if (m_planets[i]->getlevel() == 2 || m_planets[i]->getlevel() == 3) {
		if(iter->second->getlevel() == 2 || iter->second->getlevel()==3){
			//if (m_planets[i]->intersectFireRange(*player)) {
			if(iter->second->intersectFireRange(*player)){
				//m_planets[i]->fire(m_state->getPlayer()->m_pos_x, m_state->getPlayer()->m_pos_y);
				iter->second->fire(player->getX(), player->getY());
				
			}
		}
	}
}

void Level::bulletColissionPlanet()
{

	for (int i = 0; i < m_player_bullets_active.size(); i++) {
		//for (int j = 0; j < m_planets.size(); j++) {

		for (auto& pair : m_planets) {
			Planet* planet = pair.second;
			if (inView(planet->m_pos_x, planet->m_pos_y)) {

				//if (m_planets[j]->intersect(*m_player_bullets_active[i])) {
				if (planet->intersect(*m_player_bullets_active[i])) {
					//m_planets[j]->changeHP(-2);
					//m_planets[j]->explosion = true;
					planet->changeHP(-2);
					planet->explosion = true;

					delete m_player_bullets_active[i];
					m_player_bullets_active.erase(m_player_bullets_active.begin() + i);


					//if (m_planets[j]->returnHP() == 0) {
					if (planet->returnHP() <= 0) {
						score += 100;
						//delete m_planets[j];
						if (m_state->getOnline() && m_state->getNet()) {
							m_state->getNet()->addLevelPacketToQueue(pair.first, 0);
						}
						

						delete planet;
						delete m_asteroids[pair.first];
						m_asteroids.erase(pair.first);
						//m_planets.erase(m_planets.begin() + j);
						m_planets.erase(pair.first);
						graphics::playSound(m_state->getFullAssetPath("big-explosion.wav"), 1.0f, false);
						//delete m_asteroids[j];
						//m_asteroids.erase(m_asteroids.begin() + j);

					}
					else {
						graphics::playSound(m_state->getFullAssetPath("explosion.wav"), 1.0f, false);
					}
					return;

				}
			}
				
				

			
			
		}
		
		

	}
	

}
void Level::bulletColissionPlayer() 
{

	for (int i = 0; i < m_planet_bullets_active.size(); i++) {
		

		
			if (m_state->getPlayer()->intersect(*m_planet_bullets_active[i])) {
				if (!m_state->getPlayer()->shield) {
					m_state->getPlayer()->explosion = true;
					m_state->getPlayer()->changeHP(-bulletDMG);
				}


				delete m_planet_bullets_active[i];
				m_planet_bullets_active.erase(m_planet_bullets_active.begin() + i);
				return;
			}
		
	}

}
void Level::checkColissions()
{	
	//for (int i = 0; i < m_planets.size(); i++) {
	for (auto& pair : m_planets){
		Planet* planet = pair.second;
		//if (inView(m_planets[i]->m_pos_x, m_planets[i]->m_pos_y)) {
		if(inView(planet->m_pos_x,planet->m_pos_y)) {
		
			//if (m_state->getPlayer()->intersect(*m_planets[i])) {
			if(m_state->getPlayer()->intersect(*planet)) {
				if (!flagC) {
			
					m_state->getPlayer()->changeHP(-5);
				
					collisionFramecounter = m_state->framecounter;
					flagC = true;
					return;

				}
				if (collisionFramecounter + 200 < m_state->framecounter) {
					flagC = false;
					return;
				}
			
			
			}
		}
		//if (inView(m_asteroids[i]->m_pos_x, m_asteroids[i]->m_pos_y)) {
		if(inView(m_asteroids[pair.first]->m_pos_x , m_asteroids[pair.first]->m_pos_y)){
				//if (m_state->getPlayer()->intersect(*m_asteroids[i])) {
			if(m_state->getPlayer()->intersect(*m_asteroids[pair.first])){
				if (!flagC) {

					m_state->getPlayer()->changeHP(-AcollisionDMG);

					collisionFramecounter = m_state->framecounter;
					flagC = true;
					return;

				}
				if (collisionFramecounter + 200 < m_state->framecounter) {
					flagC = false;
					return;
				}


			}
		}
			
	}
	
		



}

void Level::mapPlanets() 
{
	int index = 0;
	//for (int i = 0; i < m_planets.size(); i++) { 
	for(auto iter = m_planets.begin(); iter != m_planets.end(); iter++){
		/*
		m_planets[i]->m_pos_x = map1x[i];
		m_planets[i]->m_pos_y = map1y[i];
		*/
		iter->second->m_pos_x = map1x[index];
		iter->second->m_pos_y =  map1y[index];
		if (m_state->amHost()) {
			iter->second->setoid(index);
		}
		else {
			iter->second->setoid(m_state->getMapInfo().planet_oid[index]);
		}
		
		
		index++;
	}
}

void Level::initObjects()
{
	
	for (auto& p : m_planets) {
		p.second->init();
	}



	for (auto& asteroid : m_asteroids) {
		asteroid.second->init();
	}
	


	/*
	for (int i = 0; i < m_planets.size(); i++) {
		m_planets[i]->init();
		m_asteroids[i]->init();
	}
	*/


	for (auto t = m_tokens.begin(); t != m_tokens.end(); ++t) {
		t->second->init();
	}


}

Background* Level::getBackground()
{
	return m_background;
}

Level::Level(const std::string& name)
{

	if (!m_state->getOnline() || m_state->amHost()) {

		readMap(m_state->getFullAssetPath("vectors.txt"));
		readTokens(m_state->getFullAssetPath("tokens.txt"));
	}

	
}

void Level::shoot(Bullet* b)// level is responsible for init draw updating and deleting bullets 
{
	b->init();
	m_player_bullets_active.push_back(b);

}

void Level::Pshoot(Bullet* b)
{
	b->init();
	m_planet_bullets_active.push_back(b);
}

void Level::update(float dt)
{

	if (incidentGoToLobby && m_state->getOnline()) {

		switch (incidentType) {
		case PLAYER_DEAD:	
			graphics::setFont(m_state->getFullAssetPath("font.ttf"));
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "PLAYER DIED GAME OVER", m_brush_player_bullet_count);
			break;
		case COMPLETED_LEVEL:
			graphics::setFont(m_state->getFullAssetPath("font.ttf"));
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "LEVEL COMPLETED GOOD JOB", m_brush_player_bullet_count);
			break;
		case NEW_SIEGE:
			graphics::setFont(m_state->getFullAssetPath("font.ttf"));
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "SIEGE COMPLETED", m_brush_player_bullet_count);
			break;

		}

		if (!deathflag) {

			timeofIncedent = graphics::getGlobalTime();
			deathflag = true;
		}
		if (graphics::getGlobalTime() - timeofIncedent < 3000) {

			return;
		}
		


		deleteLevel();
		return;

	}



	//if (m_planets.size() == 0 && levelDiffuculty != 3) {
	if (m_planets.empty() && levelDiffuculty != 3) {
		if (m_state->getOnline()) {

			graphics::setFont(m_state->getFullAssetPath("font.ttf"));
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "LEVEL COMPLETED GOOD JOB", m_brush_player_bullet_count);
			if (!deathflag) {

				
				if (m_state->getOnline() && m_state->getNet()) {
					m_state->getNet()->addLevelPacketToQueue(*m_state->getPlayer()->geto_id(), COMPLETED_LEVEL);
				}

				timeofIncedent = graphics::getGlobalTime();
				deathflag = true;
			}

			if (graphics::getGlobalTime() - timeofIncedent < 3000) {

				return;
			}


			deleteLevel();
			return;





		}
		else {
			graphics::setFont(m_state->getFullAssetPath("font.ttf"));
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "EISAI OREOS PEXTIS", m_brush_player_bullet_count);
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 3.0f, "PRESS R TO RESTART", m_brush_player_bullet_count);
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 10.0f, 3.0f, "PRESS M FOR MAIN MENU", m_brush_player_bullet_count);
			if (graphics::getKeyState(graphics::SCANCODE_R)) {


				restart(); //delete the main objects of the level (planets bullets and asteroids) and clear their vectors
				m_state->init();





			}
			if (graphics::getKeyState(graphics::SCANCODE_M)) {

				m_state->setLevelpointerNull();
				m_state->setStatus('M');
				delete this;
			}
			//cout << "a";
			return;
		}

	}
	//else if(m_planets.size() == 0){
	else if (m_planets.empty()) {
		restart();
		createGameObjects();
		mapPlanets();

		initObjects();

		if (m_state->getOnline() && m_state->getNet()) {
			m_state->getNet()->addLevelPacketToQueue(*m_state->getPlayer()->geto_id(), NEW_SIEGE);
		}

		std::this_thread::sleep_for(std::chrono::duration<float, milli>(800));
		siege++;
		score += 1000;


		return;
	}
	else if (graphics::getKeyState(graphics::SCANCODE_P) && levelDiffuculty == 3) { // delete this after
		restart();
		createGameObjects();
		mapPlanets();

		initObjects();

		std::this_thread::sleep_for(std::chrono::duration<float, milli>(800));
		siege++;
		score += 1000;


		return;
	
	}


	if (m_state->getPlayer()->returnHP() <= 0) {

		

			if (m_state->getPlayer()->getLives() == 1) {
				//he is dead game over do the game over thingy
				
				
				if (m_state->getOnline()) {
					graphics::setFont(m_state->getFullAssetPath("font.ttf"));
					graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "GAME OVER", m_brush_player_bullet_count);
					if (!deathflag) {

						if (!efyges) {
							graphics::playSound(m_state->getFullAssetPath("efyges1.mp3"), 1.0f, false);// hope you dont get mad:)
							efyges = true;
						}
						if (m_state->getOnline() && m_state->getNet()) {
							m_state->getNet()->addLevelPacketToQueue(*m_state->getPlayer()->geto_id(), PLAYER_DEAD);
						}

						timeofIncedent = graphics::getGlobalTime();
						deathflag = true;
					}

					if (graphics::getGlobalTime() - timeofIncedent < 3000) {
						
						return;
					}
					


					deleteLevel();
					return;
				}
				else {
					if (!efyges) {
						graphics::playSound(m_state->getFullAssetPath("efyges1.mp3"), 1.0f, false);// hope you dont get mad:)
						efyges = true;
					}

					graphics::setFont(m_state->getFullAssetPath("font.ttf"));
					graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "GAME OVER", m_brush_player_bullet_count);
					graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 3.0f, "PRESS R TO RESTART", m_brush_player_bullet_count);
					graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 10.0f, 3.0f, "PRESS M FOR MAIN MENU", m_brush_player_bullet_count);
					if (graphics::getKeyState(graphics::SCANCODE_R)) {


						restart(); //delete the main objects of the level (planets bullets and asteroids) and clear their vectors
						m_state->init();





					}
					if (graphics::getKeyState(graphics::SCANCODE_M)) {
						deleteLevel();

					}
					//cout << "a";
					return;



				}
				
			}
			else {
				m_state->getPlayer()->reduceLives();
				m_state->getPlayer()->respawn();
				
				playerRespawning = true;
				std::this_thread::sleep_for(std::chrono::duration<float, milli>(3000));
				timeOfdeath = graphics::getGlobalTime();
				
				
				
				


			}



	}		/*
		else {
			

		}
		*/

	


	float delta_time = dt / 2000.0f;
	const float v = 12.0f;


	if (m_state->getPlayer()->isActive()) {
		m_state->getPlayer()->update(dt);
	}


	for (auto iter = m_state->geto_playersmap().begin(); iter != m_state->geto_playersmap().end(); iter++) {

		if (iter->second->isActive()) { 
			iter->second->update(dt, true); 
		}

	}




	
	for (auto& asteroids : m_asteroids) {
		asteroids.second->update(dt);
	}
	for (auto& planet : m_planets) {
		planet.second->update(dt);
	}
	
	/*
	for (int i = 0; i < m_planets.size(); i++) {
		m_planets[i]->update(dt);
		m_asteroids[i]->update(dt);
	}
	*/
	for (int i = 0; i < m_player_bullets_active.size(); i++) {
		if (!inView(m_player_bullets_active[i]->m_pos_x , m_player_bullets_active[i]->m_pos_y)) {// if a bullet goes out of the view its deleted added this because the minimap is too overpoewred
			
			delete m_player_bullets_active[i];
			m_player_bullets_active.erase(m_player_bullets_active.begin() + i);
		}
	}
	for (int i = 0; i < m_planet_bullets_active.size(); i++) {
		if (!inView(m_planet_bullets_active[i]->m_pos_x, m_planet_bullets_active[i]->m_pos_y)) {

			delete m_planet_bullets_active[i];
			m_planet_bullets_active.erase(m_planet_bullets_active.begin() + i);
		}
	}
	for (auto& b : m_player_bullets_active) {

		b->update(dt);

	}
	for (auto& b : m_planet_bullets_active) {

		b->update(dt);

	}

	if (m_state->getOnline()) {
		
		for (auto& planet_id : planetToDeleteNET) {

			
			m_planets.erase(planet_id);
			
		}
		for (auto& token_id : tokenToDeleteNET) {

			
			m_tokens.erase(token_id);
			
		}


	}




	
	m_minimap->update(dt);

	collectToken();
	planetFireRange();
	bulletColissionPlayer();
	bulletColissionPlanet();
	checkColissions();
	GameObject::update(dt);
}

void Level::init()
{
	efyges = false;
	levelDiffuculty = m_state->getdificultyLVL();
	switch (levelDiffuculty) {
	case 0:
		bulletDMG = 1;
		AcollisionDMG = 2;
		break;
	case 1:
		bulletDMG = 1;
		AcollisionDMG = 5;
		break;
	case 2:
		bulletDMG = 5;
		AcollisionDMG = 5;
		break;
	case 3:
		bulletDMG = 1;
		AcollisionDMG = 2;
		break;

	}

	




	score = 0;
	createGameObjects();
	mapPlanets();
	m_background = new Background();
	m_minimap = new Minimap();
	m_minimap->init();
	m_background->init();
	initObjects();
	m_state->createMapInfoData();


	for (int i = 1; i < m_state->geto_playersmap().size() + 1; i++) { //start from i=1 cuz on posPlayer[0][0] is my info always (host)
		
		m_state->geto_playersmap().find(m_state->getMapInfo().posPlayer[i][0])->second->m_pos_x = m_state->getMapInfo().posPlayer[i][1];// girnaei apo to find iter meta to end kai skaei 
		// to proto find tha einai gi 0 pou den yparxei sto o_players
		m_state->geto_playersmap().find(m_state->getMapInfo().posPlayer[i][0])->second->m_pos_y = m_state->getMapInfo().posPlayer[i][2];

		m_state->geto_playersmap().find(m_state->getMapInfo().posPlayer[i][0])->second->setPrevPos(m_state->getMapInfo().posPlayer[i][1], m_state->getMapInfo().posPlayer[i][2], 0, 90.0f, m_state->framecounter);
		m_state->geto_playersmap().find(m_state->getMapInfo().posPlayer[i][0])->second->setNewPos(m_state->getMapInfo().posPlayer[i][1], m_state->getMapInfo().posPlayer[i][2], 0, 90.0f, m_state->framecounter);
	}





	//graphics::playMusic("assets//Sectio Aurea - Giove.mp3", 1.0f, true, 200000);
	
}

void Level::init(startG mapinfo)
{
	efyges = false;
	levelDiffuculty = m_state->getdificultyLVL();
	switch (levelDiffuculty) {
	case 0:
		bulletDMG = 1;
		AcollisionDMG = 2;
		break;
	case 1:
		bulletDMG = 1;
		AcollisionDMG = 5;
		break;
	case 2:
		bulletDMG = 5;
		AcollisionDMG = 5;
		break;
	case 3:
		bulletDMG = 1;
		AcollisionDMG = 2;
		break;

	}
	
	
		for (int i = 0; i < sizeof(mapinfo.map_x) / sizeof(mapinfo.map_x[0]); i++) {

			map1x.push_back(mapinfo.map_x[i]);
			map1y.push_back(mapinfo.map_y[i]);
			planet_level.push_back(mapinfo.planet_lvl[i]);


		}
		for (int i = 0; i < sizeof(mapinfo.token_x) / sizeof(mapinfo.token_x[0]); i++) {

			token_x.push_back(mapinfo.token_x[i]);
			token_y.push_back(mapinfo.token_y[i]);
			token_type.push_back(mapinfo.token_type[i]);

		}



	
	for (int i = 0; i < m_state->geto_playersmap().size() + 1; i++) {
		if (mapinfo.posPlayer[i][0] == *(m_state->getPlayer()->geto_id())) {
			m_state->getPlayer()->m_pos_x = mapinfo.posPlayer[i][1];
			m_state->getPlayer()->m_pos_y = mapinfo.posPlayer[i][2];
			m_state->getPlayer()->setPrevPos(mapinfo.posPlayer[i][1] , mapinfo.posPlayer[i][2] , 0 , 90.0f , m_state->framecounter);
			m_state->getPlayer()->setNewPos(mapinfo.posPlayer[i][1], mapinfo.posPlayer[i][2], 0 ,90.0f ,   m_state->framecounter);
			m_state->getPlayer()->setSpawnPos(mapinfo.posPlayer[i][1], mapinfo.posPlayer[i][2]);
		}
		else {
			m_state->geto_playersmap().find(mapinfo.posPlayer[i][0])->second->m_pos_x = mapinfo.posPlayer[i][1];
			m_state->geto_playersmap().find(mapinfo.posPlayer[i][0])->second->m_pos_y = mapinfo.posPlayer[i][2];
			m_state->geto_playersmap().find(mapinfo.posPlayer[i][0])->second->setPrevPos(mapinfo.posPlayer[i][1], mapinfo.posPlayer[i][2], 0 , 90.0f , m_state->framecounter);
			m_state->geto_playersmap().find(mapinfo.posPlayer[i][0])->second->setNewPos(mapinfo.posPlayer[i][1], mapinfo.posPlayer[i][2], 0, 90.0f, m_state->framecounter);

		}
		
		
	}






	score = 0;
	createGameObjects();
	mapPlanets();
	m_background = new Background();
	m_minimap = new Minimap();
	m_minimap->init();
	m_background->init();
	initObjects();












}

void Level::draw()
{
	
	
	m_background->draw();

	for (auto& planet : m_planets) {

		planet.second->draw();


	}


	for (auto& asteroids : m_asteroids) {

		asteroids.second->draw();
	}

	for (auto t = m_tokens.begin(); t != m_tokens.end(); ++t) {
		t->second->draw();
	}


	for (auto iter = m_state->geto_playersmap().begin(); iter != m_state->geto_playersmap().end(); iter++) {

		if (iter->second->isActive()) { iter->second->draw(true); }



	}
	if (m_state->getPlayer()->isActive()) {

		m_state->getPlayer()->draw();
	}
	

	
	/*
	for (int i = 0; i < m_planets.size(); i++) {
		m_planets[i]->draw();
		m_asteroids[i]->draw();
	}
	*/
	
	for (auto& b : m_player_bullets_active) {

		b->draw();
	}
	for (auto& b : m_planet_bullets_active) {

		b->draw();
	}



	





	//HUD//

	m_brush_player_hp.outline_opacity = 0.0f;
	
	m_brush_player_hp.texture = m_state->getFullAssetPath("//healthbar//healthbar"+ std::to_string((m_state->getPlayer()->returnHP()))+ ".png");
	graphics::drawRect(5.4f, 0.4f, 10, 0.4f, m_brush_player_hp);
	
	
	SETCOLOR(m_brush_player_bullet_count.fill_color, 255, 0, 0);
	graphics::setFont(m_state->getFullAssetPath("font.ttf"));
	graphics::drawText(28.0f, 0.8f, 1.2f,m_state->getPlayer()->returnBulletsCount(), m_brush_player_bullet_count);
	m_brush_player_bullet_count.outline_opacity = 0.0f;

	m_brush_player_bullet_count.texture = m_state->getFullAssetPath("bullets-icon.png");
	graphics::drawRect(27.0f, 0.5, 1.0f, 1.0f, m_brush_player_bullet_count);
	
	for (int i = 0; i < m_state->getPlayer()->getLives(); i++) {
		
		m_brush_player_bullet_count.texture = m_state->getFullAssetPath("health-icon.png");
		graphics::drawRect(16.0f + i, 0.5, 0.6f, 0.5f, m_brush_player_bullet_count);


	}
	

	graphics::setFont(m_state->getFullAssetPath("font.ttf"));
	//graphics::drawText(13.0f, 0.8f, 1.2f, "PLANETS LEFT : " + to_string(m_planets.size()), m_brush_player_bullet_count);
	graphics::drawText(m_state -> getCanvasWidth()*0.1, m_state->getCanvasHeight(), 1.2f, "PLANETS LEFT : " + to_string(m_planets.size()), m_brush_player_bullet_count);
	
	if (levelDiffuculty == 3) {


		graphics::setFont(m_state->getFullAssetPath("font.ttf"));
		graphics::drawText(m_state->getCanvasWidth() * 0.6, m_state->getCanvasHeight(), 1.2f, "SCORE: " + to_string(score), m_brush_player_bullet_count);



		graphics::setFont(m_state->getFullAssetPath("font.ttf"));
		graphics::drawText(m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight(), 1.2f, "SIEGE : " + to_string(siege), m_brush_player_bullet_count);



	}
	

	m_minimap->draw();
	
	
	if (m_state->getOnline() && !gameLoaded) {
		m_state->getNet()->sendLoadedLevelMSG(*(m_state->getPlayer()->geto_id()));
		gameLoaded = true;
		while (!allPlayersLoadedLevel()) {
		
		}
		m_state->resetPlayerLoadedLevel();
		
		
	}
	if (playerRespawning &&  graphics::getGlobalTime() - timeOfdeath < 5000) {


		graphics::setFont(m_state->getFullAssetPath("font.ttf"));
		graphics::drawText(m_state->getCanvasWidth()*0.35, m_state->getCanvasHeight() - 2.0f, 1.0f, "YOU DIED! " + std::to_string(m_state->getPlayer()->getLives()) + " LIVES REMAINING", m_brush_player_bullet_count);
		
	}
	else {
		playerRespawning = false;
	}









	
}

bool Level::getGameLoadedStatus()
{
	return gameLoaded;
}

std::unordered_map<int, Planet*>& Level::getm_planets()
{
	return m_planets;
}

std::unordered_map<int, Tokens*>& Level::getm_tokens()
{
	return m_tokens;
}

void Level::deleteLevel()
{

	
	m_state->setStatus('M');
	if (m_state->getNet()) {
		m_state->getNet()->setinGame(false);
		if (m_state->amHost()) {
			m_state->getMainscreen()->setSelector(LOBBY_SCREEN_STARTG);
		}
		else {
			m_state->getMainscreen()->setSelector(LOBBY_SCREEN_CS);
		}
	}
	
	m_state->setLevelpointerNull();
	m_state->getPlayer()->resetLives();
	m_state->getPlayer()->clearQueuedPackets();
	
	
	
	delete this;


}

void Level::addToVectorDeleteP(int oid)
{
	planetToDeleteNET.push_back(oid);


}

void Level::addToVectorDeleteT(int oid)
{
	tokenToDeleteNET.push_back(oid);
}

void Level::setIncident(int iType)
{
	incidentGoToLobby = true;
	incidentType = iType;

}



void Level::restart() 
{
	
	for (auto& a : m_asteroids) {
		
			delete a.second;
		
	}
	m_asteroids.clear();
	for (auto& p : m_planets) {
		
			delete p.second;
		
	}
	m_planets.clear();
	
	/*
	for (int i = 0; i < m_planets.size(); i++) {
		delete m_planets[i];
		delete m_asteroids[i];
	}
	m_planets.clear();
	m_asteroids.clear();
	*/
	for (auto& b : m_player_bullets_active) {
		if (b) {
			delete b;
		}
	}
	m_player_bullets_active.clear();
	for (auto& b : m_planet_bullets_active) {
		if (b) {
			delete b;
		}
	}
	m_planet_bullets_active.clear();

	for (auto t = m_tokens.begin(); t != m_tokens.end(); ) {
		if (t->second) {
			delete t->second;
			t = m_tokens.erase(t); 
		}

	}

	m_state->getPlayer()->resetLives();
	



}
void Level::restartSiege()
{

	



}
Level::~Level()
{
	/*
	for (int i = 0; i < m_planets.size(); i++) {
		delete m_planets[i];
		delete m_asteroids[i];
	}
	*/
	
	for (auto& planet : m_planets) {
		delete planet.second;
	}
	for (auto& asteroid : m_asteroids) {
		 delete asteroid.second;
	}
	
	
	for (auto& p_gob : m_player_bullets_active) {
		if (p_gob) delete p_gob;
	}
	
	for (auto& p_gob : m_planet_bullets_active) {
		if (p_gob) delete p_gob;
	}
	for (auto t = m_tokens.begin(); t != m_tokens.end(); ++t) {
		if (t->second) delete t->second;
	}
	
	if (m_background) {
		delete m_background;
	}
	

}
