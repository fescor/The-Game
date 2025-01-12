#include "net.h"
#include <string>
#include <winsock2.h>
#include <sstream>
#include "Player.h"



#define PC_IP "10.124.68.23"
#define LAPTOP_IP "10.124.68.24"
#define AGLOU_PC "10.124.68.113"
#define BABUSUS "10.124.68.39"
//TODO :  wierd stuff happening on movment from peer(connecting to host) 

//TODO : bug when on join function i try to join a game again (havent restarded the client it is stuck and wont load the game)


using namespace std;

int Net::host()
{
	float timeB = graphics::getGlobalTime();
	float timeDIF = 0.0f;
	ENetEvent event;
	

	while (online) {
		timeDIF = graphics::getGlobalTime() - timeB ;
		timeB = graphics::getGlobalTime();


		//cout << timeDIF << endl;

		if (!p_packets.empty()) {


			
			//m_state->getMutex().lock();
			Data p;
			p_packets.try_pop(p);
			//p_packets.pop();
			//m_state->getMutex().unlock();
			

			sendDataBroadcast(p, PMOVE);
			enet_host_flush(client);
			

		}

		if (m_state->loadedLevel() && !inGame) {
			
			Data d;
			d.strtg = m_state->getMapInfo();
			

			sendDataBroadcast(d , START_GAME);



			inGame = true;


		}

		if (!ad_packets.empty()) {

			//m_state->getMutex().lock();
			Data ad;
			ad_packets.try_pop(ad);
			//p_packets.pop();
			//m_state->getMutex().unlock();


			sendDataBroadcast(ad, VOICE_DATA);
			enet_host_flush(client);

		}
		
		


		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:

				peerConnectRoutineHOST(event);
				break;

			case ENET_EVENT_TYPE_RECEIVE:

				parseData(event.packet->data, event.packet->dataLength, event, timeDIF);
				enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				
				cout << "enet_peer disconected" << endl;
				//stop playback stream when a peer disconnects
				bool streamflag = false;
				m_state->PlaybackStreamOpen(streamflag);

				if (event.peer->data != nullptr) {
					deletePeer(*((int*)event.peer->data));
					event.peer->data = nullptr;
				}
				
			}
		}
		else {
			if (enet_host_check_events(client, &event) > 0) {
				parseData(event.packet->data, event.packet->dataLength, event, timeDIF);
			}
			/*
			for (auto iter : connectedPeers) {
				parseData(enet_peer_receive(iter.second, 0), timeDIF, iter.second);

			}
			*/

		}
		
	
	}
	

	m_state->setOnline(false, false);
	enet_host_destroy(client);
	return 0;
}

int Net::join()
{	
	ENetEvent event;
	if (!connectToHost("192.168.68.105")) { online = false; } // laptopip : 192.168.1.10 pcip : 192.168.68.105 
	float timeB = graphics::getGlobalTime();
	float timeDIF = 0.0f;
	
	
	while (online) {
		timeDIF = graphics::getGlobalTime() - timeB;
		timeB = graphics::getGlobalTime();

		if (!p_packets.empty()) {


			//m_state->getMutex().lock();
			Data p;
			p_packets.try_pop(p);
			//p_packets.pop();
			//m_state->getMutex().unlock();

			sendDataBroadcast(p, PMOVE);
			enet_host_flush(client);


		}

		if (!ad_packets.empty()) {	
			Data ad;
			ad_packets.try_pop(ad);

			sendDataBroadcast(ad, VOICE_DATA);
			enet_host_flush(client);

		}


		if (enet_host_service(client, &event, 0) > 0) {
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:

				peerConnectRoutineJoin(event);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				
				parseData(event.packet->data, event.packet->dataLength, event, timeDIF);
				enet_packet_destroy(event.packet);
				
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				
				cout << "enet peer disconnected " << endl;
				//stop playback stream when a peer disconnects
				bool streamflag = false;
				m_state->PlaybackStreamOpen(streamflag);

				if (event.peer->data != nullptr) {
					deletePeer(*((int*)event.peer->data));
					event.peer->data = nullptr;
				}
				
				


			}
			
		}
		else {
			if (enet_host_check_events(client, &event) > 0) {
				parseData(event.packet->data, event.packet->dataLength, event, timeDIF);
			}
			/*
			for (auto iter : connectedPeers) {
				parseData(enet_peer_receive(iter.second, 0), timeDIF, iter.second);

			}
			*/

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
	


	//cout << "A new Player connected with ID :  " + std::to_string(maxpeerID) + " IP : " + (hex_to_strip(event.peer->address.host)) << endl;
	//open playback stream when a new peer connects
	bool streamflag = true;
	m_state->PlaybackStreamOpen(streamflag);



	



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
			//open playback stream when a new peer connects
			bool streamflag = true;
			m_state->PlaybackStreamOpen(streamflag);
			return;



		}


	}
	newPeers[event.peer->address.host] =  event.peer;
	return;

}

