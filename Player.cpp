#include "Player.h"



Player::Player(){
	_money = 0;
	_hight_score_easy = 0;
	_hight_score_normal = 0;
	_hight_score_hard = 0;
	_treats_eaten = 0;
	
	for (int i = 0; i < NUM_OF_SNAKES; i++)unlocked_snakes[i] = false;
	unlocked_snakes[0] = true;
	LOOP(100)unlocked_levels[i] = false;
	unlocked_levels[0] = true;
}

Player::~Player()
{
}

void Player::save(){
	std::fstream save("Data/player.dat", std::ios::binary | std::ios::out);
	save.write((char*)(this), sizeof(Player));
	save.close();
}

void Player::load(){
	std::fstream load("Data/player.dat", std::ios::binary | std::ios::in);
	load.read((char*)(this), sizeof(Player));
	load.close();
}
