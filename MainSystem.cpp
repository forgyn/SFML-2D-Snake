#include "MainSystem.h"
#include <iostream>


Color MainSystem::default_text_color = Color(50,200,50);
unsigned short MainSystem::_volume = 100;
std::vector<SoundBuffer*> MainSystem::_sound_buffer = std::vector<SoundBuffer*>();

MainSystem::MainSystem(){
	_player = new Player;
	_player->load();
	_config = new Config;
	_config->load();
	_volume = _config->_volume;
	MainSystem::_sound_buffer.push_back(new SoundBuffer());
	MainSystem::_sound_buffer.back()->loadFromFile("Sounds/eating_sound_1.ogg");
	MainSystem::_sound_buffer.push_back(new SoundBuffer());
	MainSystem::_sound_buffer.back()->loadFromFile("Sounds/YoureGonnaGoFarKidEurobeatRemix.ogg");
	_score = 0;
	default_font = new Font;
	default_font->loadFromFile("Fonts/font2.ttf");
	Map::Tile::_texture.loadFromFile("Textures/Snake.png");
	texture_buffer.push_back(Texture());
	texture_buffer[TITLE].loadFromFile("Textures/title.png");
	_window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT,100U),"Snake Ultra",Style::Close);
	_window->setFramerateLimit(MAX_FPS);
	mouse = new Mouse;
	load_levels();
}

bool MainSystem::mainGame(int16_t selectedLevel, uint16_t selectedSnake){
	if (selectedLevel == -1)return false;
	_map = new Map(_levels[selectedLevel], Vector2u(MAP_SIZE_X, MAP_SIZE_Y));

	_snake = new Snake(*_map,selectedSnake);
	_map->spawnTreat();
	_map->spawnSnake(_snake);
	_map->update();


	unsigned score = 0;
	TextOutput score_text(820, 100, 50, default_font, _window);
	TextOutput snake_size_text(820, 150, 50, default_font, _window);
	TextOutput money_text(820, 200, 50, default_font, _window);
	TextButton exit(200, 60, 850, 400, "EXIT",_window, default_font);
	exit.setTextRatio(1);
	exit.setColor(Color::Transparent, default_text_color);


	_song.setBuffer(*_sound_buffer[SONG1]);
	_song.setVolume(_volume);
	_song.play();
	_score = 0;
	_clock.restart();
	float speed_multiplier = 1;
	float difficulty_multiplier = 0.005;
	bool speeded = false;
	uint8_t eaten_treats = 0;
	while (_window->isOpen()) {
		score_text.setValue("Score: "+std::to_string(score) + " points");
		snake_size_text.setValue("Treats: " + std::to_string(eaten_treats)+"/"+to_string(_levels[selectedLevel]->treats_to_victory));
		money_text.setValue("Money: " + std::to_string(_player->_money) + " $");
		
		while (_window->pollEvent(e)) {
			exit.update(&e,mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::KeyPressed:
				switch (e.key.code) {
				case Keyboard::Space:
					if (speeded) {
						speeded = false;
						speed_multiplier = 1;
					}
					else {
						speeded = true;
						speed_multiplier = 0.5;
					}
					break;
				default:
					_snake->input(e.key.code);
					break;
				}
				break;
			}
		}
		if (exit.isPressed()) {
			_song.stop();
			delete _snake; _snake = nullptr;
			delete _map; _map = nullptr;
			return false;
		}
		if (_clock.getElapsedTime().asSeconds() >= (BASE_GAME_SPEED_S)-difficulty_multiplier) {
			_clock.restart();
			_snake->updatePos();
			//GAME OVER
			if (_snake->checkColide()) {
				_song.stop();
				delete _snake; _snake = nullptr;
				delete _map; _map = nullptr;
				return gameOverScreen();
			}
			_map->updateExit();
			_snake->updateSnakeTiles();
			_snake->updateSnakeTextures();
			//EATING TREATH
			if (_snake->getFeed()) {
				eaten_treats++;
				if (eaten_treats >= _levels[selectedLevel]->treats_to_victory)_map->openExit();
				_snake->unFeed();
				difficulty_multiplier += 0.002;
				if (difficulty_multiplier > 0.09)difficulty_multiplier = 0.09;
				
				_map->spawnTreat();
				score += 100*(1- (BASE_GAME_SPEED_S)-difficulty_multiplier);

				_player->_money++;
				//_map->spawnTreat();
			}
		}
		
		exit.draw();
		score_text.draw();
		snake_size_text.draw();
		money_text.draw();
		_map->draw(*_window);
		_window->display();
		//VICTORY
		if (_snake->victory()) {
			_song.stop();
			_player->unlocked_levels[selectedLevel+1] = true;
			delete _snake; _snake = nullptr;
			delete _map; _map = nullptr;
			return victoryScreen(score);
		}

		_window->clear(Color::Yellow);
	}
	_song.stop();
	delete _snake; _snake = nullptr;
	delete _map; _map = nullptr;
	return false;
}

