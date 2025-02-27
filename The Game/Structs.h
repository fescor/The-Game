#pragma once
#include "enet/enet.h"
#include "cereal/archives/binary.hpp"
#include <algorithm>

struct pMOVE
{
	int id = 0;
	float angle = 0;
	float speed = .0;
	float x = .0;
	float y = .0;
	unsigned long fc = 0;
	bool fire = false;
	

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id, angle , speed , x , y , fc , fire); // serialize things by passing them to the archive
	}
	//gege


};
struct voice
{
	float chunk;
	int id;
	int counter;




	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(chunk , id , counter); // serialize things by passing them to the archive
	}

};


struct playerInfo
{
	int spaceshipID = 0;
	int id = 0;


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(spaceshipID, id); // serialize things by passing them to the archive
	}
};
struct Loaded {

	int id = 0;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id); 
	}




};
struct startG {

	float timeinfo = 0.0f;

	int map_x[35] = {};
	int map_y[35] = {};
	int planet_lvl[35] = {};
	int planet_oid[35] = {};
	int token_x[12] = {};
	int token_y[12] = {};
	char token_type[12] = {};
	int token_oid[12] = {};
	float posPlayer[4][3];// [id][m_pos_x][m_pos_y]





	template<class Archive>
	void serialize(Archive& archive)
	{
		archive( timeinfo , map_x, map_y, planet_lvl, planet_oid, token_x, token_y, token_type, token_oid , posPlayer); // serialize things by passing them to the archive
	}
	



};





struct PEER
{
	int id = 0;
	enet_uint32 ip = 0;


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id , ip); // serialize things by passing them to the archive
	}
	PEER() {};
};

struct  audiodata
{
	int playerid; //who send the data
	//std::vector<float> audioData; 
	float audioData[512];
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(audioData, playerid); // serialize things by passing them to the archive
	}
};
struct setID
{
	int id = 0;
	int availablespaceships[4] = {};


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id , availablespaceships); // serialize things by passing them to the archive
	}

};
struct dc
{
	int idc;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(idc); // serialize things by passing them to the archive
	}

};
struct spaceShip
{
	int o_id;
	int spaceShip;
	int hostSpaceship;
	int availableS[4] = {};
	int lobbyPlayersSS[4] = { };
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(o_id , spaceShip, hostSpaceship, availableS , lobbyPlayersSS); // serialize things by passing them to the archive
	}

};



struct levelPacketDelete
{

	int o_id;
	int type;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(o_id , type); // serialize things by passing them to the archive
	}

};




enum PACKETTYPE {
	NEWPEER,
	SETID,
	LOOBYPEER,
	DISCONNECT,
	PMOVE,
	START_GAME,
	LOADED_LEVEL,
	MAP_INFO,
	PLAYER_INFO,
	VOICE_DATA,
	SPACE_SHIP,
	LEVEL_PACKET,





};

enum LEVEL_PACKET_TYPE 
{
	PLANET_DESTROYED,
	TOKEN_CONSUME,
	PLAYER_DEAD,
	NEW_SIEGE,
	COMPLETED_LEVEL,
};



struct packet
{
	PACKETTYPE type;
	union 
	{
		
		PEER newpeer;
		setID setid;
		dc idc;
		pMOVE pmove;
		startG strtg;
		Loaded loaded_level;
		playerInfo pi;
		audiodata ad;

		voice  vc;
		spaceShip ss;
		levelPacketDelete lvlp;

		



	};
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(type);// serialize things by passing them to the archive
		switch (type) {
			case NEWPEER:
				archive(newpeer);
				break;
			case SETID:
				archive(setid);
				break;
			case LOOBYPEER:
				archive(newpeer);
				break;
			case DISCONNECT:
				archive(idc);
				break;
			case PMOVE:
				archive(pmove);
				break;
			case START_GAME:
				archive(strtg);
				break;
			case LOADED_LEVEL:
				archive(loaded_level);
				break;
			case PLAYER_INFO:
				archive(pi);
				break;
			case VOICE_DATA:
				archive(ad);
				break;
			case SPACE_SHIP:
				archive(ss);
				break;
			case LEVEL_PACKET:
				archive(lvlp);
				break;



				

				

		}
		
	}

	packet() {};
};

union Data 
{


	PEER newp{};

	setID setid;

	dc idc;

	pMOVE pmove;

	startG strtg;

	Loaded loaded_level;

	playerInfo pi;

	voice vc;

	spaceShip ss;
	
	levelPacketDelete lvlp;

	
	audiodata ad;
	

};
