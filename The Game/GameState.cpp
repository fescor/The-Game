#include "GameState.h"
#include "Level.h"
#include "Player.h"
#include "MainScreen.h"
#include "Net.h"


void GameState::createPlayer(const int id) // this should be called by net only
{
	o_players.push_back(new Player(id));

}

void GameState::deletePlayer(const int id) // this should be called by net only 
{
	for (auto iter = o_players.begin(); iter != o_players.end(); iter++) {
		if (*(*iter)->geto_id() == id) {
			delete *(iter);
			o_players.erase(iter);
			return;

		}

	}

}

int* GameState::connectpeer2player()
{
	return o_players.back()->geto_id();
	
}

void GameState::initNet()
{

	m_player = new Player("Player"); // i create my player instance when i go online to set  online things 
	net = new Net(host);
	//std::thread nt(&Net::run, net);
	
	nt = std::thread(&Net::run, net);
	
}

void GameState::deleteNet()
{

	if (net) {
		delete net;
	}
	net = nullptr;
	
}

void GameState::setMainpointerNull()
{
	mainscreen = nullptr;
}

void GameState::setLevelpointerNull()
{
	m_current_level = nullptr;
}


void GameState::setSpaceship(int i)
{
	spaceship = i;
}

int GameState::getSpaceship()
{
	return spaceship;
}

void GameState::setdificultyLVL(int d)
{
	dificultyLVL = d;
}

int GameState::getdificultyLVL()
{
	return dificultyLVL;
}

void GameState::setOnline(bool _online ,bool _host)
{
	online = _online;
	host = _host;
	return ;
}

bool GameState::getOnline()
{
	return online;
}



void GameState::setStatus(char s)
{
	status = s;
}



char GameState::getStatus()
{
	return status;
}

GameState::GameState()
{
	
}

void GameState::init()
{

	

	switch (status) {
	case 'M':
		if (!mainscreen) {
			mainscreen = new MainScreen();
		}	
		mainscreen->init();
		
		break;
	case'L':
		startLevel();
		break;

	}
	
		
	
	
	


	

	

	//graphics::preloadBitmaps(getAssetDir());

}

void GameState::draw()
{

	switch (status) {
	case 'M':
		mainscreen->draw();
		break;

	case'L':
		if (!m_current_level) {
			return;
		}
		m_current_level->draw();
		break;

	}

}

void GameState::update(float dt)
{
	if (dt > 500) {
		return;
	}
	
	
	switch (status) {
	case 'M':
		


		mainscreen->update(dt);

		break;
	case'L':
		if (!m_current_level) {
			framecounter++;
			return;
		}
		m_current_level->update(dt);
		break;

	}
	if (online && net == nullptr) { 
		initNet();
	}
	if (!online && net != nullptr) {
		
		net->setOnline(false);
		nt.join();

		deleteNet();

		if (m_player) {
			delete m_player;
		}

	}
	
	
	framecounter++;
	
	
	
}

GameState::~GameState()
{
	if (m_player) {

		delete m_player;
	}
	if (m_current_level) {

		delete m_current_level;
	}
	if (mainscreen) {
		delete mainscreen;
	}
	
}

void GameState::startLevel()
{
	
	if (!m_current_level) {
		m_current_level = new Level();
	}
	
	
	m_current_level->init();
	

	if (!m_player) {
		m_player = new Player("Player");
	}
	
	m_player->init();
}

GameState* GameState::getInstance()
{
	if (m_uniqe_instance == nullptr) {
		m_uniqe_instance = new GameState();
	}
	return m_uniqe_instance;
}

std::string GameState::getAssetDir()
{
	return m_assets_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_assets_path + asset;
}



GameState* GameState::m_uniqe_instance = nullptr;