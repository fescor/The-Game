#include "GameState.h"
#include "Level.h"
#include "Player.h"
#include "MainScreen.h"
#include "Net.h"
#include <portaudio.h>
#include "AudioHandler.h"



void GameState::createPlayer(const int id) // this should be called by net only
{
	
	o_players[id] = std::make_unique<Player>(id);

}

void GameState::deletePlayer(const int id) // this should be called by net only 
{
	o_players.erase(id); // dont need to delete smart pointer sees out of scope and deletes it self

}

int* GameState::connectpeer2player(int id)
{
	return o_players.find(id)->second->geto_id();

}

void GameState::PlaybackStreamOpen(bool flag) {
	if (flag) {
		if (!outputaudiohandler) {
			outputaudiohandler = new AudioHandler();
		}
		//pst = std::thread(&AudioHandler::startplaybackstream, audiohandler);
		outputaudiohandler->startplaybackstream();
	}
	else {
		/*
		if (pst.joinable()) {
			pst.join();
		}
		*/
		outputaudiohandler->stopPlaybackAudio();
		delete outputaudiohandler;
		outputaudiohandler = nullptr;
	}
	
}


bool GameState::PushToTalk(bool isStreaming) {
	if (isStreaming) {
		
		if (!inputaudiohandler) {
			inputaudiohandler = new AudioHandler();//gia na ginei init thn prwth fora mono mexri na to ksana pathsei 
			//audiohandler = std::make_unique<AudioHandler>();
			
		}
		//pst = std::thread(&AudioHandler::startAudio, inputaudiohandler);//thread start the stream
		inputaudiohandler->startAudio();	

		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		//preperator = std::thread(&AudioHandler::preparedata, inputaudiohandler);//thread start to send the data
		return true;
	}else
	{		
		
		
		inputaudiohandler->stopAudio();

		delete inputaudiohandler;
		inputaudiohandler = nullptr;
		return false;
	}
		
}
	
	

void GameState::sendToPlayback(audiodata ad) {

	int player_id = ad.playerid;
	std::vector<float> chunk(std::begin(ad.audioData), std::end(ad.audioData));
	
	
	//std::cout << "Starting thread for startstream..." << std::endl;
//	playbackstarter = std::thread(&AudioHandler::startplaybackstream, outputaudiohandler);

	//std::cout << "Starting thread for setbuffer..." << std::endl;
	//receiver = std::thread(&AudioHandler::setbuffer, outputaudiohandler, player_id, chunk);
	outputaudiohandler->setbuffer(player_id,chunk);
	
}



void GameState::initNet()
{

	
	net = new Net(host);
	//std::thread nt(&Net::run, net);
	
	nt = std::thread(&Net::run,  net);
	
}

void GameState::deleteNet()
{

	if (net) {
		delete net;
	}
	net = nullptr;
	o_players.clear();
	playersLoadedLevel = 0;
	
}

int GameState::getAvailableSS(int ss, int oid)
{
	
	if (ss == o_players[oid]->getPSpaceship() && availableSpaceship[ss] != 4) {
		
		return ss; 
	}
	else if (ss == o_players[oid]->getPSpaceship() && availableSpaceship[ss] == 4) {
		for (int i = 0; i < 4; i++) {
			if (availableSpaceship[i] != 4) {
				changeAvailableSpaceshipArray(i, 4);
				
				
				return i;
			}
		}

	}
	else if (ss != o_players[oid]->getPSpaceship() && availableSpaceship[ss] != 4) {
		int prev_onlinep_spaceship = o_players[oid]->getPSpaceship();
		
		changeAvailableSpaceshipArray(prev_onlinep_spaceship, prev_onlinep_spaceship);
		changeAvailableSpaceshipArray(ss, 4);
		
		
		return ss;


	}
	else if (ss != o_players[oid]->getPSpaceship() && availableSpaceship[ss] == 4) {
		int prev_onlinep_spaceship = o_players[oid]->getPSpaceship();
		changeAvailableSpaceshipArray(prev_onlinep_spaceship, prev_onlinep_spaceship);
		for (int i = 0; i < 4; i++) {
			if (availableSpaceship[i] != 4) {
				changeAvailableSpaceshipArray(i, 4);
				
				return i;
			}
		}

	}
	





}

