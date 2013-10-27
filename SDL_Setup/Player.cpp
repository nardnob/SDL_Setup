#include "Player.h"
#include "constants.cpp"
using namespace std;

Player::Player()
{

}

Player::Player (int clipX_in, int clipY_in)
{
	//this->textureNum = textureNum_in;
	this->active = true;

	this->mass = MASS_PLAYER;
	this->forceX = 0;
	this->forceY = 0;
	this->velX = 0;
	this->velY = 0;
	this->maxVelX = 3;
	this->maxVelY = 3;
	this->posX = 100;
	this->posY = 100;

	this->currentTexture = TEXTURE_FRONT;

	//define TEXTURE_UP, DOWN, LEFT, and RIGHT clips
	for(int i = 0; i < 4; i++)
	{
		this->rect[i].x = clipX_in + ENTITY_CLIP_W * i;
		this->rect[i].y = clipY_in;
		this->rect[i].w = ENTITY_CLIP_W;
		this->rect[i].h = ENTITY_CLIP_H;
	}

	for(int i = 0; i < 4; i++)
	{
		this->keyDown[i] = false;
	}
}

void Player::pressKey(int toPress)
{
	this->keyDown[toPress] = true;
}

void Player::releaseKey(int toRelease)
{
	this->keyDown[toRelease] = false;
}

bool Player::keyIsDown(int i)
{
	return keyDown[i];
}

//should only accept a constant, i.e. TEXTURE_RIGHT, TEXTURE_LEFT. Must not overreach the bounds of the rect[] (currentTexture will be used as rect[currentTexture] in display)
void Player::toggleTexture(int toChoose)
{
	this->currentTexture = toChoose;
}