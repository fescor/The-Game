#include "mainScreen.h"
#include "GameState.h"
#include <string>
#include "setcolor.h"
#include <thread>
#include <chrono>
#include <minmax.h>
#include "Planet.h"
#include "Net.h"
#include "Player.h"


using namespace std;



void MainScreen::drawSpaceShip(int i)
{
	m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(i) + ".png");
	
	m_spaceship.outline_opacity = 0.0f;

	graphics::drawRect(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);

}

void MainScreen::hover()// here implement hover on menus for all cases
{

	switch (selector)
	{
	case PLAY:
	case CDIFICULTY:
	case CSPACESHIP:
	case CONTROLS:
	case ONLINE:
	case CREATE_LOBBY:
	case JOIN_LOBBY:
	case SEND_BROADCAST:
	case DC:
	case GIVE_HOST_IP_INPUT:
	case GIVE_HOST_IP_CONNECT:
	case GIVE_HOST_IP_BACK:
	case CD_GAMEMODE_BACKTOLOBBY:
	

		if (graphics::getKeyState(graphics::SCANCODE_W)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			moveup();
			return;

		}

		if (graphics::getKeyState(graphics::SCANCODE_S)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			movedown();
			return;

		}
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			
			select();

			return;
		}
		break;
	case SELECT_SPACESHIP:
	case LOBBY_SCREEN_STARTG:
	case LOBBY_SCREEN_CS:
	case LOBBY_SCREEN_DISCONNECT:
	case LOBBY_SCREEN_GAMEMODES:
		if (graphics::getKeyState(graphics::SCANCODE_A)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			moveleft();

			return;
		}
		if (graphics::getKeyState(graphics::SCANCODE_D)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			moveright();

			return;
		}
		if (graphics::getKeyState(graphics::SCANCODE_W)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			moveup();
			return;

		}

		if (graphics::getKeyState(graphics::SCANCODE_S)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			movedown();
			return;

		}
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			
			select();

			return;
		}

		break;
	case SELECT_CONTROLS:

		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			
			select();

			return;
		}

		break;

	case CD_EASY:
	case CD_HARD:
	case CD_AREUSURE:
	case CD_SURVAVAL:
	
		if (graphics::getKeyState(graphics::SCANCODE_W)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			moveup();
			return;

		}

		if (graphics::getKeyState(graphics::SCANCODE_S)) {
			graphics::playSound(m_state->getFullAssetPath("hover.mp3"), 1.0f, false);
			movedown();
			return;

		}
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			if (m_state->amHost()) {
				select();
			}
			

			return;
		}
		break;


	}

}









void MainScreen::moveup()
{
	

		
	if (!m_state->getOnline() || m_state->getOnline() && m_state->amHost()) {
		if (selector == PLAY) {
			selector = ONLINE;

			return;
		}
		if (selector == CD_EASY) {
			selector = CD_SURVAVAL;

			return;

		}

	}
			
			
			if (selector == CREATE_LOBBY) {
				selector = JOIN_LOBBY;
				return;
			}
			if (selector == DC) {
				selector = SEND_BROADCAST;
				return;

			}
			if (selector == GIVE_HOST_IP_INPUT) {
				selector = GIVE_HOST_IP_BACK;
				return;

			}
			if (selector == GIVE_HOST_IP_CONNECT) {
				selector = GIVE_HOST_IP_INPUT;
				return;

			}
			if (selector == GIVE_HOST_IP_BACK) {
				selector = GIVE_HOST_IP_CONNECT;
				return;

			}
			if (m_state->amHost()) {
				if (selector == LOBBY_SCREEN_STARTG) {
					selector = LOBBY_SCREEN_GAMEMODES;
					return;
				}
				else if (selector == LOBBY_SCREEN_CS) {
					selector = LOBBY_SCREEN_DISCONNECT;
					return;

				}
				else if (selector == LOBBY_SCREEN_GAMEMODES) {
					selector = LOBBY_SCREEN_STARTG;
					return;
				}
				else if (selector == LOBBY_SCREEN_DISCONNECT) {
					selector = LOBBY_SCREEN_CS;
					return;
				}


			}
			else {
				if (selector == LOBBY_SCREEN_GAMEMODES || selector == LOBBY_SCREEN_CS) {
					selector = LOBBY_SCREEN_DISCONNECT;
					return;
				}
				if (selector == CD_EASY) {
					selector = CD_GAMEMODE_BACKTOLOBBY;
					return;
				}
			}

			

			selector--;
			return;
	
}