int GameState::setOPSpaceship(spaceShip p)
{
	if (amHost()) {
		int spaceship = getAvailableSS(p.spaceShip , p.o_id);

		o_players.find(p.o_id)->second->setPSpaceship(spaceship);

		return spaceship;
	}
	else {
		mtx.lock();
		availableSpaceship[0] = p.availableS[0];
		availableSpaceship[1] = p.availableS[1];
		availableSpaceship[2] = p.availableS[2];
		availableSpaceship[3] = p.availableS[3];
		mtx.unlock();
		if (p.spaceShip == 5) {
			o_players[0]->setPSpaceship(p.hostSpaceship);
		}
		else{
			if (p.o_id == *getPlayer()->geto_id()) {
				getPlayer()->setPSpaceship(p.spaceShip);

			}
			else {
				geto_playersmap().find(p.o_id)->second->setPSpaceship(p.spaceShip);
			}
			if (p.hostSpaceship != o_players[0]->getPSpaceship()) {
				o_players[0]->setPSpaceship(p.hostSpaceship);
			}

		}
		for (int i = 0; i < 3; i++) {
			if (p.lobbyPlayersSS[i] != 0) {
				if (!(p.lobbyPlayersSS[i] == *getPlayer()->geto_id())){
					o_players.find(p.lobbyPlayersSS[i])->second->setPSpaceship(i);
				}
				

			}
		}


		return 1;
		






		if (p.spaceShip != 5) {// 5 means that the only host changes spaces ship
			if (o_players.find(0)->second->getPSpaceship() != p.hostSpaceship) {
				//mtx.lock();
				//availableSpaceship[o_players.find(0)->second->getPSpaceship()] = o_players.find(0)->second->getPSpaceship();
				o_players.find(0)->second->setPSpaceship(p.hostSpaceship);
				//availableSpaceship[p.hostSpaceship] = 4;
				//mtx.unlock();

			}


			if (p.o_id == *getPlayer()->geto_id()) {
				getPlayer()->setPSpaceship(p.spaceShip);
				/*
				mtx.lock();
				availableSpaceship[p.spaceShip] = 4;
				mtx.unlock();
				*/
			}
			else {
				//mtx.lock();
				//availableSpaceship[o_players.find(p.o_id)->second->getPSpaceship()] = o_players.find(p.o_id)->second->getPSpaceship();
				o_players.find(p.o_id)->second->setPSpaceship(p.spaceShip);
				//availableSpaceship[p.spaceShip] = 4;
				//mtx.unlock();
			}

		}
		else if (p.spaceShip == 5) {
			
			
			//mtx.lock();
			//availableSpaceship[o_players[0]->getPSpaceship()] = o_players[0]->getPSpaceship();
			o_players[0]->setPSpaceship(p.hostSpaceship);
			//availableSpaceship[p.hostSpaceship] = 4;

			//mtx.unlock();


		}
		
		
	}
	

}

void GameState::setavailableSpaceship(int oid)
{
	changeAvailableSpaceshipArray(geto_playersmap().find(oid)->second->getPSpaceship(), geto_playersmap().find(oid)->second->getPSpaceship());
}

void GameState::changeAvailableSpaceshipArray(int pos, int value)
{
	mtx.lock();
	availableSpaceship[pos] = value;
	mtx.unlock();


}

void GameState::setMainpointerNull()
{
	mainscreen = nullptr;
}

void GameState::setLevelpointerNull()
{
	if (m_current_level) {
		m_current_level = nullptr;
	}
	
}


