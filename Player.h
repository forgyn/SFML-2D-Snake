#pragma once
#include "MainSystem.h"
class MainSystem;

class Player
{
public:
	Player();
	~Player();
	void save();
	void load();
	unsigned _money;
	unsigned long _hight_score_easy;
	unsigned long _hight_score_normal;
	unsigned long _hight_score_hard;
	unsigned long _treats_eaten;
	bool unlocked_snakes[NUM_OF_SNAKES] = {};
	bool unlocked_levels[100] = {};
};

