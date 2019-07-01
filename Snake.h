#pragma once

//#define DEBUG

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
class Map;
class MainSystem;
#ifdef DEBUG
#include <iostream>
#include <iomanip>
#endif 
using namespace sf;
class Snake{
public:
	enum LOOK_DIRECTION {
		LEFT = 180,
		RIGHT = 0,
		DOWN = 270,
		UP = 90
	};
	enum STATE {
		NORMAL = 1,
		PAUSE = 10000,
		FAST = 5
	};
	
	Snake(Map &map,const unsigned short &type);
	bool addHead(Vector2u pos);
	inline bool moveSnake(const int & x, const int & y);
	bool addHead(const int & x, const int & y);
	bool addBody(Vector2u pos);
	bool addBody(const unsigned & x, const unsigned & y);
	bool addTail(Vector2u pos);
	bool addTail(const unsigned & x, const unsigned & y);
	inline unsigned isOnEdge(const Vector2u &pos);
	inline unsigned isInCorner(const Vector2u &pos);
	inline void moveFat();
	void input(const sf::Keyboard::Key &key);
	void inputP1(const sf::Keyboard::Key &key);
	void inputP2(const sf::Keyboard::Key &key);
	friend bool checkColide(const Snake &, const Snake &);
	bool checkColide();
	bool getFeed() { return _feeded; }
	void unFeed() { _feeded = false; }
	inline bool updatePos();
	inline void updateSnakeTiles();
	void updateSnakeTextures();
	size_t getSize() { return _part_pos.size(); }
	bool update();
	bool victory() { return _VICTORY; }
	~Snake();
	const unsigned short type;
private:
	bool _VICTORY = false;
	LOOK_DIRECTION _direction;
	LOOK_DIRECTION _prev_direction;
	STATE _state;
	Vector2u nextPos;
	bool _feeded;
	Map *_map = nullptr;
	std::vector<Vector2u> _part_pos;
	std::vector<bool> _snake_fat_body;
	Sound _sound_effect;
	inline void updateHead();
	inline void updateBody();
	inline void updateTail();
};

