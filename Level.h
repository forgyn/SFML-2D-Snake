#pragma once
#include <fstream>
#include <string>
typedef unsigned char uint8_t;

enum LevelTile {AIR = 0, OBSTACLE = 1, FINISH_POINT = 2,SNAKE_HEAD = 3,SNAKE_TAIL = 4};

using namespace std;

class Config_level {
public:
	uint8_t level_number = 0;
	void saveToFile() {
		std::fstream save("Data/levels.dat", std::ios::binary | std::ios::out);
		save.write((char*)(this), sizeof(Config_level));
		save.close();
	}
};

class Level
{
public:
	Level(string name, size_t size, uint8_t** map,size_t treats_to_victory = 10, uint8_t style = 0);
	Level(){}
	void addLeveltoFile(uint8_t id);
	void setTile(uint8_t i,uint8_t j,LevelTile tile);
	~Level();
	char NAME[20] = "ERROR";
	size_t SIZE = 0;
	size_t treats_to_victory = 0;
	uint8_t STYLE = 0;
	uint8_t level_map[40][40];
};

