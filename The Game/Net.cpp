#include "net.h"
#include <string>
#include <winsock2.h>
#include <sstream>
#include "Player.h"


#define PC_IP "10.124.68.23"
#define LAPTOP_IP "10.124.68.24"
#define AGLOU_PC "10.124.68.113"
#define BABUSUS "10.124.68.39"
//TODO :  fix on join when unable to join to host wird stuff happen



using namespace std;

int Net::host()
{
	
	ENetEvent event;
	

	while (online) {
		if (p_packets.size()) {


			net_mutex.lock();
			Data p = *(p_packets.front());
			p_packets.pop();
			net_mutex.unlock();

			sendDataBroadcast(p, PMOVE);
			enet_host_flush(client);
			

		}


		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:

				peerConnectRoutineHOST(event);
				break;

			case ENET_EVENT_TYPE_RECEIVE:

				parseData(event.packet->data, event.packet->dataLength, event);
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				
				cout << "enet_peer disconected" << endl;
				event.peer->data = nullptr;

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
	connectToHost(LAPTOP_IP);
	
	
	
	while (online) {
		if (p_packets.size()) {


			net_mutex.lock();
			Data p = *(p_packets.front());
			p_packets.pop();
			net_mutex.unlock();

			sendDataBroadcast(p, PMOVE);
			enet_host_flush(client);


		}
		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:

				peerConnectRoutineJoin(event);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				
				parseData(event.packet->data, event.packet->dataLength, event);
				enet_packet_destroy(event.packet);
				
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				
				cout << "enet peer disconnected " << endl;
				event.peer->data = nullptr;


			}
		}
		if (!m_state->getOnline()) {
			
			disconnect();
			online = false;
			
			break;

		}
		


	}
	
	

	
	m_state->setOnline(false, false); //isos den xriazete


	enet_host_destroy(client);
	return 0;

	

}

void Net::peerConnectRoutineHOST(ENetEvent& event)
{

	sendDataToPeer(event.peer, setPeerID(), SETID); //here i send the peer that connected his online id 
	
	anounceLobbyPeers(event.peer);


	anounceNewPeer(event.peer->address.host, maxpeerID); // anounce to the lobby that someone connected


	//connectedPeers[maxpeerID] = shared_ptr<ENetPeer>(new ENetPeer);
	connectedPeers[maxpeerID] = event.peer;

	//*(connectedPeers[maxpeerID]) = *(event.peer);

	//peers.insert(pair<int, unsigned int>(maxpeerID,event.peer->address.host));// store the new peeer as a pair of (id , ip) 
	

	m_state->createPlayer(maxpeerID);// create a player instance for the newpeer

	event.peer->data = m_state->connectpeer2player(maxpeerID); // save the o_id of the new peer at his (enet) data field, every time i have a packet i can id it by the data field
	connectedPeers[maxpeerID]->data = m_state->connectpeer2player(maxpeerID);
	


	cout << "A new Player connected with ID :  " + std::to_string(maxpeerID) + " IP : " + (hex_to_strip(event.peer->address.host)) << endl;


	



}

void Net::peerConnectRoutineJoin(ENetEvent& event)
{

	

	for (auto peer : peers2beConnected) {
		if (peer.second == event.peer->address.host) {


			connectedPeers[peer.first] = event.peer;
			
			//*(connectedPeers[peer.first]) = *(event.peer);
			m_state->createPlayer(peer.first);

			
			connectedPeers[peer.first]->data = m_state->connectpeer2player(peer.first);
			peers2beConnected.erase(peer.first);
			

			cout << "A new Player connected with ID :  " + std::to_string(peer.first) + " IP : " + (hex_to_strip(event.peer->address.host)) << endl;
			return;



		}


	}
	newPeers[event.peer->address.host] =  event.peer;
	return;






	

}

