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

#include "Player.h"
#include "Engine.h"
#include "Map.h"
#include "Snake.h"
#include "DynamicArray.h"

class Player;
class Snake;

class MainSystem
{
public:
	enum DIFFICULTY { EASY = 999, NORMAL = 980, HARD = 940 };
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
		LOOP(MainSystem::_sound_buffer.size())delete MainSystem::_sound_buffer[i];
	};
	bool mainGame(const DIFFICULTY &dif);
	bool gameOverScreen();
	bool gameOverScreenMul(const unsigned short &player);
	DIFFICULTY selectDifficulty();
	void titleScreen();
	void save_player();
	void load_player();
	void options();
	bool multiplayer(const DIFFICULTY &dif);
	unsigned short selectSnake();
public:
	static unsigned short _volume;
	static std::vector<SoundBuffer*> _sound_buffer;
private:
	RenderWindow *_window = nullptr;
	Clock _clock;
	Snake *_snake = nullptr;
	Map *_map = nullptr;
	Player *_player = nullptr;
	Sound _song;
	unsigned _score;
	static Font default_font;
	static Color default_text_color;
	std::vector<Texture> texture_buffer;
	unsigned short _snake_pricec[NUM_OF_SNAKES] = { 0,10,25,50,100,250,1000 };
};

