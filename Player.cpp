#include "Player.h"



Player::Player(){
	_money = 0;
	_hight_score_easy = 0;
	_hight_score_normal = 0;
	_hight_score_hard = 0;
	_treats_eaten = 0;
	_volume = 100;
	for (int i = 0; i < NUM_OF_SNAKES; i++)unlocked_snakes[i] = false;
}

Player::~Player()
{
}
