#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include <iostream>
#include "Snake.h"
#define LOOP(n)for(size_t i = 0;i<n;i++)
#define LOOPX(n)for(size_t x = 0;x<n;x++)
#define LOOPY(n)for(size_t y = 0;y<n;y++)
#define MAP_OFFSET_X 0
#define MAP_OFFSET_Y 0
#define MAP_SIZE_X 800
#define MAP_SIZE_Y 800
#define NUM_OF_TREATS 5
#define SIZE_OF_MAP 10
#define TILE (*_map[pos.y][pos.y])
using namespace sf;
typedef std::uniform_int_distribution<unsigned> randomU;
class Snake;


class Map{
public:
	enum TILE_TYPE {ERROR,AIR,SNAKE_HEAD,SNAKE_BODYI,SNAKE_BODYL,SNAKE_BODYLO,SNAKE_BODYO,SNAKE_BODYIL,SNAKE_TAIL,TREAT};
	struct Tile {
		Tile(const Vector2f &size, const Vector2f &pos);
			
		Tile(){ this->_type = ERROR; }
		void updateSnakeParts(const unsigned short &type);
		void updateNoSnake();
		void update();
		inline void draw(RenderWindow &window) {window.draw(_body);}
		void setType(const TILE_TYPE &type) { _type = type; }
		TILE_TYPE getType() { return _type; }
		void rotate(const float &angle){ _angle = angle; _body.setRotation(angle); }
		void setRandomTreat();
		void setRandomAngle() { rotate(randomAngle(randomGenerator)); }
		inline void reset();
		inline void setStyle(const unsigned short &s) { _style = s; }
		unsigned short getAngle() { return _angle; }
		static Texture _texture;
		static unsigned short _animation;
	private:
		static std::uniform_int_distribution<unsigned> randomTreat;
		static std::uniform_int_distribution<unsigned> randomAngle;
		
		const Vector2f _pos;
		const Vector2f _size;
		RectangleShape _body;
		unsigned short _angle = 0;
		size_t s_c = 40;
		unsigned _treatID = 0;
		unsigned short _style = 0;
		TILE_TYPE _type = AIR;
	};
	Map(const size_t &, const Vector2u &vm ,Color = Color::White);
	~Map();
	void update();
	void draw(RenderWindow &window);
	Tile ***getMap() { return _map; }
	void setTileType(const Vector2u &,const TILE_TYPE &);
	void updateTile(const Vector2u &pos,const unsigned short &type);
	bool checkTile(const Vector2u &, const TILE_TYPE &);
	void rotateTile(const Vector2u &, const float &);
	void updateNoSnake();
	void resetTile(const Vector2u &);
	void clear();
	void spawnTreat();
	//debug funkce
#ifdef DEBUG
	void showBitmap() {
		LOOPY(_size) {
			LOOPX(_size) {
				std::cout << std::setw(3) << _map[y][x]->getType() << " ";
			}
			std::cout << std::endl <<std::endl;
		}
	}
#endif
	size_t getSize() { return _size; }
	static std::default_random_engine randomGenerator;
private:
	Tile ***_map = nullptr;
	size_t _size;
	RectangleShape _background;
};