bool MainSystem::gameOverScreen(){
	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(300, 100, 400, 300, "PLAY AGAIN", _window, default_font));
	buttons(new TextButton(300, 100, 400, 400, "MAIN MENU", _window, default_font));
	buttons(new TextButton(300, 100, 400, 500, "EXIT", _window, default_font));

	LOOP(buttons.size()) { 
		buttons[i]->setColor(Color::Transparent, default_text_color);
	}

	TextOutput text(300, 100,100, default_font,_window);
	text.setValue("GAME OVER");
	text.setColor(Color::Red);
	while (_window->isOpen()) {
		_window->clear(Color::White);
		while (_window->pollEvent(e)) {

			LOOP(buttons.size())buttons[i]->update(&e,mouse);


			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			}
		}
		if (buttons[0]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return true;
		}
		else if (buttons[1]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return false;
		}
		else if (buttons[2]->isPressed()) {
			_window->close();
			LOOP(buttons.size())delete buttons[i];
			return false;
		}
		LOOP(buttons.size())buttons[i]->draw();
		text.draw();
		_window->display();
	}
	return false;
}

bool MainSystem::gameOverScreenMul(const unsigned short &player){
	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(300, 100, 400, 300,  "PLAY AGAIN", _window, default_font));
	buttons(new TextButton(300, 100, 400, 400,  "MAIN MENU", _window, default_font));
	buttons(new TextButton(300, 100, 400, 500,  "EXIT", _window, default_font));

	LOOP(buttons.size()) { 
		buttons[i]->setColor(Color::Transparent, default_text_color);
	}

	TextOutput text(300, 100, 100, default_font, _window);
	text.setValue("GAME OVER: PLAYER" + std::to_string(player + 1) + " WIN");
	text.setColor(Color::Red);

	while (_window->isOpen()) {
		_window->clear(Color::White);
		while (_window->pollEvent(e)) {

			LOOP(buttons.size())buttons[i]->update(&e,mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;

			}
		}
		if (buttons[0]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return true;
		}
		else if (buttons[1]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return false;
		}
		else if (buttons[2]->isPressed()) {
			_window->close();
			LOOP(buttons.size())delete buttons[i];
			return false;
		}
		LOOP(buttons.size())buttons[i]->draw();
		text.draw();
		_window->display();
	}
	return false;
}
#ifdef DEBUG
bool MainSystem::selectDebug(){
	uint8_t y = 0;
	while (true) {
		system("CLS");
		cout << ((y == 0) ? ">" : " ") << "Normal game" << endl;
		cout << ((y == 1) ? ">" : " ") << "Map editor" << endl;
		char input = _getwch();
		if (input == 'w')if (y > 0)y--;
		if (input == 's')if (y < 1)y++;
		if (input == 'x') {
			if (y == 0)return false;
			if (y == 1)return true;
		}
	}
	return false;
}
bool MainSystem::debugMenu(){
	cout << "What you wanna du ??? xD" << endl;
	return false;
}
#endif

