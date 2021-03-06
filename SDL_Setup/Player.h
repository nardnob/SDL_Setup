#ifndef PLAYER_H
#define PLAYER_H

#include "constants.cpp"
#include "Entity.h"
#include "PlayerStatus.h"

class Player: public Entity
{
private:
	bool
		keyDown[KEY_BUFFER_SIZE];

public:
	void pressKey(int toPress);
	void releaseKey(int toRelease);
	bool keyIsDown(int i);
	void toggleTexture(int toChoose);

	//PlayerStatus currentStatus;

	Player();
	Player(int in_clipX, int in_clipY, double in_posX, double in_posY, double in_base_posX, double in_base_posY, double in_base_w, double in_base_h);
};

#endif