void Net::sendDataToPeer(ENetPeer* peer, union Data payload, PACKETTYPE type) 
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
	case PLAYER_INFO:
		p.type = type;

		p.pi = payload.pi;
	

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

void Net::sendDataBroadcast(union Data payload, PACKETTYPE type)
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
	case START_GAME:
		p.type = type;
		p.strtg = payload.strtg;
		break;
	case LOADED_LEVEL:
		p.type = type;
		p.loaded_level = payload.loaded_level;
		break;
	case VOICE_DATA: 
		p.type = type;
		p.ad = payload.ad;
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
	enet_host_broadcast(client, 0, packet);


	
	
	if (type != PMOVE) {
		cout << "sending " + to_string(type) +  " MSG" << endl;
	}

	

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
	//edw ginete malakia problema
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


bool Net::connectToHost(const std::string ip) // this is called when i connect to host
{
	
	ENetPeer* peer;																	// peer defins where we connect to so one peer for each player should be implemented here;
	ENetAddress addressPeer;														// for every peer need to set the this method
	ENetEvent event;
	enet_address_set_host(&addressPeer, ip.c_str());
	addressPeer.port = 7777;

	

	peer = enet_host_connect(client, &addressPeer, 1, 0);
	if (peer == nullptr) {  
		cout << " peer is not available ";
		//m_state->setOnline(false, false);
		//enet_host_destroy(client);
		return false;
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {


		cout << "conection succeded " << endl;
			connectedPeers[0] = event.peer;
		//*(connectedPeers[0]) = *(event.peer);


		m_state->createPlayer(0);

		event.peer->data = m_state->connectpeer2player(0);
		connectedPeers[0]->data = m_state->connectpeer2player(0);
		
		//opens the playback stream when the host connects
		bool streamflag = true;
		m_state->PlaybackStreamOpen(streamflag);
		return true;
	}
	else {
		cout << "conection failed" << endl; // afto den to thelw na vgenei apthn SINARTHSH EPIDI DEN KATAFERA NA KANW CONNECT
		enet_peer_reset(peer);

		//here need some return
		//m_state->setOnline(false, false);
		//enet_host_destroy(client);
		return false;

	}
	
}

bool Net::connectToPeer(const std::string ip, int id)// this should be called when i get a NEWP packet
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
		return false;
	}
	if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {


		cout << "conection succeded ";
		
		
		//peers.insert(pair<int, unsigned int>(id, peer->address.host));

		connectedPeers[id] = event.peer;
		//*(connectedPeers[id]) = *(event.peer);
		
		m_state->createPlayer(id);

		event.peer->data = m_state->connectpeer2player(id);
		connectedPeers[id]->data = m_state->connectpeer2player(id);

		//opens the playback stream 
		bool streamflag = true;
		m_state->PlaybackStreamOpen(streamflag);

		return true;

	}
	else {
		cout << "conection failed"; // afto den to thelw na vgenei apthn SINARTHSH EPIDI DEN KATAFERA NA KANW CONNECT
		enet_peer_reset(peer);

		//here need some return
		m_state->setOnline(false, false);
		enet_host_destroy(client);
		return false;

	}
}

void Net::anounceNewPeer(enet_uint32 ip, int id)
{
	
	PEER p;
	p.id = id;
	p.ip = ip;
	union Data payload;
	payload.newp = p;
	
	
	sendDataBroadcast(payload , NEWPEER );// here the new peer is gonna get the data from the broadcast it should ignore it todo on join function.

}

