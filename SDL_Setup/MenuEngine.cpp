#include "MenuEngine.h"
#include "Button.h"

void MenuEngine::engine(int state, Gamestate& gamestate)
{
	switch(state)
	{
	case STATES_GAMEPLAY:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_ESC))
		{
			gamestate.switchState(STATES_PAUSE_MENU);
		}
		break;
	case STATES_DEATH_MENU:
		/*
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(STATES_START_MENU);
		}
		*/

		for(int i = 0; i < gamestate.vector_clickEvents.size(); i++)
		{
			switch(gamestate.vector_clickEvents.at(i))
			{
			case Button::BUTTON_TOSTATE_STARTMENU:
				gamestate.switchState(STATES_START_MENU);
				break;
			}
			gamestate.vector_clickEvents.pop_back();
		}
		break;

	case STATES_OPTIONS_MENU:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(gamestate.backTo.back());
			gamestate.backTo.pop_back();
		}

		for(int i = 0; i < gamestate.vector_clickEvents.size(); i++)
		{
			switch(gamestate.vector_clickEvents.at(i))
			{
			case Button::BUTTON_TOSTATE_PREVIOUS:
				gamestate.switchState(gamestate.backTo.back());
				gamestate.backTo.pop_back();
				break;
			}
			gamestate.vector_clickEvents.pop_back();
		}
		break;

	case STATES_CREDITS_MENU:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(STATES_START_MENU);
		}

		for(int i = 0; i < gamestate.vector_clickEvents.size(); i++)
		{
			switch(gamestate.vector_clickEvents.at(i))
			{
			case Button::BUTTON_TOSTATE_STARTMENU:
				gamestate.switchState(STATES_START_MENU);
				break;
			}
			gamestate.vector_clickEvents.pop_back();
		}
		break;

	case STATES_PAUSE_MENU:
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(STATES_GAMEPLAY);
		}

		for(int i = 0; i < gamestate.vector_clickEvents.size(); i++)
		{
			switch(gamestate.vector_clickEvents.at(i))
			{
			case Button::BUTTON_EXIT:
				gamestate.gui->quit = true;
				break;
			case Button::BUTTON_TOSTATE_OPTIONS:
				gamestate.backTo.push_back(STATES_PAUSE_MENU);
				gamestate.switchState(STATES_OPTIONS_MENU);
				break;
			case Button::BUTTON_TOSTATE_GAMEPLAY:
				gamestate.switchState(STATES_GAMEPLAY);
				break;
			case Button::BUTTON_TOSTATE_STARTMENU:
				gamestate.switchState(STATES_START_MENU);
				break;
			}
			gamestate.vector_clickEvents.pop_back();   
		}
		break;

	case STATES_START_MENU:
		/*
		if(gamestate.vector_players.at(0).keyIsDown(KEY_SPACE))
		{
			gamestate.switchState(STATES_GAMEPLAY);
		}
		*/

		for(int i = 0; i < gamestate.vector_clickEvents.size(); i++)
		{
			switch(gamestate.vector_clickEvents.at(i))
			{
			case Button::BUTTON_TOSTATE_GAMEPLAY:
				gamestate.switchState(STATES_GAMEPLAY);
				break;

			case Button::BUTTON_TOSTATE_OPTIONS:
				gamestate.backTo.push_back(STATES_START_MENU);
				gamestate.switchState(STATES_OPTIONS_MENU);
				break;

			case Button::BUTTON_TOSTATE_CREDITS:
				gamestate.switchState(STATES_CREDITS_MENU);
				break;

			case Button::BUTTON_EXIT:
				gamestate.gui->quit = true;
				break;
			}
			gamestate.vector_clickEvents.pop_back();
		}
		break;
	}
}