#pragma once
#include <enet/enet.h>
#include <vector>
#include "GameState.h"
#include "GameObject.h"
#include <list>
#include <map>
#include <fstream>
#include "Structs.h"
#include "cereal/archives/binary.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "Player.h"




using namespace std;
class Net : public GameObject {

	std::map<int , unsigned int> peers;
	bool online = false;
	ENetHost* client;
	ENetAddress address;
	
	std::list < std::string> newPeers; // peers connected waiting for validation;
	int host();
	int join();
	
	void connectRoutine(ENetEvent& event);


	void sendDataToPeer(ENetPeer* peer , union data payload, PACKETTYPE type);

	void sendDataBroadcast(union data payload, PACKETTYPE type);



	
	void connectToPeer(const std::string ip);
	void connectToPeer(const std::string ip, const int id);

	void anounceNewPeer(ENetPeer * peer , enet_uint32 ip, int id);
	
	void parseData(unsigned char* buffer, size_t size);
	
	void deletePeer(int id);
	
	
	
	
	
	std::string hex_to_strip(unsigned int input);
	int hex_to_intip(unsigned int input);
	
	///JOIN FUNCTIONS

	void setmyID(int id);
	void validatePeer(enet_uint32 ip, int id);


	
	int maxpeerID = 0;


	bool _host = false;

	public:
		Net(bool host);
		int run();
		
		void setOnline(bool a);
		bool getOnline();

		~Net();

};