MainSystem::DIFFICULTY MainSystem::selectDifficulty(){
	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(300, 100, 400, 300, "EASY",_window, default_font));
	buttons(new TextButton(300, 100, 400, 400, "NORMAL",_window, default_font));
	buttons(new TextButton(300, 100, 400, 500, "HARD",_window, default_font));
	buttons(new TextButton(300, 100, 400, 600, "BACK", _window, default_font));
	LOOP(buttons.size()) { 
		buttons[i]->setColor(Color::Transparent, default_text_color);
	}
	
	
	TextOutput text(150,100,80, default_font,_window);
	text.setValue("-SELECT YOUR DIFFICULTY-");
	while (_window->isOpen()) {
		_window->clear(Color::White);
		/*
		if (_waiter.getElapsedTime().asSeconds() > 1) { 
			LOOP(buttons.size())buttons[i]->update(); 
		}*/

		while (_window->pollEvent(e)) {
			LOOP(buttons.size())buttons[i]->update(&e, mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			}
		}
		if (buttons[0]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return EASY;
		}
		else if (buttons[1]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return NORMAL;
		}
		else if (buttons[2]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return HARD;
		}
		else if (buttons[3]->isPressed()) {
			LOOP(buttons.size())delete buttons[i];
			return EXIT;
		}
		text.draw();
		LOOP(buttons.size())buttons[i]->draw();
		_window->display();
	}
	LOOP(buttons.size())delete buttons[i];
}

int16_t MainSystem::selectLevel(){
	vector<TextButton*> levels;
	unsigned off_x = 0;
	unsigned off_y = 0;
	for (int i = 0; i < _levels.size(); i++) {
		levels.push_back(new TextButton(200, 50, 50 + off_x, 50 + off_y, _levels[i]->NAME, _window,default_font));
		levels[i]->setTextRatio(0.8);
		if (!_player->unlocked_levels[i])levels[i]->setColor(Color::Red);
		if (off_y >= 800) {
			off_y = 0;
			off_x += 225;
		}
		else off_y += 50;
	}


	TextButton exit(200, 100, 950, 650, "EXIT", _window, default_font);
	exit.setTextRatio(0.7);

	while (_window->isOpen()) {
		_window->clear(Color::White);

		while (_window->pollEvent(e)) {
			LOOP(levels.size())levels[i]->update(&e,mouse);
			exit.update(&e, mouse);
			if (e.type == Event::Closed) {
				_window->close();
				return -1;
			}
		}
		for (int i = 0; i < levels.size(); i++) {
			if (levels[i]->isPressed()) { 
				if (_player->unlocked_levels[i]) {
					LOOPX(levels.size())delete levels[x];
					return i;
				}
			}
		}
		if (exit.isPressed()) {
			LOOPX(levels.size())delete levels[x];
			return -2;
		}

		exit.draw();
		LOOP(levels.size())levels[i]->draw();
		_window->display();
	}
}

