#include "mainScreen.h"
#include "GameState.h"
#include <string>
#include "setcolor.h"
#include <thread>
#include <chrono>
#include <minmax.h>
#include "Planet.h"


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
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 11:
	case 12:
		
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
				graphics::playSound(m_state->getFullAssetPath("select2.mp3"), 1.0f, false);
				select();

				return;
			}
			break;
	case 9:
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
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			graphics::playSound(m_state->getFullAssetPath("select2.mp3"), 1.0f, false);
			select();

			return;
		}
		
		break;
	case 10:
	case 13:
	
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			graphics::playSound(m_state->getFullAssetPath("select2.mp3"), 1.0f, false);
			select();

			return;
		}

		break;

	

	}

	
	
	
	
	

}









void MainScreen::moveup()
{
	

		

			
			if (selector == 0) {
				selector = 4;
				
				return;
			}
			if (selector == 5) {
				selector = 8;
				
				return;

			}
			if (selector == 11) {
				selector = 12;
				return;
			}

			

			selector--;
			return;
	
}

void MainScreen::movedown()
{
		
			if (selector == 4) {
				selector = 0;
			

				return;

			}
			if (selector == 8) {
				selector = 5;
		
				return;
			}
			if (selector == 12) {
				selector = 11;
				return;
			}
			
		
			selector++;
			return;
		

}

void MainScreen::moveleft()
{
	
		
			if (spaceshipSelector == 0) {
				spaceshipSelector = 3;
				
				return;

			}
			spaceshipSelector--;
			


		
		



	

}

void MainScreen::moveright()
{
	

		
			if (spaceshipSelector == 3) {
				spaceshipSelector = 0;
				
				return;

			}

			spaceshipSelector++;
			
			return;


	
		
	


}

void MainScreen::select()
{
	
			/// TO DO VALE ENUM DEN KATALAVENEI KANEIS TI GINETE EDW
			


			switch (selector) {
			case 0:

				m_state->setStatus('L');
				m_state->init();
				break;
			case 1:

				selector = 5;  

				break;
			case 2:
				selector = 9;
		
				break;
			case 3:
				selector = 10;
				break;
			case 5:
				m_state->setdificultyLVL(0);
				selector = 0;
				

				break;
			case 6:
				m_state->setdificultyLVL(1);
				selector = 0;

				break;

			case 7:
				m_state->setdificultyLVL(2);
				selector = 0;


				break;
			case 8:
				m_state->setdificultyLVL(3);
				selector = 0;
				break;
			case 9:


				m_state->setSpaceship(spaceshipSelector);
				selector = 0;

				break;
			case 10:
				selector = 0;
				break;
			case 4:
				selector = 11;
				
				break;
			case 11: // host a game
				selector = 13;
				m_state->setOnline(true, true);
				break;
			case 12:// join a game 
				selector = 13;
				m_state->setOnline(true , false);
				break;
			case 13:
				selector = 0;
				m_state->setOnline(false, false);
				break;

			}

		

	
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

		
		std::this_thread::sleep_for(std::chrono::duration<float, milli>(sleep_time));
		
		
		

}

void MainScreen::init()
{


	

	graphics::playMusic("assets//backgroundmain.mp3", 1.0f, true, 0);
	selector = 0;
	if (!m_background) {
		m_background = new Background();
	}
		
	m_background->init();
	
	
}

void MainScreen::draw()
{

	m_background->draw();
	graphics::setFont(m_state->getFullAssetPath("font.ttf"));
	m_main_text.fill_opacity = 1.0f;
	switch (selector) {
	case 0:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);



		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);

		break;
	case 1:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)

			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);

		break;
	case 2:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);

		break;
	case 3:

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);
			break;
	case 4:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 7.0f, "START", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "CHANGE DIFICULTY", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "CHOOSE SPACESHIP", m_main_text);
		
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 7.0f, 2.0f, "CONTROLS", m_main_text);
		
			SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 9.0f, 2.0f, "ONLINE", m_main_text);
			SETCOLOR(m_main_text.fill_color, 255, 255, 255)
		break;

	case 5:

		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "You lose 1hp when hit by a bullet, lot of resources around the map,", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, " colisions with asteroids lose you 2 hp, collisions with planets are fatal.", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);




		break;

	case 6:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);

		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "You lose 2hp when hit by a bullet , resources are limited around the map ,", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, " collisions with asteroids and planets are fatal. ", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);



		break;
	case 7:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);



		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "THE ULTIMATE CHALLENGE.When hit by a bullet you die,only bullet resources ", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, "and very rare hp regenerations around the map ,", m_main_text);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 2.0f, 1.0f, "enemy bullets are sharper and asteroids move faster. GOOD LUCK ", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);

		SETCOLOR(m_main_text.fill_color, 255, 255, 255);

		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);


		break;
	case 8:
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5, 2.0f, "EASY", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 2.0f, 2.0f, "HARD", m_main_text);



		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 1.0f, "PLAY UNTIL YOU DIE : Kill every planet to complete a siege.Earn 100 points from killing", m_main_text);
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 - 3.0f, 1.0f, "a planets and 1000 points for completing a siege", m_main_text);


		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 4.0f, 2.0f, "ARE YOU SURE?", m_main_text);


		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
			graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 6.0f, 2.0f, "SURVIVAL", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
			break;
	case 9:


		drawSpaceShip(spaceshipSelector);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0);
		graphics::drawText(9.6f, m_state->getCanvasHeight() * 0.5 + 5.0f, 5.0f, "SELECT", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255);

		break;
	case 10:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0);

		m_keys.outline_opacity = 0.0f;
		graphics::drawRect(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.5, 25.0f, 7.0f, m_keys);
		m_keys.texture = m_state->getFullAssetPath("keyboard.png");

		graphics::drawText(10.0f, m_state->getCanvasHeight() * 0.5 - 4.0f, 2.0f, "WASD TO MOVE", m_main_text);
		graphics::drawText(10.0f, m_state->getCanvasHeight() * 0.5 + 5.0f, 2.0f, "SPACE TO SHOOT", m_main_text);
		break;

	case 11:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 , 2.0f, "CREATE LOBBY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "JOIN GAME", m_main_text);
		
		break;
	case 12:
		
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 , 2.0f, "CREATE LOBBY", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 + 3.0f, 2.0f, "JOIN GAME", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
		break;
	case 13:
		SETCOLOR(m_main_text.fill_color, 255, 0, 0)
		graphics::drawText(2.0f, m_state->getCanvasHeight() * 0.5 , 2.0f, "DISCONECT", m_main_text);
		SETCOLOR(m_main_text.fill_color, 255, 255, 255)
		break;


	}

	m_main_text.fill_opacity = 0.3f;
	
	




}