void MainScreen::movedown()
{
	if (!m_state->getOnline() || m_state->getOnline() && m_state->amHost()) {

		if (selector == ONLINE) {

			selector = PLAY;


			return;

		}
		if (selector == CD_SURVAVAL) {
			selector = CD_EASY;

			return;
		}
	}
		if (selector == JOIN_LOBBY) {
			selector = CREATE_LOBBY;
			return;
		}
		if (selector == SEND_BROADCAST)
		{
			selector = DC;
			return;
		}
		if (selector == GIVE_HOST_IP_INPUT) {
			selector = GIVE_HOST_IP_CONNECT;
			return;

		}
		if (selector == GIVE_HOST_IP_CONNECT) {
			selector = GIVE_HOST_IP_BACK;
			return;

		}
		if (selector == GIVE_HOST_IP_BACK) {
			selector = GIVE_HOST_IP_INPUT;
			return;

		}
	
			if (m_state->amHost()) {
				if (selector == LOBBY_SCREEN_GAMEMODES) {
					selector = LOBBY_SCREEN_STARTG;
					return;
				}
				if (selector == LOBBY_SCREEN_DISCONNECT) {
					selector = LOBBY_SCREEN_CS;
					return;
				}
				if (selector == LOBBY_SCREEN_STARTG) {
					selector = LOBBY_SCREEN_GAMEMODES;
					return;
				}
				if (selector == LOBBY_SCREEN_CS) {
					selector = LOBBY_SCREEN_DISCONNECT;
					return;
				}

			}
			else {
				if (selector == LOBBY_SCREEN_DISCONNECT) {
					selector = LOBBY_SCREEN_CS;
					return;
				}
				if (selector == LOBBY_SCREEN_GAMEMODES) {
					selector = LOBBY_SCREEN_DISCONNECT;
					return;
				}
				if (selector == CD_GAMEMODE_BACKTOLOBBY) {
					selector = CD_EASY ;
					return;
				}
			}
			
			selector++;
			return;
		

}

void MainScreen::moveleft()
{
	if (selector == SELECT_SPACESHIP) {


		spaceshipSelector--;
		if (spaceshipSelector < 0) {
			spaceshipSelector = 3;
		}
		while (m_state->availableSpaceship[spaceshipSelector] == 4) {
			spaceshipSelector--;
			if (spaceshipSelector < 0) {
				spaceshipSelector = 3;



			}

		}
	}
	if (m_state->amHost()) {
		if (selector == LOBBY_SCREEN_CS ) {
			selector = LOBBY_SCREEN_STARTG;
			return;
		}
		else if (selector == LOBBY_SCREEN_STARTG) {
			selector = LOBBY_SCREEN_CS;
			return;
		}
		else if (selector == LOBBY_SCREEN_GAMEMODES) {
			selector = LOBBY_SCREEN_DISCONNECT;
			return;
		}
		else if (selector == LOBBY_SCREEN_DISCONNECT) {
			selector = LOBBY_SCREEN_GAMEMODES;
			return;
		}

	}
	else {
		if (selector == LOBBY_SCREEN_CS) {
			selector = LOBBY_SCREEN_GAMEMODES;
			return;
		}
		if (selector == LOBBY_SCREEN_GAMEMODES) {
			selector = LOBBY_SCREEN_CS;
			return;
		}
	}
	

						
			
			
			

		
		



	

}

