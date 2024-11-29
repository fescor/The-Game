#include "enet/enet.h"
#include "cereal/archives/binary.hpp"

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



enum PACKETTYPE {
	NEWPEER ,
	SETID,
	LOOBYPEER,



};



struct packet
{
	PACKETTYPE type;
	union 
	{
		
		PEER newpeer;
		setID setid;
		//vale edw ta alla structs(paketa)


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
				

		}
		
	}

	packet() {};
};

union data {


	PEER newp{};

	setID setid;





};