#pragma once
#include <string>
#include <sgg/graphics.h>
#include <chrono>
#include <thread>
#include <string>




class GameState {
private:
	std::string m_assets_path = "assets//";
	float m_canvas_width = 30.0f;
	float m_canvas_height = 20.0f;
	//mainScreen* mainscreen = 0;
	static GameState* m_uniqe_instance;
	

	class Player* m_player = 0;
	class Level* m_current_level = 0;
	class MainScreen* mainscreen = 0;

	
	////// NET VARS //////

	class Net* net = nullptr;
	std::vector<Player*> o_players;// a pointer to a vector called o_players that contains pointers to player objects
	bool online = false;
	bool host = false;
	

	



	graphics::Brush m_brush_background;
	graphics::Brush m_brush_player;

	float m_player_posx = m_canvas_height / 2.0f; // starting point of my player the center of the screen
	float m_player_posy = m_canvas_width / 2.0f;
	float m_player_size = 2.0f;
	int selections[4] = { 0 , 1 , 2, 3 };
	char status = 'M';
	int dificultyLVL = 0;
	int spaceship = 0;


	void initNet();
	void deleteNet();

	std::thread nt;

protected:




public:
	void setMainpointerNull();
	void setLevelpointerNull();
	void setSpaceship(int i);
	int getSpaceship();
	void setdificultyLVL(int d);
	int getdificultyLVL();
	void setOnline(bool _online, bool _host);
	bool getOnline();

	/////THIS FUNCTIONS SHOULD BE CALLED BY NET ONLY/////
	void createPlayer(const int id);
	void deletePlayer(const int id);
	int* connectpeer2player();
	/////////////////////////////////////////////////////


	
	void setStatus(char s);
	
	char getStatus();
	GameState();
	float m_global_offset_x = 0.0f;  //borw na kanw friend class ton pexti kai na exw private ta offset o pextis ta pernei me get/set
	float m_global_offset_y = 0.0f;
	void init();
	void draw();
	void update(float dt);
	unsigned long framecounter = 0;

	~GameState();
	void startLevel();
	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }


	static GameState* getInstance();
    std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	class Player* getPlayer() { return m_player; }
	class Level* getLevel() { return m_current_level; }
	

};