void MainScreen::moveright()
{
	if (selector == SELECT_SPACESHIP) {
		spaceshipSelector++;
		if (spaceshipSelector > 3) { spaceshipSelector = 0; }
		while (m_state->availableSpaceship[spaceshipSelector] == 4) {
			spaceshipSelector++;
			if (spaceshipSelector > 3) {
				spaceshipSelector = 0;



			}

		}
	}
	if (m_state->amHost()) {
		if (selector == LOBBY_SCREEN_STARTG) {

			selector = LOBBY_SCREEN_CS;
			return;


		}
		else if (selector == LOBBY_SCREEN_CS ) {
			selector = LOBBY_SCREEN_STARTG;
			return;
		}
		else if (selector == LOBBY_SCREEN_DISCONNECT) {
			selector = LOBBY_SCREEN_GAMEMODES;
			return;
		}
		else if (selector == LOBBY_SCREEN_GAMEMODES) {
			selector = LOBBY_SCREEN_DISCONNECT;
			return;
		}
	}
	else {
		if (selector == LOBBY_SCREEN_GAMEMODES) {
			selector = LOBBY_SCREEN_CS;
			return;
		}
		if (selector == LOBBY_SCREEN_CS) {
			selector = LOBBY_SCREEN_GAMEMODES;
			return;
		}
	}
	

		

	
		
	


}

void MainScreen::getKeyStrokes()
{
	if (uiipIter < 14) {
		if (graphics::getKeyState(graphics::SCANCODE_0)) {
			userinputIP[uiipIter] = '0';
			uiipIter++;

		}
		else if (graphics::getKeyState(graphics::SCANCODE_1)) {
			userinputIP[uiipIter] = '1';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_2)) {
			userinputIP[uiipIter] = '2';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_3)) {
			userinputIP[uiipIter] = '3';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_4)) {
			userinputIP[uiipIter] = '4';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_5)) {
			userinputIP[uiipIter] = '5';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_6)) {
			userinputIP[uiipIter] = '6';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_7)) {
			userinputIP[uiipIter] = '7';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_8)) {
			userinputIP[uiipIter] = '8';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_9)) {
			userinputIP[uiipIter] = '9';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE)) {
			if (uiipIter != 0) {
				userinputIP[uiipIter - 1] = ' ';
			}
			uiipIter--;
			if (uiipIter < 0) {
				uiipIter = 0;
			}


		}
		else if (graphics::getKeyState(graphics::SCANCODE_PERIOD)) {
			userinputIP[uiipIter] = '.';
			uiipIter++;
		}
		else if (graphics::getKeyState(graphics::SCANCODE_KP_ENTER)) {
			selector == GIVE_HOST_IP_CONNECT;

		}

	}
	else {
		if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE)) {
			if (uiipIter != 0) {
				userinputIP[uiipIter - 1] = ' ';
			}
			uiipIter--;



		}
	}

	






}