void MainSystem::titleScreen(){
#ifdef DEBUG
	if (selectDebug())debugMenu();
#endif
	static std::uniform_int_distribution<unsigned> randomSnake(0, 5);
	std::uniform_int_distribution<unsigned> randomDirection(1, 4);
	fel::SimplePicture title(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4), Vector2f(1000,WINDOW_HEIGHT/3),&texture_buffer[TITLE]);
	Map *_tmp_map = new Map(20, Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT), Color(255, 141, 0));
	_tmp_map->spawnTreat();
	
	
	_tmp_map->updateNoSnake();
	Snake **snakes = new Snake*[2];
	snakes[0] = new Snake(*_tmp_map, randomSnake(Map::randomGenerator));
	snakes[1] = new Snake(*_tmp_map, randomSnake(Map::randomGenerator));
	//_snake = new Snake(*_map, BASE_GAME_SPEED_S + (0.01), randomSnake(Map::randomGenerator));
	
	snakes[0]->addTail(Vector2u(0, 4));
	snakes[0]->addHead(Vector2u(1, 4));
	snakes[1]->addTail(Vector2u(1, 3));
	snakes[1]->addHead(Vector2u(2, 3));

	_tmp_map->clear();
	LOOP(2) {
		snakes[i]->updatePos();
		snakes[i]->updateSnakeTiles();
		snakes[i]->updateSnakeTextures();
	}

	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(400, 100, 400, 400,  "SINGLE PLAYER", _window, default_font));
	//buttons.end()->addText("SINGLE PLAYER", default_font, 400, 400, 50, default_text_color);
	buttons(new TextButton(400, 100, 400, 500,  "MULTIPLAYER", _window, default_font));
	//buttons.end()->addText("MULTIPLAYER", default_font, 400, 460, 50, default_text_color);
	buttons(new TextButton(400, 100, 400, 600,  "OPTIONS", _window, default_font));
	//buttons.end()->addText("OPTIONS", default_font, 400, 520, 50, default_text_color);
	buttons(new TextButton(400, 100, 400, 700,  "EXIT", _window, default_font));
	//buttons.end()->addText("EXIT", default_font, 400, 580, 50, default_text_color);
	LOOP(buttons.size()) { 
		buttons[i]->setColor(Color::Transparent, default_text_color);
	}

	RectangleShape backgroud(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	backgroud.setFillColor(Color(100,100,100,100));
	Clock move_clock;
	_clock.restart();

	while (_window->isOpen()) {
		_window->clear(Color::White);

		
		//LOOP(buttons.size())buttons[i].isPointed(m.getPosition(*_window), 0, 0, 0, *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		while (_window->pollEvent(e)) {
			LOOP(buttons.size())buttons[i]->update(&e, mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			}
		}
		
		if (buttons[0]->isPressed()) {
			_waiter.restart();
			selected_level = selectLevel();
			if (selected_level != -2) {
				int16_t selectedSnake = selectSnake();
				while (mainGame(selected_level, selectedSnake));
			}
		}
		else if (buttons[1]->isPressed()) {
			_waiter.restart();
			while (multiplayer(selectDifficulty()));
		}
		else if (buttons[2]->isPressed()) {
			_waiter.restart();
			options();
		}
		else if (buttons[3]->isPressed())_window->close();

		if (move_clock.getElapsedTime().asSeconds() > 0.2) {
			LOOP(2)
			switch (randomDirection(Map::randomGenerator)) {
			case 1:
				snakes[i]->input(Keyboard::W);
				break;
			case 2:
				snakes[i]->input(Keyboard::S);
				break;
			case 3:
				snakes[i]->input(Keyboard::A);
				break;
			case 4:
				snakes[i]->input(Keyboard::D);
				break;
			}
			move_clock.restart();
		}
		if (_clock.getElapsedTime().asSeconds() > BASE_GAME_SPEED_S/2) {
			_clock.restart();
			//_tmp_map->clear();
			LOOP(2) {	
				snakes[i]->updatePos();
				snakes[i]->updateSnakeTiles();
				snakes[i]->updateSnakeTextures();
			if (snakes[i]->getFeed()) {
					snakes[i]->unFeed();
				}
			}
			//_tmp_map->updateNoSnake();
		}
		_tmp_map->draw(*_window);
		_window->draw(backgroud);
		LOOP(buttons.size())buttons[i]->draw();
		title.draw(_window);
		_window->display();
	}
	_player->save();
	delete _player;
	_config->save();
	delete _config;
	delete snakes[0], snakes[1];
	snakes[0] = nullptr; snakes[1] = nullptr;
	delete[] snakes;
	LOOP(buttons.size())delete buttons[i];
}

bool MainSystem::victoryScreen(uint16_t score){
	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(300, 100, 400, 300, "NEXT LEVEL", _window, default_font));
	buttons(new TextButton(300, 100, 400, 400, "MAIN MENU", _window, default_font));
	buttons(new TextButton(300, 100, 400, 500, "EXIT", _window, default_font));

	LOOP(buttons.size()) {
		buttons[i]->setColor(Color::Transparent, default_text_color);
	}

	TextOutput text(300, 100, 100, default_font, _window);
	TextOutput score_text(200,200,100,default_font,_window);
	text.setValue("VICTORY");
	text.setColor(Color::Green);
	score_text.setValue("Your score: "+to_string(score)+" p.");
	score_text.setColor(Color::Green);
	while (_window->isOpen()) {
		_window->clear(Color::White);
		
		while (_window->pollEvent(e)) {
			LOOP(buttons.size())buttons[i]->update(&e,mouse);
			if (e.type == Event::Closed) {
				_window->close();
				LOOP(buttons.size())delete buttons[i];
				return false;
			}

		}
		if (buttons[0]->isPressed()) { 
			LOOP(buttons.size())delete buttons[i];
			if (selected_level + 1 < _level_config->level_number) {
				selected_level++;
				return true;
			}
			else return false;
		}
		else if (buttons[1]->isPressed()) { 
			LOOP(buttons.size())delete buttons[i];
			return false; 
		}
		else if (buttons[2]->isPressed()){
			_window->close();
			LOOP(buttons.size())delete buttons[i];
			return false;
		}
		text.draw();
		score_text.draw();
		LOOP(buttons.size())buttons[i]->draw();
		_window->display();
	}
}

