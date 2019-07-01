#include "Level.h"



Level::Level(string name, size_t size, uint8_t** map, size_t treats_to_victory, uint8_t style)
	: 
	SIZE(size),
	treats_to_victory(treats_to_victory),
	STYLE(style)
{
///TODO
//vytvoø mapu
	//nastav mapu na vzduch
	for (uint8_t i = 0; i < SIZE; i++)
		for (uint8_t j = 0; j < SIZE; j++)
			level_map[i][j] = map[i][j];
	if (name.size() < 19) {
		for (int i = 0; i < name.size(); i++)NAME[i] = name[i];
		NAME[name.size()] = '\0';
	}
}

void Level::addLeveltoFile(uint8_t id){
	std::fstream save("Data/levels.dat", std::ios::binary | std::ios::out);
	save.seekp(sizeof(Config_level)+(sizeof(Level)*id));
	save.write((char*)(this), sizeof(Level));
	save.close();
}

void Level::setTile(uint8_t i, uint8_t j, LevelTile tile){
	level_map[i][j] = tile;
}

Level::~Level()
{
}
