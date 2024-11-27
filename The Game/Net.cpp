#include "net.h"
#include <enet/enet.h>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <sstream>





using namespace std;

int Net::host()
{

	ENetEvent event;
	
	

	while (online) {
		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				
				//memcpy(p, buffer + 1, sizeof(newP));

				//memcpy(temp2, &event.peer->address.host, sizeof enet_uint32);
				//temp2 = &event.peer->address.host;
				
				//auto temp = pair<int, unsigned int>(maxpeerID + 1, event.peer->address.host);

				
				peers.insert(pair<int, unsigned int>(maxpeerID + 1, hex_to_intip(event.peer->address.host)));
				anounceNewPeer(event.peer, event.peer->address.host, maxpeerID + 1);

				m_state->createPlayer(maxpeerID + 1);

				event.peer->data = m_state->connectpeer2player(); //afto simenei oti to peer pou ekane join einai conected me to m_id toy player pou eftiaksa (peer.data = player.o_id)
				maxpeerID++;


				cout << peers.begin()->first;
				
				 
				cout << hex_to_strip(event.peer->address.host);
				

				sendData(event.peer, "i_see_you");
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				/*
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);
				*/
				parseData(event.packet->data, event.packet->dataLength);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("\n %d disconnected.\n", *(int*)event.peer->data);

				deletePeer(*(int*)event.peer->data);

				
				
				


				/* Reset the peer's client information. */
				event.peer->data = NULL;

				


			}
		}
		


	}
	m_state->setOnline(false, false);
	enet_host_destroy(client);
	return 0;
}

int Net::join()
{	
	ENetEvent event;
	connectToPeer("10.124.68.24");
	sendData("this_is_some_data");
	
	while (online) {
		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port);
				//// TODO otan kaneis kapios connect prepei na to sindeo to enetpeer* me kapio peer apto to map mou (tha mou exei kanei broa)
				newPeers.push_back(hex_to_strip(event.peer->address.host));



				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->data,
					event.channelID);

				parseData(event.packet->data, event.packet->dataLength);
				



				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:/// TO DO HANDLE DC
				printf("%s disconnected.\n", event.peer->data);

				/* Reset the peer's client information. */

				event.peer->data = NULL;


			}
		}


	}
	
	m_state->setOnline(false, false); //isos den xriazete


	enet_host_destroy(client);
	return 0;

	

}

void Net::sendData(ENetPeer* peer, const char* data) ///overloading sendData ////
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet); //second parameter is channel id i want to send the packet through
	

}