bool GameState::setSpaceship(int i)
{
	if (spaceship == i) { // if its the same do nothing
		return false;
	}
	spaceship = i;
	getPlayer()->setPSpaceship(i);
	changeAvailableSpaceshipArray(i, 4);
	return true;
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

std::string GameState::getHostIP()
{
	return host_ip;
}

void GameState::setHostIP(std::string ip)
{
	ip.erase(ip.end());
	host_ip = ip;
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

	if (!m_player) {
		m_player = new Player("Player"); // i create my player instance when i go online to set  online things 
	}

	

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



	if (status == 'L' && m_current_level == nullptr) {

		init();

	}
	if (status == 'L' && m_current_level == nullptr) {

		init();



	}

	//float CurrentTime = graphics::getGlobalTime();
	CurrentState = graphics::getKeyState(graphics::SCANCODE_K);
	if ((CurrentState) && (!PreviousState) && online) {

		if (!isStreaming) {
			isStreaming = true; //edw ksekinaei h diadikasia tou stream
			GameState::PushToTalk(isStreaming);
			
		}
	}
	if ((!CurrentState) && (PreviousState) && online) {
		
		if (isStreaming){
			isStreaming = false;
			GameState::PushToTalk(false);
		}
	}

	PreviousState = CurrentState; //update state
	


	
	
	if (!playerToDelete.empty()) {
		int o_id; 
		playerToDelete.try_pop(o_id);
		deletePlayer(o_id);

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
		/*
		if (m_player) {
			delete m_player;
			m_player = nullptr;
		}
		*/

		if (m_current_level) {
			getLevel()->deleteLevel();
		}
		for (int i = 0; i < 4; i ++) {
			if (i != getPlayer()->getPSpaceship()) {
				changeAvailableSpaceshipArray(i, i);
			}
		}
		mainscreen->setSelector(PLAY);
		
		

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
	

	if (!m_player) {
		m_player = new Player("Player");
	}
	
	m_player->init(false);
	
	for (auto iter = o_players.begin(); iter != o_players.end(); iter++) {

		if (iter->second) { iter->second->init(true); }
	}

	if (!amHost() && online) {
		m_current_level->init(mapinfo);
	}
	else {
		m_current_level->init();
	}

	

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

void GameState::insertOPlayersPmove(pMOVE packet)
{

	o_players.find(packet.id)->second->insertPlayerPacket(packet);


}

const std::map<int , std::unique_ptr<Player>>& GameState::geto_playersmap()// is this okey do i need to have shared pointers?
{
	return o_players;
}

int GameState::getPlayersLoadedLevel()
{
	return playersLoadedLevel;
}

void GameState::playerLoadedLevel()
{
	playersLoadedLevel++;
}

void GameState::resetPlayerLoadedLevel()
{
	playersLoadedLevel = 0;
}

bool GameState::loadedLevel()
{
	if (m_current_level) {
		return getLevel()->getGameLoadedStatus();
	}
	return false;
	

}
	
	

bool GameState::amHost()
{
	return host;
}

startG GameState::getMapData()
{
	startG strg;
	
	std::unordered_map<int, Planet*> planets = getLevel()->getm_planets();
	int i = 0;
	for (auto key : planets) {
		strg.map_x[i] = key.second->m_pos_x;
		strg.map_y[i] = key.second->m_pos_y;
		strg.planet_lvl[i] = key.second->getlevel();
		strg.planet_oid[i] = key.second->getoid();

		i++;

	}
	std::unordered_map<int, Tokens*> tokens = getLevel()->getm_tokens();
	i = 0;
	for (auto key : tokens) {

		strg.token_x[i] = key.second->m_pos_x;
		strg.token_y[i] = key.second->m_pos_y;
		strg.token_type[i] = key.second->getType();
		strg.token_oid[i] = key.second->getoid();
		i++;


	}
	return strg;
	
}



void GameState::setMapData(startG strg)
{

	mapinfo = strg;


}



startG& GameState::getMapInfo()
{
	return mapinfo;
}
void GameState::deletePlanet(int o_id)
{
	mtx.lock();
	getLevel()->addToVectorDeleteP(o_id);
	mtx.unlock();

}

void GameState::deleteToken(int o_id)
{


	mtx.lock();
	getLevel()->addToVectorDeleteT(o_id);
	mtx.unlock();




}
void GameState::incident(int type)
{
	getLevel()->setIncident(type);

}
void GameState::setSpawnPosOplayers()
{
	for (auto& o_player : o_players) {
		o_player.second->m_pos_x = o_player.second->getSpawnPos().x;
		o_player.second->m_pos_y = o_player.second->getSpawnPos().y;

	}


}
void GameState::createMapInfoData()
{


	std::unordered_map<int, Planet*> planets = getLevel()->getm_planets();
	int i = 0;
	for (auto key : planets) {
		mapinfo.map_x[i] = key.second->m_pos_x;
		mapinfo.map_y[i] = key.second->m_pos_y;
		mapinfo.planet_lvl[i] = key.second->getlevel();
		mapinfo.planet_oid[i] = key.second->getoid();

		i++;

	}
	std::unordered_map<int, Tokens*> tokens = getLevel()->getm_tokens();
	i = 0;
	for (auto key : tokens) {

		mapinfo.token_x[i] = key.second->m_pos_x;
		mapinfo.token_y[i] = key.second->m_pos_y;
		mapinfo.token_type[i] = key.second->getType();
		mapinfo.token_oid[i] = key.second->getoid();
		i++;


	}





	mapinfo.timeinfo = 0.0f;
	i = 1;
	// this is the pos of the host
	mapinfo.posPlayer[0][0] = 0;
	mapinfo.posPlayer[0][1] = getPlayer()->m_pos_x;
	mapinfo.posPlayer[0][2] = getPlayer()->m_pos_y;
	getPlayer()->setSpawnPos(mapinfo.posPlayer[0][1], mapinfo.posPlayer[0][2]);


	// this is the pos of connected peer i give them(as host)
	for (auto iter = o_players.begin(); iter != o_players.end(); iter++) {
		if (i % 2) {
			mapinfo.posPlayer[i][0] = iter->first;
			mapinfo.posPlayer[i][1] = getPlayer()->m_pos_x + (i * 2);
			mapinfo.posPlayer[i][2] = getPlayer()->m_pos_y + (i * 2);

		}
		else {
			mapinfo.posPlayer[i][0] = iter->first;
			mapinfo.posPlayer[i][1] = getPlayer()->m_pos_x - (i * 2);
			mapinfo.posPlayer[i][2] = getPlayer()->m_pos_y - (i * 2);

		}

		i++;
	}
	
	

}








GameState* GameState::m_uniqe_instance = nullptr;