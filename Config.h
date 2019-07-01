#pragma once
#include <fstream>
class Config {
public:
	size_t level_num = 0;
	unsigned short _volume = 50;
	void save() {
		std::fstream save("Data/config.dat", std::ios::binary | std::ios::out);
		save.write((char*)(this), sizeof(Config));
		save.close();
	}
	void load() {
		std::fstream load("Data/config.dat", std::ios::binary | std::ios::in);
		load.read((char*)(this), sizeof(Config));
		load.close();
	}
};