void Net::sendData(ENetPeer* peer, const unsigned int data)
{
	ENetPacket* packet = enet_packet_create(&data, sizeof(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);

}

void Net::sendData(const char* data)
{
	ENetPacket* packet = enet_packet_create(&data, sizeof(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(client, 0, packet);

}

void Net::sendData(const unsigned int data)
{
	ENetPacket* packet = enet_packet_create(&data, sizeof(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(client, 0, packet);

}

void Net::sendData(void* data) const
{

	ENetPacket* packet = enet_packet_create(data, sizeof(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(client, 0, packet);

}





int Net::run() {


	if (enet_initialize() != 0) {
		cout << " Error on Enet Init";

		return EXIT_FAILURE;



	}
	atexit(enet_deinitialize);

	client = enet_host_create(&address, 3, 1, 0, 0);

	if (client == nullptr) {
		cout << " Error creating client";
		m_state->setOnline(false, false);
		return EXIT_FAILURE;
	}
	online = true;

	if (_host) {
		host();
	}
	else {
		join();
	}

	return 0;


}
std::string Net::hex_to_strip(unsigned int input)        //////// O XRISTOS KAI H PANAGIA
{
	stringstream my_ss;					//convert decimal to hex
	my_ss << hex << input;
	std::string res = my_ss.str();

	std::string output = res; //// reverse hex
	for (size_t i = 0, middle = output.size() / 2, size = output.size(); i < middle; i += 2)
	{
		std::swap(output[i], output[size - i - 2]);
		std::swap(output[i + 1], output[size - i - 1]);
	}
	unsigned int val = stoi(output, 0, 16);
	std::string ip = std::to_string((val & 0xFF000000) >> 24) + '.' + std::to_string((val & 0x00FF0000) >> 16) + '.' + std::to_string((val & 0x0000FF00) >> 8) + '.' + std::to_string((val & 0x000000FF));
	
	return ip;
}

int Net::hex_to_intip(unsigned int input)        //////// O XRISTOS KAI H PANAGIA
{
	stringstream my_ss;					//convert decimal to hex
	my_ss << hex << input;
	std::string res = my_ss.str();

	std::string output = res; //// reverse hex
	for (size_t i = 0, middle = output.size() / 2, size = output.size(); i < middle; i += 2)
	{
		std::swap(output[i], output[size - i - 2]);
		std::swap(output[i + 1], output[size - i - 1]);
	}
	unsigned int val = stoi(output, 0, 16);
	return val;
}


void Net::connectToPeer(const std::string ip) // this is called when i connect to host
{
	ENetPeer* peer;																	// peer defins where we connect to so one peer for each player should be implemented here;
	ENetAddress addressPeer;														// for every peer need to set the this method
	ENetEvent event;
	enet_address_set_host(&addressPeer, ip.c_str());
	addressPeer.port = 7777;
	
	

	peer = enet_host_connect(client, &addressPeer, 1, 0);
	if (peer == nullptr) {   // vale diaforetika returns gia kathe enedexomeno
		cout << " peer is not available ";
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return ;
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {


		cout << "conection succeded ";
		online = true;
		
		peers.insert(pair<int, unsigned int>(0, peer->address.host));

		m_state->createPlayer(0);

		event.peer->data = m_state->connectpeer2player();

		return;
	}
	else {
		cout << "conection failed"; // afto den to thelw na vgenei apthn SINARTHSH EPIDI DEN KATAFERA NA KANW CONNECT
		enet_peer_reset(peer);

		//here need some return
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return ;

	}
	
}

void Net::connectToPeer(const std::string ip, int id)
{
	ENetPeer* peer;																	// peer defins where we connect to so one peer for each player should be implemented here;
	ENetAddress addressPeer;														// for every peer need to set the this method
	ENetEvent event;
	enet_address_set_host(&addressPeer, ip.c_str());
	addressPeer.port = 7777;


	peer = enet_host_connect(client, &addressPeer, 1, 0);
	if (peer == nullptr) {   // vale diaforetika returns gia kathe enedexomeno
		cout << " peer is not available ";
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return;
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {


		cout << "conection succeded ";
		online = true;
		
		peers.insert(pair<int, unsigned int>(id, peer->address.host));

		m_state->createPlayer(id);

		event.peer->data = m_state->connectpeer2player();

		return;

	}
	else {
		cout << "conection failed"; // afto den to thelw na vgenei apthn SINARTHSH EPIDI DEN KATAFERA NA KANW CONNECT
		enet_peer_reset(peer);

		//here need some return
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return;

	}
}

void Net::anounceNewPeer(ENetPeer* newPeer, enet_uint32 ip, int id)
{
	
	newP* p = new newP();
	p->id = id;
	p->ip = ip;
	sendData(p);
	//createpayload(p, 1);
	
	

}

void Net::parseData(unsigned char* buffer, size_t size)
{
	
	std::stringstream ss = std::stringstream(std::string((char*)buffer));
	packet  p;

	{
		cereal::BinaryInputArchive iarchive(ss); // Create an input archive

		
		iarchive(p); // Read the data from the archive
	}

	cout << "msg type : " << p.type << " peer id : " << p.newpeer.id << " peer ip : " << p.newpeer.ip << endl;

	



}

void Net::deletePeer(int id)
{
	for (auto iter = peers.begin(); iter != peers.end(); iter++) {
		if (iter->first == id) {
			m_state->deletePlayer(iter->first);
			peers.erase(iter);
			return;
			
		}
	}

}

void Net::validatePeer(enet_uint32 ip, int id) // this should be called when host announces a new peer so i validate it 
{
	/// TODO
	std::string ipStr = hex_to_strip(ip);

	for (auto iter = newPeers.begin(); iter != newPeers.end(); iter++) {
		if (*iter == ipStr) {
			peers.insert(pair<int, unsigned int >(id, ip));
			newPeers.erase(iter);
		}
	}






}

void Net::serializeData(newP structure, int datatype)
{

	packet p;
	p.type = NEWPEER;
	p.newpeer = structure;
	

	std::stringstream ss; // any stream can be used
	std::ios::binary;
	{
		cereal::PortableBinaryOutputArchive oarchive(ss); // Create an output archive		
		oarchive(p);// Write the data to the archive
		
	} // archive goes out of scope, ensuring all contents are flushed
	

	std::string data = ss.str();
	sendData(data.c_str());
	

}

void Net::setOnline(bool a)
{
	online = a;
}

bool Net::getOnline()
{
	return online;
}

Net::Net(bool host) {
		
	_host = host;
	address.host = ENET_HOST_ANY;
	address.port = 7777;
	client = nullptr;
	


}

Net::~Net()
{
	

}