void Net::sendDataToPeer(ENetPeer* peer, union data payload, PACKETTYPE type) 
{

	packet p;
	switch (type)
	{
	case NEWPEER:
		p.type = type;
		p.newpeer = payload.newp;
		break;
	case SETID:
		p.type = type;
		p.setid = payload.setid;
		break;
	case LOOBYPEER:
		p.type = type;
		p.newpeer = payload.newp;
		break;
	

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
		p.type = type;
		p.newpeer = payload.newp;
		break;
	case DISCONNECT:
		p.type = type;
		p.idc = payload.idc;
		break;
	case PMOVE:
		p.type = type;
		p.pmove = payload.pmove;
		break;

	}

	std::stringstream ss; // any stream can be used
	std::ios::binary;
	{
		cereal::PortableBinaryOutputArchive oarchive(ss); // Create an output archive		
		oarchive(p);// Write the data to the archive

	} // archive goes out of scope, ensuring all contents are flushed


	std::string data = ss.str();
	
	ENetPacket* packet = enet_packet_create(data.c_str(), data.length() + 1, ENET_PACKET_FLAG_RELIABLE);
	//enet_host_broadcast(client, 0, packet);

	
	for (auto peer : connectedPeers)
	{
		enet_peer_send(peer.second, 0, packet);		
	}
	
	
	

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


void Net::connectToHost(const std::string ip) // this is called when i connect to host
{
	
	ENetPeer* peer;																	// peer defins where we connect to so one peer for each player should be implemented here;
	ENetAddress addressPeer;														// for every peer need to set the this method
	ENetEvent event;
	enet_address_set_host(&addressPeer, ip.c_str());
	addressPeer.port = 7777;

	

	peer = enet_host_connect(client, &addressPeer, 1, 0);
	if (peer == nullptr) {  
		cout << " peer is not available ";
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return ;
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {


		cout << "conection succeded " << endl;
		
		
		//peers.insert(pair<int, unsigned int>(0, peer->address.host));
		
		connectedPeers[0] = event.peer;
		//*(connectedPeers[0]) = *(event.peer);


		m_state->createPlayer(0);

		event.peer->data = m_state->connectpeer2player(0);
		connectedPeers[0]->data = m_state->connectpeer2player(0);

		return;
	}
	else {
		cout << "conection failed" << endl; // afto den to thelw na vgenei apthn SINARTHSH EPIDI DEN KATAFERA NA KANW CONNECT
		enet_peer_reset(peer);

		//here need some return
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return ;

	}
	
}

void Net::connectToPeer(const std::string ip, int id)// this should be called when i get a NEWP packet
{
	ENetPeer* peer;																	// peer defins where we connect to so one peer for each player should be implemented here;
	ENetAddress addressPeer;														// for every peer need to set the this method
	ENetEvent event;
	enet_address_set_host(&addressPeer, ip.c_str());
	addressPeer.port = 7777;


	peer = enet_host_connect(client, &addressPeer, 1, 0);
	if (peer == nullptr) {  
		cout << " peer is not available ";
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return;
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {


		cout << "conection succeded ";
		
		
		//peers.insert(pair<int, unsigned int>(id, peer->address.host));

		connectedPeers[id] = event.peer;
		//*(connectedPeers[id]) = *(event.peer);
		
		m_state->createPlayer(id);

		event.peer->data = m_state->connectpeer2player(id);
		connectedPeers[id]->data = m_state->connectpeer2player(id);

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

void Net::anounceNewPeer(enet_uint32 ip, int id)
{
	
	PEER p;
	p.id = id;
	p.ip = ip;
	union data payload;
	payload.newp = p;
	
	
	sendDataBroadcast(payload , NEWPEER );// here the new peer is gonna get the data from the broadcast it should ignore it todo on join function.

}

void Net::anounceLobbyPeers(ENetPeer* newPeer)
{
	PEER p;
	union data payload;
	

	for (auto peer : connectedPeers) {	//TODO: tell the new peer who is already on the lobby 
		p.id = peer.first;
		p.ip = peer.second->address.host;
		payload.newp = p;
		sendDataToPeer(newPeer, payload, LOOBYPEER);
	}

}

union data Net::setPeerID()
{

	maxpeerID++;

	setID setid;
	setid.id = maxpeerID;
	union data payload;
	payload.setid = setid;

	
	return payload;



}

void Net::parseData(unsigned char* buffer, size_t size , ENetEvent & event)
{


	std::stringstream ss = std::stringstream(std::string((char*)buffer, size));
	

	
	packet  p;

	{
		cereal::PortableBinaryInputArchive iarchive(ss); // Create an input archive

		
		iarchive(p); // Read the data from the archive
	}

	switch (p.type)
	{
	case NEWPEER:

		if (p.newpeer.id == *(m_state->getPlayer()->geto_id())) { break;}
		cout << "msg type : " << p.type << endl << " peer id : " << p.newpeer.id << endl << " peer ip : " << hex_to_strip(p.newpeer.ip) << endl;
		connectToPeer(hex_to_strip(p.newpeer.ip) , p.newpeer.id);


		break;
	case SETID:

		setmyID(p.setid.id);
		break;
	case LOOBYPEER:// here i want to store the info of the peer in newPeers and when the conection comes validate it(not here validation) 

		validatePeer(p.newpeer.ip, p.newpeer.id, event);
		break;
	case DISCONNECT:

		enet_peer_disconnect(event.peer , 0);
		deletePeer(p.idc.idc);

		break;
	case PMOVE:

		//m_state->getp_movePacket(p.pmove.id, p.pmove);
		cout << "this is a broadcast" << endl;
		break;
	
		

	}

	

	



}

void Net::disconnect()///telling everyone i am disconecting
{

	union data payload;
	dc dc;
	dc.idc = *(int*)m_state->getPlayer()->geto_id();
	payload.idc = dc;


	
	//sendDataToPeer(p, payload, DISCONNECT);
	sendDataBroadcast(payload, DISCONNECT);
	enet_host_flush(client);
	
	
}



void Net::deletePeer(int id)
{

	m_state->deletePlayer(id);
	connectedPeers.erase(connectedPeers.find(id));
	cout << "Player with ID : " + std::to_string(id) + " disconected " << endl;



}

void Net::setmyID(int id)
{

	m_state->getPlayer()->seto_id(id);
	
	cout << "HOST SET MY ONLINE ID TO : " + to_string(*(int*)m_state->getPlayer()->geto_id())<< endl;
	

}

void Net::validatePeer(enet_uint32 ip, int id, ENetEvent & event) // this should be called when host announces a new peer so i validate it 
{
	
	if (newPeers.find(ip) != newPeers.end()) {
		
		connectedPeers[id] = newPeers.find(ip)->second;
		newPeers.erase(ip);
		m_state->createPlayer(id);

		connectedPeers[id]->data = m_state->connectpeer2player(id);
		cout << "A new Player connected with ID :  " + std::to_string(id) + " IP : " + (hex_to_strip(event.peer->address.host)) << endl;
		return;


	}

	
	peers2beConnected.insert(pair<int, unsigned int>(id, ip));

}


void Net::setOnline(bool a)
{
	online = a;
}

bool Net::getOnline()
{
	return online;
}

void Net::addpMOVEToQueue(int o_id, float angle, float speed, float x, float y)
{
	pMOVE packet;
	packet.id = o_id;
	packet.angle = angle;
	packet.speed = speed;
	packet.x = x;
	packet.y = y;

	
	std::shared_ptr<Data> payload(new Data);

	payload->pmove = packet;
	


	net_mutex.lock();
	p_packets.push(payload);
	net_mutex.unlock();


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
