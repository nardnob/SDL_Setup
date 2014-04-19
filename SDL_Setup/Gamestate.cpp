#include <Windows.h>

#include "constants.cpp"
#include "Button.h"
#include "Message.h"
#include "Gamestate.h"
using namespace std;

Gamestate::Gamestate()
{
	//The current state of the game to be calculated / displayed. i.e. Gameplay, menu system, etc.
	currentState = STATES_GAMEPLAY;

	//open the Gamestate font
	font_Gamestate_1 = TTF_OpenFont(FONT_GAMESTATE_1_FILENAME, FONT_GAMESTATE_1_SIZE);
	font_Gamestate_2 = TTF_OpenFont(FONT_GAMESTATE_2_FILENAME, FONT_GAMESTATE_2_SIZE);
	font_Gamestate_3 = TTF_OpenFont(FONT_GAMESTATE_3_FILENAME, FONT_GAMESTATE_3_SIZE);
}

void Gamestate::init()
{	
	//initialize the currentMap object with the mapFileName text file
	//currentMap = TerrainMap("map_001.txt", this);
	this->switchState(STATES_START_MENU);
}

void Gamestate::switchState(int newState)
{
	//delete the memory spaces in the heap
	for(int i = 0; i < this->vector_menuObjects.size(); i++)
	{
		delete this->vector_menuObjects.at(i);
	}

	//clear the current menuObjects
	this->vector_menuObjects.clear();

	//Set the games current state to the death menu.
	this->currentState = newState;

	//clear the keyboard buffer
	for(int i = 0; i < KEY_BUFFER_SIZE; i++)
	{
		this->vector_players.at(0).releaseKey(i);
	}

	switch(newState)
	{
	case STATES_DEATH_MENU:		
		this->vector_menuObjects.push_back(
			new Message(
				0,
				0,
				this->font_Gamestate_1,
				"Gameover! Loser!",
				true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight / 2 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Message(
				0,
				0,
				this->font_Gamestate_2,
				"Press Spacebar to continue ...",
				true
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = (gui->monitorHeight * 3)/4 - this->vector_menuObjects.back()->get_height() / 2;

		break;

	case STATES_GAMEPLAY:
		//reload to map_001 at the starting point
		gui->switchMap("map_001.txt", 100, 100);

		//restore the players' life
		vector_players.at(0).currentStatus.healLife();
		break;

	case STATES_START_MENU:
		this->vector_menuObjects.push_back(
			new Button(
				&this->vector_clickEvents,
				Button::BUTTON_START,
				647,
				78,
				0,
				0,
				0,
				0,
				"Start the Game",
				this->font_Gamestate_3
				));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_OPTIONS,
			647,
			78,
			0,
			0,
			0,
			0,
			"Options",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 100;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_CREDITS,
			647,
			78,
			0,
			0,
			0,
			0,
			"Credits",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 200;

		this->vector_menuObjects.push_back(
			new Button(
			&this->vector_clickEvents,
			Button::BUTTON_EXIT,
			647,
			78,
			0,
			0,
			0,
			0,
			"Exit",
			this->font_Gamestate_3
			));
		this->vector_menuObjects.back()->posX = gui->monitorWidth / 2 - this->vector_menuObjects.back()->get_width() / 2;
		this->vector_menuObjects.back()->posY = gui->monitorHeight * 3 / 5 - this->vector_menuObjects.back()->get_height() / 2 + 300;

		break;
	}
}