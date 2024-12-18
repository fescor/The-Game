#pragma once
#include "sgg/graphics.h"
#include "box.h"
#include "GameObject.h"
#include <list>
#include "Planet.h"
#include "Asteroid.h"
#include "Bullets.h"
#include <unordered_map>
#include "Background.h"
#include "Tokens.h"
#include <fstream>
#include <sstream>
#include "Minimap.h"

class Level : public GameObject
{
	graphics::Brush m_brush_player_hp;
	graphics::Brush m_brush_player_bullet_count;
	
	
	graphics::Brush m_brush_background;
	graphics::Brush m_brush_explosion;
	
	graphics::Brush m_planet_healthbar;
	



	Background* m_background;

	//std::vector<Planet*> m_planets;
	//std::vector<Asteroid*> m_asteroids;
	std::unordered_map<int, Planet*> m_planets;
	std::unordered_map<int, Asteroid*> m_asteroids;

	std::vector<int> map1x;
	std::vector<int> map1y;
	std::vector<int> planet_level;

	std::vector<int> token_x;
	std::vector<int> token_y;
	std::vector<char> token_type;

	//std::vector<std::string> m_planet_names;

	//std::vector<Bullet*> m_player_bullets;

	std::vector<Bullet*> m_planet_bullets_active;
	std::vector<Bullet*> m_player_bullets_active;

	//std::vector<Explosion*> m_explosion;
	std::unordered_map<int ,Tokens*> m_tokens;


	graphics::Brush m_planet_brush;
	void createGameObjects();
	void collectToken();
	void planetFireRange();
	void bulletColissionPlanet();
	void bulletColissionPlayer();
	void checkColissions();
	void mapPlanets();
	void initObjects();
	void restart();
	void restartSiege();
	void readMap(const std::string& filePath);
	void readTokens(const std::string& filePath);
	bool inView(float x , float y);
	bool flagC = false;
	int levelDiffuculty;
	int bulletDMG;
	int AcollisionDMG;
	int siege = 1;
	int score = 0;
	bool efyges = false;

	bool allPlayersLoadedLevel();
	

	

public:
	
	
	
	
	int collisionFramecounter = 0;
	bool fire = false;
	Background* getBackground();
	
	Level(const std::string& name = "level1");


	friend class Minimap;
	Minimap* m_minimap;

	void shoot(Bullet* b);
	void Pshoot(Bullet* b);
	void update(float dt)  override;
	void init() 	override;
	void draw() 	override;
	~Level();

};