void Net::anounceLobbyPeers(ENetPeer* newPeer)
{
	PEER p;
	union Data payload;
	

	for (auto peer : connectedPeers) {	//TODO: tell the new peer who is already on the lobby 
		p.id = peer.first;
		p.ip = peer.second->address.host;
		payload.newp = p;
		sendDataToPeer(newPeer, payload, LOOBYPEER);
	}

}

union Data Net::setPeerID()
{

	maxpeerID++;

	setID setid;
	setid.id = maxpeerID;
	union Data payload;
	payload.setid = setid;

	
	return payload;



}

void Net::parseData(unsigned char* buffer, size_t size , ENetEvent & event, int timeDIF)
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

		validatePeer(p.newpeer.ip, p.newpeer.id, event.peer);
		break;
	case DISCONNECT:

		enet_peer_disconnect(event.peer , 0);
		deletePeer(p.idc.idc);
		event.peer->data = nullptr;

		break;
	case PMOVE:
		if (_host = true) {


		}
		m_state->insertOPlayersPmove(p.pmove);
		/*
		{
			std::string s = "TIME BETWEEN ONLINE FRAMES :" + std::to_string(timeDIF) + "\n" +
				"RECIEVED MOVE PACKET WITH ID : " + std::to_string(p.pmove.fc) + "\n" +
				"AT FRAME " + to_string(m_state->framecounter) + "\n" +
				"AT : " + std::to_string(graphics::getGlobalTime()) + "\n";
			cout << s;
		}
		*/

		
		break;
	case START_GAME:

		m_state->setMapData(p.strtg);

		if (!_host) {
			m_state->setStatus('L');

		}

		break;

	case LOADED_LEVEL:

		m_state->playerLoadedLevel();
		break;
	case VOICE_DATA:
		std::cout << "elava ta arxeia" << std::endl;
		m_state->sendToPlayback(p.ad);
		break;

	}


}

void Net::parseData(ENetPacket* net_packet, int timeDIF, ENetPeer* peer)
{

	if (net_packet == NULL) { return ; }


	std::stringstream ss = std::stringstream(std::string((char*)net_packet->data, net_packet->dataLength));


	
	packet  p;

	{
		cereal::PortableBinaryInputArchive iarchive(ss); // Create an input archive


		iarchive(p); // Read the data from the archive
	}

	switch (p.type)
	{
	case NEWPEER:

		if (p.newpeer.id == *(m_state->getPlayer()->geto_id())) { break; }
		cout << "msg type : " << p.type << endl << " peer id : " << p.newpeer.id << endl << " peer ip : " << hex_to_strip(p.newpeer.ip) << endl;
		connectToPeer(hex_to_strip(p.newpeer.ip), p.newpeer.id);


		break;
	case SETID:

		setmyID(p.setid.id);



		break;
	case LOOBYPEER:// here i want to store the info of the peer in newPeers and when the conection comes validate it(not here validation) 

		validatePeer(p.newpeer.ip, p.newpeer.id, peer);
		break;
	case DISCONNECT:

		enet_peer_disconnect(peer, 0);
		deletePeer(p.idc.idc);
		peer->data = nullptr;

		break;
	case PMOVE:
		if (_host = true) {


		}
		m_state->insertOPlayersPmove(p.pmove);
		{
			std::string s = "TIME BETWEEN ONLINE FRAMES :" + std::to_string(timeDIF) + "\n" +
				"RECIEVED MOVE PACKET WITH ID : " + std::to_string(p.pmove.fc) + "\n" +
				"AT FRAME " + to_string(m_state->framecounter) + "\n" +
				"AT : " + std::to_string(graphics::getGlobalTime()) + "\n";
			cout << s;
		}


		break;
	case START_GAME:

		m_state->setMapData(p.strtg);

		if (!_host) {
			m_state->setStatus('L');

		}

		break;

	case LOADED_LEVEL:

		m_state->playerLoadedLevel();
		break;

	}

}

