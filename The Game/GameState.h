#pragma once
#include <string>
#include <sgg/graphics.h>
#include <chrono>
#include <thread>
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "Structs.h"
#include <array>
#include <concurrent_queue.h>
//#include "Net.h"






class GameState {
private:
	std::mutex mtx;
	

	std::string m_assets_path = "assets//";
	float m_canvas_width = 30.0f;
	float m_canvas_height = 20.0f;
	
	static GameState* m_uniqe_instance;
	

	class Player* m_player = nullptr;
	class Level* m_current_level = nullptr;
	class MainScreen* mainscreen = nullptr;

	//AUDIO//
	
	class AudioHandler* inputaudiohandler = nullptr;
	class AudioHandler* outputaudiohandler = nullptr;
	bool PushToTalk(bool);
	bool isStreaming = false;
	bool toSend = false; 
	bool PreviousState = false;
	bool CurrentState;
	std::thread pst;
	std::thread preperator; 
	std::thread receiver;
	std::thread playbackstarter; 
	float ReleaseTime = 0.0f;
	const float Waitasec = 500.0f;
	
	



	
	////// NET VARS //////

	class Net* net = nullptr;
	//std::vector<Player*> o_players;// a pointer to a vector called o_players that contains pointers to player objects
	std::map<int , std::unique_ptr<Player>> o_players; //amma pernaei to map se allh sinartish malon thelei sharedpointer
	bool online = false;
	bool host = false;
	std::string host_ip;
	int playersLoadedLevel = 0;


	startG mapinfo = {};
	
	
	
	
	



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
	bool  connectionFailed = false;
	bool connectingToHost = false;

	
	concurrency::concurrent_queue<int> playerToDelete;


	//int availableSpaceship[4] = {4,1,2,3};
	std::array<int, 4> availableSpaceship = {4,1,2,3};
	int  getAvailableSS(int ass, int oid);
	int setOPSpaceship(spaceShip p);
	void setavailableSpaceship(int oid);
	void changeAvailableSpaceshipArray(int pos, int value);

	void setMainpointerNull();
	void setLevelpointerNull();
	bool setSpaceship(int i);
	int getSpaceship();
	void setdificultyLVL(int d);
	int getdificultyLVL();
	void setOnline(bool _online, bool _host);
	std::string getHostIP();
	void setHostIP(std::string ip);
	bool getOnline();

	/////THIS FUNCTIONS SHOULD BE CALLED BY NET ONLY/////
	void createPlayer(const int id);
	void deletePlayer(const int id);
	int* connectpeer2player(int id);
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
	class Net* getNet() { return net ; }
	class MainScreen* getMainscreen() { return mainscreen; }
	void insertOPlayersPmove(struct pMOVE packet);
	const std::map<int, std::unique_ptr<Player>>& geto_playersmap();
	int getPlayersLoadedLevel();// get the numbers of connected players that have loaded the level in their local machine
	void playerLoadedLevel(); // increases the number of connected players that have loaded the level in their local machine
	void resetPlayerLoadedLevel();
	bool loadedLevel();// is the level has loaded on the local machine 

	bool amHost();
	struct startG getMapData();// net takes the map data to be send to peers
	void setMapData(startG strg);//game takes the map data to be loaded 

	struct startG& getMapInfo();// access the stored map data recieved from host 

	void createMapInfoData();
	void deletePlanet(int o_id);
	void deleteToken(int o_id);

	void incident(int type);

	void setSpawnPosOplayers();
	//PUBLIC AUDIO //
	void sendToPlayback(audiodata); //when receive audiodata send to audiohandler to playback
	void PlaybackStreamOpen(bool flag); //open or close the streams when connection or disconnection
	
	
	

};