Level* MainSystem::load_level(uint8_t n){
	fstream load("Data/levels.dat", ios::in | ios::binary);
	Level* loadedLevel = new Level;

	load.seekg(sizeof(Config_level) + (sizeof(Level) * n));
	load.read((char*)loadedLevel, sizeof(Level));
	cout << load.gcount() << endl;
	load.close();
	return loadedLevel;
}

Config_level* MainSystem::load_level_config()
{
	fstream load("Data/levels.dat", ios::in | ios::binary);
	Config_level* config = new Config_level;
	load.read((char*)config, sizeof(Config_level));
	load.close();
	return config;
}

void MainSystem::load_levels()
{
	_level_config = load_level_config();
	fstream load("Data/levels.dat", ios::in | ios::binary);
	for (int i = 0; i < _level_config->level_number; i++) {
		_levels.push_back(new Level);
		load.seekp(sizeof(Config_level) + (sizeof(Level) * i));
		load.read((char*)_levels[i], sizeof(Level));
	}

	load.close();
}

void MainSystem::options(){

	RectangleShape volumeBar;
	volumeBar.setPosition(250, 100);
	volumeBar.setFillColor(Color::Green);

	TextOutput text1(450,25,60, default_font,_window);
	text1.setValue("Volume");
	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(75, 75, 175, 100, "<",_window, default_font));
	bool decrease = false;
	buttons(new TextButton(75, 75, 850, 100,  ">", _window, default_font));
	bool increase = false;
	buttons(new TextButton(400, 200, 400, 500,  "-BACK-", _window, default_font));
	buttons[0]->setTextRatio(1);
	buttons[1]->setTextRatio(1);
	LOOP(buttons.size()) { 
		buttons[i]->setColor(Color::Transparent,default_text_color);
	}


	while (_window->isOpen()) {
		_window->clear(Color::White);
		
		while (_window->pollEvent(e)) {
			LOOP(buttons.size())buttons[i]->update(&e, mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonReleased:
				increase = false;
				decrease = false;
				break;
			}

		}
		LOOP(buttons.size())
			if (buttons[i]->isPressed())
				switch (i) {
				case 0:
					decrease = true;
					break;
				case 1:
					increase = true;
					break;
				case 2:
					LOOP(buttons.size())delete buttons[i];
					return;
					break;
				}
		if (increase) {
			if (_volume < 100)_volume++;
		}
		if (decrease) {
			if (_volume > 0)_volume--;
		}
		text1.draw();
		LOOP(buttons.size())buttons[i]->draw();
		volumeBar.setSize(Vector2f(6 * _volume, 75));
		volumeBar.setFillColor(Color(255-255*((float)_volume/100), 255 * ((float)_volume / 100),0));
		_config->_volume = _volume;
		_window->draw(volumeBar);
		_window->display();
	}
	LOOP(buttons.size())delete buttons[i];
}

