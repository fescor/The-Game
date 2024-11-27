#include "enet/enet.h"
#include "cereal/archives/binary.hpp"

struct newP 
{
	uint8_t id = 0;
	enet_uint32 ip = 0;


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(id , ip); // serialize things by passing them to the archive
	}
};

struct payload {
	int msgtype = 0;


	void* data = nullptr;

	payload() {};
	payload(void* _data, int _msgtype) : data(_data), msgtype(_msgtype) {}

	payload(payload& p) {
		msgtype = p.msgtype;
		data = p.data;
	}
	payload& operator=(payload& p) {
		msgtype = p.msgtype;
		data = p.data;
		return *this;
	}


};

enum PACKETTYPE {
	NEWPEER , 



};



struct packet
{
	PACKETTYPE type;
	union 
	{
		
		newP newpeer;
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

		}
		
	}

	packet() {};
};

