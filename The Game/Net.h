#pragma once
#include <enet/enet.h>
#include <vector>
#include "GameState.h"
#include "GameObject.h"
#include <list>
#include <map>
#include <fstream>
#include "cereal/archives/binary.hpp"
#include "cereal/archives/portable_binary.hpp"
#include <mutex>
#include <queue>
#include "Structs.h"
#include <memory>
#include <ppl.h>
#include <concurrent_queue.h>





using namespace std;
class Net : public GameObject {
	
	bool inGame = false; // this tells if the lobby is in game or not
	


	std::mutex net_mutex;
	map<int,ENetPeer*> connectedPeers;

	map<int , unsigned int> peers2beConnected;
	bool online = false;
	ENetHost* client;
	ENetAddress address;
	
	concurrency::concurrent_queue<Data> p_packets;

	map<enet_uint32 , ENetPeer*> newPeers; // peers connected waiting for validation;
	int host();
	int join();
	
	void peerConnectRoutineHOST(ENetEvent& event);
	void peerConnectRoutineJoin(ENetEvent& event);


	void sendDataToPeer(ENetPeer* peer , union Data payload, PACKETTYPE type);

	void sendDataBroadcast(union Data payload, PACKETTYPE type);



	
	bool connectToHost(const std::string ip);

	bool connectToPeer(const std::string ip, const int id);

	void anounceNewPeer(enet_uint32 ip, int id);
	void anounceLobbyPeers(ENetPeer* newPeer);
	union Data setPeerID();
	
	void parseData(unsigned char* buffer, size_t size, ENetEvent& event , int timediff);
	void parseData(ENetPacket* packet, int timediff, ENetPeer* peer);

	void disconnect();


	
	void deletePeer(int id);

	
	startG createSGDtata();
	void sendPlayerInfo();
	void sendMySpaceShip();
	void sendOPSpaceship(int o_id , int ss);
	
	
	
	
	std::string hex_to_strip(unsigned int input);
	int hex_to_intip(unsigned int input);
	
	///JOIN FUNCTIONS

	void setmyID(setID p);
	void validatePeer(enet_uint32 ip, int id, ENetPeer* peer);


	
	int maxpeerID = 0;


	bool _host = false;

	public:

		bool changedSpaceship = false;
		
		Net(bool host);
		int run();
		
		void setOnline(bool a);
		bool getOnline();
		void addpMOVEToQueue(int o_id , float angle , float speed , float x , float y , unsigned long framecounter , bool fire_flag);
		bool isHost();
		void sendLoadedLevelMSG(int o_id);
		std::mutex& getMutex();
		void setinGame(bool ig);
		bool getinGame();
		~Net();

};