bool MainSystem::multiplayer(const DIFFICULTY &dif) {
	switch (dif) {
	case EASY:
		_map = new Map(10, Vector2u(MAP_SIZE_X, MAP_SIZE_Y), Color::Green);
		break;
	case NORMAL:
		_map = new Map(20, Vector2u(MAP_SIZE_X, MAP_SIZE_Y), Color(255, 141, 0));
		break;
	case HARD:
		_map = new Map(30, Vector2u(MAP_SIZE_X, MAP_SIZE_Y), Color::Red);
		break;
	}
	
	_map->spawnTreat();
	_map->updateNoSnake();
	Snake **snakes = new Snake*[2];
	snakes[0] = new Snake(*_map, 0);
	snakes[1] = new Snake(*_map, 3);

	snakes[0]->addTail(Vector2u(0, 4));
	snakes[0]->addHead(Vector2u(1, 4));
	snakes[1]->addTail(Vector2u(6, 9));
	snakes[1]->addHead(Vector2u(7, 9));

	TextOutput score_text_P1(820, 100, 50, default_font,_window);
	TextOutput score_text_P2(820, 150, 50, default_font,_window);
	TextOutput snake_size_text_P1(820, 200, 50, default_font,_window);
	TextOutput snake_size_text_P2(820, 250, 50, default_font,_window);
	TextOutput speed(820, 300, 50, default_font,_window);


	unsigned long score[2] = {};
	float difficulty_multiplier = 1;
	TextButton exit(200, 60, 850, 400,  "-END-",_window, default_font);
	exit.setColor(Color::Transparent, default_text_color);
	_song.setBuffer(*_sound_buffer[SONG1]);
	_song.setVolume(_volume);
	_song.play();
	
	_map->clear();
	while (_window->isOpen()) {
		_window->clear(Color::White);
		score_text_P1.setValue("Score P1: " + std::to_string(score[0]));
		score_text_P2.setValue("Score P2: " + std::to_string(score[1]));
		snake_size_text_P1.setValue("Size P1: " + std::to_string(snakes[0]->getSize()));
		snake_size_text_P2.setValue("Size P2: " + std::to_string(snakes[1]->getSize()));
		speed.setValue("Speed: " + std::to_string((int)(((BASE_GAME_SPEED_S) / ((BASE_GAME_SPEED_S)*difficulty_multiplier)) * 100)) + " %");
		
		while (_window->pollEvent(e)) {
			exit.update(&e, mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::KeyPressed:
				snakes[0]->inputP1(e.key.code);
				snakes[1]->inputP2(e.key.code);
				break;
			}
		}
		if (exit.isPressed()) {
			_song.stop();
			delete snakes[0], snakes[1];
			delete _map; _map = nullptr;
			snakes[0] = nullptr; snakes[1] = nullptr;
			delete[] snakes;
			return false;
		}
		//update snakes
		if (_clock.getElapsedTime().asSeconds() > (BASE_GAME_SPEED_S)) {
			_clock.restart();
			LOOP(2) {
				snakes[i]->updatePos();
				snakes[i]->updateSnakeTiles();
				snakes[i]->updateSnakeTextures();
				if (snakes[i]->getFeed()) {
					difficulty_multiplier *= (float)dif / 1000;
					if (difficulty_multiplier < 0.1)difficulty_multiplier = 0.1;
					snakes[i]->unFeed();
					score[i] += 100;
					_map->spawnTreat();
				}
			}
			if (checkColide(*snakes[0], *snakes[1])) {
				_song.stop();
				delete snakes[0], snakes[1];
				delete _map; _map = nullptr;
				snakes[0] = nullptr; snakes[1] = nullptr;
				delete[] snakes;
				return gameOverScreenMul(1);
			}
			if (checkColide(*snakes[1], *snakes[0])) {
				_song.stop();
				delete snakes[0], snakes[1];
				delete _map; _map = nullptr;
				snakes[0] = nullptr; snakes[1] = nullptr;
				delete[] snakes;
				return gameOverScreenMul(0);
			}
			//_map->updateNoSnake();

		}
		
		score_text_P1.draw();
		score_text_P2.draw();
		snake_size_text_P1.draw();
		snake_size_text_P2.draw();
		speed.draw();
		exit.draw();
		_map->draw(*_window);
		_window->display();
	}
	_song.stop();
	delete snakes[0], snakes[1];
	delete _map; _map = nullptr;
	snakes[0] = nullptr; snakes[1] = nullptr;
	delete[] snakes;
	return false;
}