void Net::disconnect()///telling everyone i am disconecting
{

	union Data payload;
	dc dc;
	dc.idc = *m_state->getPlayer()->geto_id();
	payload.idc = dc;

	//sendDataToPeer(p, payload, DISCONNECT);
	sendDataBroadcast(payload, DISCONNECT);
	enet_host_flush(client);
	
}



void Net::deletePeer(int id)
{

	m_state->deletePlayer(id);
	if (!connectedPeers.empty()) { connectedPeers.erase(id); }
	cout << "Player with ID : " + std::to_string(id) + " disconected " << endl;
	if (id == 0) {
		m_state->setOnline(false, false);
		cout << "HOST DISCONECTED" << '\n';
	}
}

startG Net::createSGDtata()
{
	startG strg = m_state->getMapData();
	strg.timeinfo = 0.0f;
	int i = 1;		
	// this is the pos of the host
	strg.posPlayer[0][0] = 0;									
	strg.posPlayer[0][1] = m_state->getPlayer()->m_pos_x;
	strg.posPlayer[0][2] = m_state->getPlayer()->m_pos_y;
	

	// this is the pos of connected peer i give them(as host)
	for (auto key : connectedPeers) {
		if (i % 2 ) {
			strg.posPlayer[i][0] = key.first;
			strg.posPlayer[i][1] = m_state->getPlayer()->m_pos_x + (i * 2);
			strg.posPlayer[i][2] = m_state->getPlayer()->m_pos_y + (i * 2);

		}
		else {
			strg.posPlayer[i][0] = key.first;
			strg.posPlayer[i][1] = m_state->getPlayer()->m_pos_x - (i * 2);
			strg.posPlayer[i][2] = m_state->getPlayer()->m_pos_y - (i * 2);
		
		}
		
		i++;
	}
	m_state->setMapData(strg);
	return strg;
}

void Net::sendPlayerInfo()
{
	Data d;
}


void Net::setmyID(int id)
{

	m_state->getPlayer()->seto_id(id);
	
	cout << "HOST SET MY ONLINE ID TO : " + to_string(*(int*)m_state->getPlayer()->geto_id())<< endl;
	

}

void Net::validatePeer(enet_uint32 ip, int id, ENetPeer* peer) // this should be called when host announces a new peer so i validate it 
{
	
	if (newPeers.find(ip) != newPeers.end()) {
		
		connectedPeers[id] = newPeers.find(ip)->second;
		newPeers.erase(ip);
		m_state->createPlayer(id);

		connectedPeers[id]->data = m_state->connectpeer2player(id);
		cout << "A new Player connected with ID :  " + std::to_string(id) + " IP : " + (hex_to_strip(peer->address.host)) << endl;
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

void Net::addpMOVEToQueue(int o_id, float angle, float speed, float x, float y, unsigned long framecounter)// this is called by game thread
{
	pMOVE packet;
	packet.id = o_id;
	packet.angle = angle;
	packet.speed = speed;
	packet.x = x;
	packet.y = y;
	packet.fc = framecounter;
	

	
	Data payload;

	//edw to antikeimeno pmove pernei ta apo panw 
	payload.pmove = packet;
	


	//m_state->getMutex().lock();
	p_packets.push(payload);
	//m_state->getMutex().unlock();


}

bool Net::isHost()
{
	return _host;
}

void Net::sendLoadedLevelMSG(int o_id)
{


	Loaded l;
	l.id = o_id;
	Data payload;
	payload.loaded_level = l;



	sendDataBroadcast(payload , LOADED_LEVEL);
	cout << "sending i loaded" << endl;


}

std::mutex& Net::getMutex()
{
	return net_mutex;
}

void Net::setinGame(bool ig)
{
	
	inGame = ig;
}

bool Net::getinGame()
{
	return inGame;
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

void Net::addaudiodata(int id, float arr[]) {
	//create pack
	audiodata adpacket;
	adpacket.playerid = id;
	//copy to vector 
	std::copy(arr, arr + 512, adpacket.audioData);

	//std::cout << "sendaudiodata has to send chunk with size : " << sizeof(adpacket.audioData) << std::endl;
	//std::copy(arr, arr + size, adpacket.audioData); //problima
	//create payload
	Data payload;
	payload.ad = adpacket;
	
	
	ad_packets.push(payload);

}
