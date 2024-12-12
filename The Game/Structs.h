#pragma once
#include "enet/enet.h"
#include "cereal/archives/binary.hpp"

struct pMOVE
{
	int id = 0;
	float angle = .0;
	float speed = .0;
	float x = .0;
	float y = .0;
	unsigned long fc = 0;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id, angle , speed , x , y , fc); // serialize things by passing them to the archive
	}



};
struct startG {

	float timeinfo = 0.0f;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(timeinfo); // serialize things by passing them to the archive
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

struct setID
{
	int id = 0;
	


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id); // serialize things by passing them to the archive
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



enum PACKETTYPE {
	NEWPEER ,
	SETID,
	LOOBYPEER,
	DISCONNECT,
	PMOVE, 
	START_GAME,


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

				

		}
		
	}

	packet() {};
};

typedef union data {


	PEER newp{};

	setID setid;

	dc idc;

	pMOVE pmove;

	startG strtg;

	



}Data;