void MainScreen::select()
{
	
			
			
			

			switch (selector) {
			case PLAY:

				m_state->setStatus('L');
				
				break;
			case CDIFICULTY:

				selector = CD_EASY;  

				break;
			case CSPACESHIP:
				
				m_state->changeAvailableSpaceshipArray(m_state->getPlayer()->getPSpaceship(), m_state->getPlayer()->getPSpaceship());
				selector = SELECT_SPACESHIP;
		
				break;
			case CONTROLS:
				selector = SELECT_CONTROLS;
				break;
			case CD_EASY:
				m_state->setdificultyLVL(0);
				selector = PLAY;
				if (m_state->getOnline() ) {
					
					selector = LOBBY_SCREEN_GAMEMODES;
				}
				

				break;
			case CD_HARD:
				m_state->setdificultyLVL(1);
				selector = PLAY;
				if (m_state->getOnline()) {
					selector = LOBBY_SCREEN_GAMEMODES;
				}

				break;

			case CD_AREUSURE:
				m_state->setdificultyLVL(2);
				selector = PLAY;
				if (m_state->getOnline()) {
					selector = LOBBY_SCREEN_GAMEMODES;
				}


				break;
			case CD_SURVAVAL:
				m_state->setdificultyLVL(3);
				selector = PLAY;
				if (m_state->getOnline()) {
					selector = LOBBY_SCREEN_GAMEMODES;
				}

				break;
			case SELECT_SPACESHIP:

				
				
				if (m_state->getOnline()) {
					selector = LOBBY_SCREEN_CS;
					
				}
				else {
					selector = PLAY;
				}
				if (m_state->setSpaceship(spaceshipSelector)) {
					m_state->getNet()->changedSpaceship = true;
				}
				
				break;
			case SELECT_CONTROLS:
				selector = PLAY;
				break;
			case ONLINE:

				selector = CREATE_LOBBY;
				break;
			case CREATE_LOBBY: // host a game
				
				m_state->setOnline(true, true);
				
				selector = LOBBY_SCREEN_STARTG;
				
				
				
				
				break;
			case JOIN_LOBBY:// join a game 
				
				selector = GIVE_HOST_IP_INPUT;
				//m_state->setOnline(true , false);
				break;
			case DC:
				selector = PLAY;
				m_state->setOnline(false, false);
				break;
			case SEND_BROADCAST:
				//IMPLEMENT SENDING BROADCAST WITH THE QPACKETS IMPLAMANTATION
				/*
				m_state->getNet()->addpMOVEToQueue(*(m_state->getPlayer()->geto_id()), m_state->getPlayer()->getAngle(), m_state->getPlayer()->getSpeed(),
					m_state->getPlayer()->getX(), m_state->getPlayer()->getY());
				*/
				if (m_state->getNet()->isHost()) {
					m_state->setStatus('L');
				}
				
				

				break;
			case LOBBY_SCREEN_STARTG:
				if (m_state->getNet()->isHost()) {
					m_state->setStatus('L');
				}
				break;
			case LOBBY_SCREEN_CS:
				
				m_state->changeAvailableSpaceshipArray(m_state->getPlayer()->getPSpaceship(), m_state->getPlayer()->getPSpaceship());
				while (m_state->availableSpaceship[spaceshipSelector] == 4) {
					spaceshipSelector++;
					if (spaceshipSelector > 3) {
						spaceshipSelector = 0;
					}
				}
				selector = SELECT_SPACESHIP;
				break;
				
			case GIVE_HOST_IP_BACK:
				selector = CREATE_LOBBY;

				break;

			case LOBBY_SCREEN_DISCONNECT:
				
				m_state->setOnline(false, false);
				//selector = CREATE_LOBBY;

				break;
			case GIVE_HOST_IP_CONNECT:
				if (std::string(userinputIP).empty()) {
					break;
				}
				m_state->setHostIP(std::string(userinputIP));
				m_state->setOnline(true, false);
				m_state->connectingToHost = true;
				
				
				selector = LOBBY_SCREEN_CS;
				break;

			case  GIVE_HOST_IP_INPUT:

				return;
			case LOBBY_SCREEN_GAMEMODES:
				selector = CD_EASY;
				break;
			case CD_GAMEMODE_BACKTOLOBBY:
				selector = LOBBY_SCREEN_GAMEMODES;
				break;
			}

			graphics::playSound(m_state->getFullAssetPath("select2.mp3"), 1.0f, false);

	
}

int MainScreen::getSelector()
{
	return selector;
}

void MainScreen::setSelector(int s)
{
	selector = s;
}

MainScreen::MainScreen()
{
	
	
	
	
}

void MainScreen::update(float dt)
{
	
		float sleep_time = 100.0f;

		hover();	
		if (selector == GIVE_HOST_IP_INPUT) {
			getKeyStrokes();
		}
		std::this_thread::sleep_for(std::chrono::duration<float, milli>(sleep_time));


}

void MainScreen::init()
{


	

	graphics::playMusic("assets//backgroundmain.mp3", 1.0f, true, 0);
	selector = PLAY;
	if (!m_background) {
		m_background = new Background();
	}
		
	m_background->init();
	
	
}

