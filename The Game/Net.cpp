#include "net.h"
#include <enet/enet.h>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <sstream>

#define PC_IP "10.124.68.23"
#define LAPTOP_IP "10.124.68.24"




using namespace std;

int Net::host()
{
	ENetEvent event;

	while (online) {
		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				cout << "A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port;
				
				connectRoutine(event);

				break;

			case ENET_EVENT_TYPE_RECEIVE:

				parseData(event.packet->data, event.packet->dataLength);
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:

				cout << *(int*)event.peer->data, "\n disconnected.\n";

				deletePeer(*(int*)event.peer->data);
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
	connectToPeer(LAPTOP_IP);
	
	
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

void Net::connectRoutine(ENetEvent& event)
{
	anounceNewPeer(event.peer, event.peer->address.host, maxpeerID + 1);

	peers.insert(pair<int, unsigned int>(maxpeerID + 1, hex_to_intip(event.peer->address.host)));
	

	m_state->createPlayer(maxpeerID + 1);

	event.peer->data = m_state->connectpeer2player(); //afto simenei oti to peer pou ekane join einai conected me to m_id toy player pou eftiaksa (peer.data = player.o_id)
	maxpeerID++;


	cout << peers.begin()->first<< endl;


	cout << hex_to_strip(event.peer->address.host) << endl;



}

void Net::sendDataToPeer(ENetPeer* peer, union data payload, PACKETTYPE type) ///overloading sendData ////
{

	packet p;
	switch (type)
	{
	case NEWPEER:
		p.type = NEWPEER;
		p.newpeer = payload.newp;


	}


	std::stringstream ss; // any stream can be used
	std::ios::binary;
	{
		cereal::PortableBinaryOutputArchive oarchive(ss); // Create an output archive		
		oarchive(p);// Write the data to the archive

	} // archive goes out of scope, ensuring all contents are flushed


	std::string data = ss.str();

	ENetPacket* packet = enet_packet_create(data.c_str(), data.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet); //second parameter is channel id i want to send the packet through

	

}


void Net::sendDataBroadcast(union data payload, PACKETTYPE type)
{
	packet p;
	switch (type)
	{
	case NEWPEER:
		p.type = NEWPEER;
		p.newpeer = payload.newp;


	}

	std::stringstream ss; // any stream can be used
	std::ios::binary;
	{
		cereal::PortableBinaryOutputArchive oarchive(ss); // Create an output archive		
		oarchive(p);// Write the data to the archive

	} // archive goes out of scope, ensuring all contents are flushed


	std::string data = ss.str();
	
	ENetPacket* packet = enet_packet_create(data.c_str(), data.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(client, 0, packet);
	cout << "sending some data " << endl;

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
	
	newP p;
	p.id = id;
	p.ip = ip;
	union data payload;
	payload.newp = p;
	
	
	sendDataBroadcast(payload , NEWPEER );// here the new peer is gonna get the data from the broadcast it should ignore it todo on join function.
	for (auto peer : peers) {	//TODO: tell the new peer who is already on the lobby 
		p.id = peer.first;
		p.ip = peer.second;
		payload.newp = p;
		sendDataToPeer(newPeer, payload, NEWPEER);
	}
}

void Net::parseData(unsigned char* buffer, size_t size)
{


	std::stringstream ss = std::stringstream(std::string((char*)buffer, size));
	

	
	packet  p;

	{
		cereal::PortableBinaryInputArchive iarchive(ss); // Create an input archive

		
		iarchive(p); // Read the data from the archive
	}

	cout << "msg type : " << p.type << endl << " peer id : " << p.newpeer.id << endl << " peer ip : " << hex_to_strip(p.newpeer.ip) << endl;

	



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
	
	std::string ipStr = hex_to_strip(ip);

	for (auto iter = newPeers.begin(); iter != newPeers.end(); iter++) {
		if (*iter == ipStr) {
			peers.insert(pair<int, unsigned int >(id, ip));
			newPeers.erase(iter);
		}
	}






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