unsigned short MainSystem::selectSnake() {
	fl::DynamicArray<TextButton*> buttons;
	buttons(new TextButton(80, 100, 200, 250,  "<",_window, default_font));
	buttons(new TextButton(80, 100, 850, 250,  ">", _window, default_font));
	buttons(new TextButton(300, 100, 400, 600,  "SELECT", _window, default_font));
	buttons[0]->setTextRatio(1);
	buttons[1]->setTextRatio(1);
	LOOP(buttons.size()) {
		buttons[i]->setColor(Color::Transparent, default_text_color);
	}



	Map::Tile **snake = new Map::Tile*[4];
	LOOP(4)snake[i] = new Map::Tile(Vector2f(200, 200), Vector2f(300 + 100 * i, 200));
	snake[0]->setType(Map::SNAKE_TAIL);
	snake[1]->setType(Map::SNAKE_BODYI);
	snake[2]->setType(Map::SNAKE_BODYI);
	snake[3]->setType(Map::SNAKE_HEAD);


	TextOutput text(175, 100, 100, default_font,_window);
	text.setValue("Choose your SNAKE!!!!");
	TextOutput lock_unlock_text( 250, 400, 50, default_font,_window);
	lock_unlock_text.setValue("LOCKED");
	TextOutput price(250, 450, 50, default_font,_window);
	TextOutput money(250, 500, 50, default_font,_window);



	static int short type = 0;
	while (_window->isOpen()) {
		_window->clear(Color::White);
		while (_window->pollEvent(e)) {
			LOOP(buttons.size())buttons[i]->update(&e, mouse);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				LOOP(4)delete snake[i];
				delete[] snake;
				return 0;
				break;
			case Event::KeyPressed:
				if (e.key.code == Keyboard::D || e.key.code == Keyboard::Right) {
					type++;
					if (type > NUM_OF_SNAKES - 1)type = 0;
				}
				if (e.key.code == Keyboard::A || e.key.code == Keyboard::Left) {
					type--;
					if (type < 0)type = NUM_OF_SNAKES - 1;
				}
				if (e.key.code == Keyboard::Enter) {
					if (_player->unlocked_snakes[type]) {
						LOOP(buttons.size())delete buttons[i];
						LOOP(4)delete snake[i];
						delete[] snake;
						return type;
					}
					else {
						if (_player->_money >= _snake_pricec[type]) {
							_player->_money -= _snake_pricec[type];
							_player->unlocked_snakes[type] = true;
						}
					}
				}
				break;

			}
		}
		LOOP(buttons.size()) {
			if (buttons[i]->isPressed()) {
				switch (i) {
				case 0:
					type--;
					if (type < 0)type = NUM_OF_SNAKES - 1;
					break;
				case 1:
					type++;
					if (type > NUM_OF_SNAKES - 1)type = 0;
					break;
				case 2:
					if (_player->unlocked_snakes[type]) {
						LOOP(buttons.size())delete buttons[i];
						LOOP(4)delete snake[i];
						delete[] snake;
						return type;
					}
					else {
						if (_player->_money >= _snake_pricec[type]) {
							_player->_money -= _snake_pricec[type];
							_player->unlocked_snakes[type] = true;
						}
					}
					break;
				}
			}
		}
		
		text.draw();
		if (_player->unlocked_snakes[type]) {
			lock_unlock_text.setValue("UNLOCKED");
			buttons.end()->setString("SELECT");
		}
		else {
			lock_unlock_text.setValue("LOCKED");
			price.setValue("UNLOCK FOR " + std::to_string(_snake_pricec[type]) + " $");
			money.setValue("YOU HAVE : " + std::to_string(_player->_money) + " $");
			if(_snake_pricec[type]<=_player->_money)buttons.end()->setString("PURCHASE");
			else { buttons.end()->setString("NOT ENOUGH MONEY!"); }
			price.draw();
			money.draw();
		}
		LOOP(buttons.size())buttons[i]->draw();
		lock_unlock_text.draw();
		LOOP(4)snake[i]->updateSnakeParts(type);
		LOOP(4)snake[i]->draw(*_window);
		_window->display();
	}
	LOOP(buttons.size())delete buttons[i];
	LOOP(4)delete snake[i];
	delete[] snake;
}