void MainScreen::draw()
{
	int i;
	
	SETCOLOR(m_lobby_gui.outline_color, 250, 0, 0);
	SETCOLOR(test.fill_color, 250, 0, 0);
	m_lobby_gui.fill_opacity = 1.0f;
	m_lobby_gui.outline_width = 2.0f;

	m_background->draw();
	graphics::setFont(m_state->getFullAssetPath("font.ttf"));
	m_main_text.fill_opacity = 1.0f;
	switch (selector) {
	case PLAY:

		
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);



		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);

		break;
	case CDIFICULTY:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)

			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);

		break;
	case CSPACESHIP:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);

		break;
	case CONTROLS:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);
		break;
	case ONLINE:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);

		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			break;

	case CD_EASY:

		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "You lose 1hp when hit by a bullet, lot of resources around the map,", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, " colisions with asteroids lose you 2 hp, collisions with planets are fatal.", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);

		if (!m_state->amHost()) {

			SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight()*0.4, 1.0f, "NOTE : ONLY HOST CAN CHANGE THE GAME MODE FOR THE LOBBY!", m_main_text);
			
			SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(m_state->getCanvasWidth()*0.3f, m_state->getCanvasHeight() - 1.0f, 2.0f, "BACK TO LOBBY", m_main_text);

		}




		break;

	case CD_HARD:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);

		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "You lose 2hp when hit by a bullet , resources are limited around the map ,", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, " collisions with asteroids and planets are fatal. ", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);
		if (!m_state->amHost()) {

			SETCOLOR(m_main_text.fill_color, 255, 0, 0)
				graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.4, 1.0f, "NOTE : ONLY HOST CAN CHANGE THE GAME MODE FOR THE LOBBY!", m_main_text);

			SETCOLOR(m_main_text.fill_color, 255, 255, 255)
				graphics::drawText(m_state->getCanvasWidth() * 0.3f, m_state->getCanvasHeight() - 1.0f, 2.0f, "BACK TO LOBBY", m_main_text);

		}



		break;
	case CD_AREUSURE:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);



		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "THE ULTIMATE CHALLENGE.When hit by a bullet you die,only bullet resources ", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, "and very rare hp regenerations around the map ,", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 2.0f, 1.0f, "enemy bullets are sharper and asteroids move faster. GOOD LUCK ", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);

		SETCOLOR(m_main_text.fill_color, 255, 255, 255);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);
		if (!m_state->amHost()) {

			SETCOLOR(m_main_text.fill_color, 255, 0, 0)
				graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.4, 1.0f, "NOTE : ONLY HOST CAN CHANGE THE GAME MODE FOR THE LOBBY!", m_main_text);

			SETCOLOR(m_main_text.fill_color, 255, 255, 255)
				graphics::drawText(m_state->getCanvasWidth() * 0.3f, m_state->getCanvasHeight() - 1.0f, 2.0f, "BACK TO LOBBY", m_main_text);

		}


		break;
	case CD_SURVAVAL:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);



		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "PLAY UNTIL YOU DIE : Kill every planet to complete a siege.Earn 100 points from killing", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, "a planets and 1000 points for completing a siege", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);


		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			if (!m_state->amHost()) {

				SETCOLOR(m_main_text.fill_color, 255, 0, 0)
					graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.4, 1.0f, "NOTE : ONLY HOST CAN CHANGE THE GAME MODE FOR THE LOBBY!", m_main_text);

				SETCOLOR(m_main_text.fill_color, 255, 255, 255)
					graphics::drawText(m_state->getCanvasWidth() * 0.3f, m_state->getCanvasHeight() - 1.0f, 2.0f, "BACK TO LOBBY", m_main_text);

			}

			break;
	case CD_GAMEMODE_BACKTOLOBBY:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);



		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "PLAY UNTIL YOU DIE : Kill every planet to complete a siege.Earn 100 points from killing", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, "a planets and 1000 points for completing a siege", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);


		
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			if (!m_state->amHost()) {

				SETCOLOR(m_main_text.fill_color, 255, 0, 0)
					graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.4, 1.0f, "NOTE : ONLY HOST CAN CHANGE THE GAME MODE FOR THE LOBBY!", m_main_text);

				
					graphics::drawText(m_state->getCanvasWidth() * 0.3f, m_state->getCanvasHeight() - 1.0f, 2.0f, "BACK TO LOBBY", m_main_text);

			}
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
		break;

	case SELECT_SPACESHIP:


		drawSpaceShip(spaceshipSelector);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0);
		graphics::drawText(9.6f, m_state->getCanvasHeight() * 0.5 + 5.0f, 5.0f, "SELECT", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255);

		break;
	case SELECT_CONTROLS:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0);

		m_keys.outline_opacity = 0.0f;
		graphics::drawRect(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5, 25.0f, 7.0f, m_keys);
		m_keys.texture = m_state->getFullAssetPath("keyboard.png");

		graphics::drawText(10.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 2.0f, "WASD TO MOVE", m_main_text);
		graphics::drawText(10.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "SPACE TO SHOOT", m_main_text);
		break;

	case CREATE_LOBBY:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "CREATE LOBBY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "JOIN GAME", m_main_text);

		break;
	case JOIN_LOBBY:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "CREATE LOBBY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "JOIN GAME", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			break;
	case DC:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "DISCONECT", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.f, 2.0f, "START GAME", m_main_text);
			break;

	case SEND_BROADCAST:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "DISCONECT", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.f, 2.0f, "START GAME", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			break;
	case LOBBY_SCREEN_STARTG:

		i = 1;
		//graphics::drawText(mouse.cur_pos_x + 1, mouse.cur_pos_y + 1, 1.0f, std::to_string(mouse.cur_pos_x) + " , " + std::to_string(mouse.cur_pos_y), test);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		graphics::drawText(m_state->getCanvasWidth() * 0.5f  - m_state->getCanvasWidth()*0.1, m_state->getCanvasHeight() * 0.5f - m_state->getCanvasHeight() * 0.3, 2.0f, "PLAYERS", m_lobby_gui);
		m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(m_state->getPlayer()->getPSpaceship()) + ".png");

		m_spaceship.outline_opacity = 0.0f;

		graphics::drawRect(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth()*0.35, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player1", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 0);
		for (auto iter = m_state->geto_playersmap().begin(); iter != m_state->geto_playersmap().end(); iter++) {
			i++;
			
			m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(iter->second->getPSpaceship()) + ".png");

			m_spaceship.outline_opacity = 0.0f;
			
			graphics::drawRect((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35)*i *1.3, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
			graphics::drawText((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386) *i*1.55, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player" + std::to_string(i), m_lobby_gui);
			
			

		}
		
		if (!m_state->amHost()) {
			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth()*0.35, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);
		}else{
			
				SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
				graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "START GAME", m_lobby_gui);
				SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
				graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);
				graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "GAME MODES", m_lobby_gui);
				graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);

				SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			

		}
		break;
	case LOBBY_SCREEN_CS:
		i = 1;
		//graphics::drawText(mouse.cur_pos_x + 1, mouse.cur_pos_y + 1, 1.0f, std::to_string(mouse.cur_pos_x) + " , " + std::to_string(mouse.cur_pos_y), test);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		graphics::drawText(m_state->getCanvasWidth() * 0.5f - m_state->getCanvasWidth() * 0.1, m_state->getCanvasHeight() * 0.5f - m_state->getCanvasHeight() * 0.3, 2.0f, "PLAYERS", m_lobby_gui);
		m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(m_state->getPlayer()->getPSpaceship()) + ".png");

		m_spaceship.outline_opacity = 0.0f;

		graphics::drawRect(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player1", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 0);
		for (auto iter = m_state->geto_playersmap().begin(); iter != m_state->geto_playersmap().end(); iter++) {
			i++;

			m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(iter->second->getPSpaceship()) + ".png");

			m_spaceship.outline_opacity = 0.0f;

			graphics::drawRect((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35) * 2*i*0.6 , m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
			graphics::drawText((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386) * 2*i*0.8 , m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player" + std::to_string(i), m_lobby_gui);



		}
		if (m_state->amHost()) {
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "START GAME", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "GAME MODES", m_lobby_gui);
			
			graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);

			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);

		}
		else {
			

			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.6, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "GAME MODES", m_lobby_gui);
			
			graphics::drawText(m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);


		}


		break;



	case GIVE_HOST_IP_INPUT:
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47,m_state->getCanvasHeight()  - m_state->getCanvasHeight() *0.85, 1.1f,
			" TYPE TO THE BOX BELOW THE LOCAL IP ADDRESS(LAN ADDRESS) OF THE LOBBY HOST.", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.80, 1.f,
			" BE AWARE THAT IN SOME ROUTERS DEFULT SETTINGS CHANGES THE LAN ADDRESS ", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.75, 1.f,
			" PERIODICLY IF THAT HAPPENS WHILE YOU ARE PLAYING OR EVEN WHILE YOU ARE IN A ", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.465, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.70, 1.f,
			"LOBBY THAT WOULD CAUSE A DISCONECTION. ", m_lobby_gui);


		m_lobby_gui.fill_opacity = 0.0f;
		m_lobby_gui.fill_opacity = 1.0f;
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		SETCOLOR(m_lobby_gui.outline_color, 255, 0, 0);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.075, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.1, 2.0f, "HOST IP", m_lobby_gui);
		m_lobby_gui.fill_opacity = 0.0f;
		graphics::drawRect(m_state->getCanvasWidth()*.5f , m_state->getCanvasHeight()*0.5f , m_state->getCanvasWidth() - m_state->getCanvasWidth()*0.3, 4, m_lobby_gui);
		m_lobby_gui.fill_opacity = 1.0f;
		graphics::drawText(m_state->getCanvasWidth() * .25f, m_state->getCanvasHeight() * 0.525f, 3, std::string(userinputIP), m_lobby_gui);

		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
		graphics::drawText(m_state->getCanvasWidth() * 0.35, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.27, 3.0f, "CONNECT", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.41, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.42, 3.0f, "BACK", m_lobby_gui);
		break;
	case GIVE_HOST_IP_CONNECT:
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.85, 1.1f,
			" TYPE TO THE BOX BELOW THE LOCAL IP ADDRESS(LAN ADDRESS) OF THE LOBBY HOST.", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.80, 1.f,
			" BE AWARE THAT IN SOME ROUTERS DEFULT SETTINGS CHANGES THE LAN ADDRESS ", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.75, 1.f,
			" PERIODICLY IF THAT HAPPENS WHILE YOU ARE PLAYING OR EVEN WHILE YOU ARE IN A ", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.465, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.70, 1.f,
			"LOBBY THAT WOULD CAUSE A DISCONECTION. ", m_lobby_gui);
		m_lobby_gui.fill_opacity = 0.0f;
		m_lobby_gui.fill_opacity = 1.0f;
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
		SETCOLOR(m_lobby_gui.outline_color, 255, 255, 255);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.075, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.1, 2.0f, "HOST IP", m_lobby_gui);
		m_lobby_gui.fill_opacity = 0.0f;
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		graphics::drawRect(m_state->getCanvasWidth() * .5f, m_state->getCanvasHeight() * 0.5f, m_state->getCanvasWidth() - m_state->getCanvasWidth() * 0.3, 4, m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		m_lobby_gui.fill_opacity = 1.0f;
		graphics::drawText(m_state->getCanvasWidth() * .25f, m_state->getCanvasHeight() * 0.525f, 3, std::string(userinputIP), m_lobby_gui);
		
		graphics::drawText(m_state->getCanvasWidth() * 0.35, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.27, 3.0f, "CONNECT", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 250, 250);
		graphics::drawText(m_state->getCanvasWidth() * 0.41, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.42, 3.0f, "BACK", m_lobby_gui);

		break;
	case GIVE_HOST_IP_BACK:
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.85, 1.1f,
			" TYPE TO THE BOX BELOW THE LOCAL IP ADDRESS(LAN ADDRESS) OF THE LOBBY HOST.", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.80, 1.f,
			" BE AWARE THAT IN SOME ROUTERS DEFULT SETTINGS CHANGES THE LAN ADDRESS ", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.47, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.75, 1.f,
			" PERIODICLY IF THAT HAPPENS WHILE YOU ARE PLAYING OR EVEN WHILE YOU ARE IN A ", m_lobby_gui);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.465, m_state->getCanvasHeight() - m_state->getCanvasHeight() * 0.70, 1.f,
			"LOBBY THAT WOULD CAUSE A DISCONECTION. ", m_lobby_gui);
		m_lobby_gui.fill_opacity = 0.0f;
		m_lobby_gui.fill_opacity = 1.0f;
		
		SETCOLOR(m_lobby_gui.outline_color, 255, 255, 255);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.075, m_state->getCanvasHeight() * 0.5 - m_state->getCanvasHeight() * 0.1, 2.0f, "HOST IP", m_lobby_gui);
		m_lobby_gui.fill_opacity = 0.0f;
		graphics::drawRect(m_state->getCanvasWidth() * .5f, m_state->getCanvasHeight() * 0.5f, m_state->getCanvasWidth() - m_state->getCanvasWidth() * 0.3, 4, m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		m_lobby_gui.fill_opacity = 1.0f;
		graphics::drawText(m_state->getCanvasWidth() * .25f, m_state->getCanvasHeight() * 0.525f, 3, std::string(userinputIP), m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
		graphics::drawText(m_state->getCanvasWidth() * 0.35, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.27, 3.0f, "CONNECT", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		graphics::drawText(m_state->getCanvasWidth() * 0.41, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.42, 3.0f, "BACK", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
		break;
	case LOBBY_SCREEN_DISCONNECT:
		i = 1;
		//graphics::drawText(mouse.cur_pos_x + 1, mouse.cur_pos_y + 1, 1.0f, std::to_string(mouse.cur_pos_x) + " , " + std::to_string(mouse.cur_pos_y), test);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		graphics::drawText(m_state->getCanvasWidth() * 0.5f - m_state->getCanvasWidth() * 0.1, m_state->getCanvasHeight() * 0.5f - m_state->getCanvasHeight() * 0.3, 2.0f, "PLAYERS", m_lobby_gui);
		m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(m_state->getPlayer()->getPSpaceship()) + ".png");

		m_spaceship.outline_opacity = 0.0f;

		graphics::drawRect(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player1", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 0);
		for (auto iter = m_state->geto_playersmap().begin(); iter != m_state->geto_playersmap().end(); iter++) {
			i++;

			m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(iter->second->getPSpaceship()) + ".png");

			m_spaceship.outline_opacity = 0.0f;

			graphics::drawRect((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35) * 2 * i * 0.6, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
			graphics::drawText((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386) * 2 * i * 0.8, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player" + std::to_string(i), m_lobby_gui);



		}
		if (m_state->amHost()) {
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "START GAME", m_lobby_gui);

			graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);

			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "GAME MODES", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);

			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);

		}
		else {
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			

			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);

			graphics::drawText(m_state->getCanvasWidth() * 0.6, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "GAME MODES", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.4 , m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);

			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);

		}
		

		break;
	case LOBBY_SCREEN_GAMEMODES:
		i = 1;
		//graphics::drawText(mouse.cur_pos_x + 1, mouse.cur_pos_y + 1, 1.0f, std::to_string(mouse.cur_pos_x) + " , " + std::to_string(mouse.cur_pos_y), test);
		SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
		graphics::drawText(m_state->getCanvasWidth() * 0.5f - m_state->getCanvasWidth() * 0.1, m_state->getCanvasHeight() * 0.5f - m_state->getCanvasHeight() * 0.3, 2.0f, "PLAYERS", m_lobby_gui);
		m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(m_state->getPlayer()->getPSpaceship()) + ".png");

		m_spaceship.outline_opacity = 0.0f;

		graphics::drawRect(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
		graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player1", m_lobby_gui);
		SETCOLOR(m_lobby_gui.fill_color, 255, 255, 0);
		for (auto iter = m_state->geto_playersmap().begin(); iter != m_state->geto_playersmap().end(); iter++) {
			i++;

			m_spaceship.texture = m_state->getFullAssetPath("spaceship" + to_string(iter->second->getPSpaceship()) + ".png");

			m_spaceship.outline_opacity = 0.0f;

			graphics::drawRect((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.35) * 2 * i * 0.6, m_state->getCanvasHeight() * 0.5 - 2.0f, 4.0f, 4.0f, m_spaceship);
			graphics::drawText((m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.386) * 2 * i * 0.8, m_state->getCanvasHeight() * 0.5 + 1.0f, 1.0f, "Player" + std::to_string(i), m_lobby_gui);



		}


		if (m_state->amHost()) {
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "START GAME", m_lobby_gui);

			graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "GAME MODES", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);

			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);

		}
		else {
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);


			graphics::drawText(m_state->getCanvasWidth() * 0.5 - m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "CHANGE SPACESHIP", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 0, 0);
			graphics::drawText(m_state->getCanvasWidth() * 0.6, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.2, 2.0f, "GAME MODES", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			graphics::drawText(m_state->getCanvasWidth() * 0.4, m_state->getCanvasHeight() * 0.5 + m_state->getCanvasHeight() * 0.4, 2.0f, "DISCONNECT", m_lobby_gui);
			SETCOLOR(m_lobby_gui.fill_color, 255, 255, 255);
			

		}
		break;

	}
		



		
		





	


	m_main_text.fill_opacity = 0.3f;
	
	




}

