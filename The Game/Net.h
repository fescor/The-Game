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





using namespace std;
class Net : public GameObject {
	int i = 0;


	std::mutex net_mutex;
	map<int,ENetPeer*> connectedPeers;

	map<int , unsigned int> peers2beConnected;
	bool online = false;
	ENetHost* client;
	ENetAddress address;
	
	std::queue< std::shared_ptr<Data>> p_packets;
	map<enet_uint32 , ENetPeer*> newPeers; // peers connected waiting for validation;
	int host();
	int join();
	
	void peerConnectRoutineHOST(ENetEvent& event);
	void peerConnectRoutineJoin(ENetEvent& event);


	void sendDataToPeer(ENetPeer* peer , union data payload, PACKETTYPE type);

	void sendDataBroadcast(union data payload, PACKETTYPE type);



	
	void connectToHost(const std::string ip);

	void connectToPeer(const std::string ip, const int id);

	void anounceNewPeer(enet_uint32 ip, int id);
	void anounceLobbyPeers(ENetPeer* newPeer);
	union data setPeerID();
	
	void parseData(unsigned char* buffer, size_t size, ENetEvent& event );

	void disconnect();


	
	void deletePeer(int id);
	
	
	
	
	
	std::string hex_to_strip(unsigned int input);
	int hex_to_intip(unsigned int input);
	
	///JOIN FUNCTIONS

	void setmyID(int id);
	void validatePeer(enet_uint32 ip, int id, ENetEvent & event);


	
	int maxpeerID = 0;


	bool _host = false;

	public:

		
		
		Net(bool host);
		int run();
		
		void setOnline(bool a);
		bool getOnline();
		void addpMOVEToQueue(int o_id , int angle , float speed , float x , float y , unsigned long framecounter);
		bool isHost();

		~Net();

};
