#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include <fstream>

using namespace sf;
#define WINDOW_WIDTH 1200.f
#define WINDOW_HEIGHT 800.f
#define BASE_GAME_SPEED_S 0.1f
#define NUM_OF_SNAKES 7
#define MAX_FPS 50
#define LOOP(n)for(size_t i = 0;i<n;i++)

///DEBUG mod
//#define DEBUG

#ifdef DEBUG
using namespace std;
#include <conio.h>
#include <iostream>
#endif

#include "Player.h"
#include "Engine.h"
#include "Map.h"
#include "Snake.h"
#include "DynamicArray.h"
#include "Config.h"
#include "TextOutput.h"
#include "Button.h"
class Player;
class Snake;

class MainSystem
{
public:
	enum DIFFICULTY { EASY , NORMAL, HARD, EXIT };
	enum TEXTURE { TITLE = 0 };
	static enum SOUND_EFFECT {
		EATING_SOUND = 0,
		SONG1 = 1
	};
	MainSystem();
	~MainSystem() { 
		delete _window; 
		delete _snake; 
		delete _map; 
		delete default_font;
		delete mouse;
		LOOP(MainSystem::_sound_buffer.size())delete MainSystem::_sound_buffer[i];
	};
	bool mainGame(int16_t selectedLevel,uint16_t selectedSnake);
	bool gameOverScreen();
	bool gameOverScreenMul(const unsigned short &player);
	
	DIFFICULTY selectDifficulty();
	int16_t selectLevel();
	void titleScreen();
	bool victoryScreen( uint16_t score);
	//void save_player();
	void load_player();
	Level* load_level(uint8_t n);
	Config_level* load_level_config();
	void load_levels();
	void options();
	bool multiplayer(const DIFFICULTY &dif);
	unsigned short selectSnake();

public:
	static unsigned short _volume;
	static std::vector<SoundBuffer*> _sound_buffer;
private:
	RenderWindow *_window = nullptr;
	Config_level* _level_config = nullptr;
	int16_t selected_level;
	Clock _clock;
	Clock _waiter;
	Snake *_snake = nullptr;
	Map *_map = nullptr;
	Player *_player = nullptr;
	Config *_config = nullptr;
	vector<Level*> _levels;
	Mouse *mouse;
	Sound _song;
	Event e;
	unsigned _score;
	Font *default_font;
	static Color default_text_color;
	std::vector<Texture> texture_buffer;
	unsigned short _snake_pricec[NUM_OF_SNAKES] = { 0,10,25,50,100,250,1000 };
};

