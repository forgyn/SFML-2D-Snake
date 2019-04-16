#include "MainSystem.h"
#include <iostream>

sf::Font MainSystem::default_font = sf::Font();
Color MainSystem::default_text_color = Color(100,200,100);
unsigned short MainSystem::_volume = 100;
std::vector<SoundBuffer*> MainSystem::_sound_buffer = std::vector<SoundBuffer*>();

MainSystem::MainSystem(){
	_player = new Player;
	load_player();
	_volume = _player->_volume;
	MainSystem::_sound_buffer.push_back(new SoundBuffer());
	MainSystem::_sound_buffer.back()->loadFromFile("Sounds/eating_sound_1.ogg");
	MainSystem::_sound_buffer.push_back(new SoundBuffer());
	MainSystem::_sound_buffer.back()->loadFromFile("Sounds/YoureGonnaGoFarKidEurobeatRemix.ogg");
	_score = 0;
	default_font.loadFromFile("Fonts/font2.ttf");
	Map::Tile::_texture.loadFromFile("Textures/Snake.png");
	texture_buffer.push_back(Texture());
	texture_buffer[TITLE].loadFromFile("Textures/title.png");
	_window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT,100U),"Snake Ultra");
	_window->setFramerateLimit(MAX_FPS);
}

bool MainSystem::mainGame(const DIFFICULTY &dif){

	std::uniform_int_distribution<unsigned> randomSnake(0, 5);
	unsigned long hight_score;
	switch (dif) {
	case EASY:
		_map = new Map(10, Vector2u(MAP_SIZE_X, MAP_SIZE_Y), Color::Green);
		hight_score = _player->_hight_score_easy;
		break;
	case NORMAL:
		_map = new Map(20, Vector2u(MAP_SIZE_X, MAP_SIZE_Y), Color(255,141,0));
		hight_score = _player->_hight_score_normal;
		break;
	case HARD:
		_map = new Map(30, Vector2u(MAP_SIZE_X, MAP_SIZE_Y), Color::Red);
		hight_score = _player->_hight_score_hard;
		break;
	}
	_snake = new Snake(*_map,selectSnake());
	_map->spawnTreat();
	_snake->addTail(Vector2u(0, 0));
	_snake->addHead(Vector2u(1, 0));
	_map->update();


	SimpleText score_text(default_font, 820, 100,50);
	SimpleText snake_size_text(default_font, 820, 150,50);
	SimpleText money_text(default_font, 820, 200, 50);
	SimpleText hight_score_t(default_font, 820, 250, 50);
	SimpleText speed(default_font, 820, 300, 50);
	SimpleButton exit(850, 400, 200, 60, Color::Transparent);
	exit.addText("-END-", default_font, 850,400, 50, default_text_color);
	
	_song.setBuffer(*_sound_buffer[SONG1]);
	_song.setVolume(_volume);
	_song.play();
	_score = 0;
	_clock.restart();
	float speed_multiplier = 1;
	float difficulty_multiplier = 1;
	bool speeded = false;
	while (_window->isOpen()) {
		Mouse m;
		score_text.text.setString("Score: "+std::to_string(_score) + " points");
		snake_size_text.text.setString("Snake size: " + std::to_string(_snake->getSize()-2));
		money_text.text.setString("Money: " + std::to_string(_player->_money) + " $");
		hight_score_t.text.setString("Hight score: " + std::to_string(hight_score));
		speed.text.setString("Speed: " + std::to_string((int)(((BASE_GAME_SPEED_S)/((BASE_GAME_SPEED_S)*speed_multiplier*difficulty_multiplier))*100))+" %");
		exit.isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		Event e;
		while (_window->pollEvent(e)) {
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
			case Event::MouseButtonPressed:
				if (exit.isPointed(m.getPosition(*_window), Vector3f(100, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					_song.stop();
					delete _snake; _snake = nullptr;
					delete _map; _map = nullptr;
					return false;
				}
				break;
			}
		}
		if (_clock.getElapsedTime().asSeconds() >= (BASE_GAME_SPEED_S)*speed_multiplier*difficulty_multiplier) {
			_clock.restart();
			_snake->updatePos();
			if (_snake->checkColide()) {
				_song.stop();
				delete _snake; _snake = nullptr;
				delete _map; _map = nullptr;
				return gameOverScreen();
			}
			_snake->updateSnakeTiles();
			_snake->updateSnakeTextures();
			if (_snake->getFeed()) {
				_snake->unFeed();
				difficulty_multiplier *= (float)dif/1000;
				if (difficulty_multiplier < 0.1)difficulty_multiplier = 0.1;
				_map->spawnTreat();
				_score += 100;
				if (_score >= hight_score) {
					
					hight_score = _score;
					switch (dif) {
					case EASY:
						_player->_hight_score_easy = hight_score;
						break;
					case NORMAL:
						_player->_hight_score_normal = hight_score;
						break;
					case HARD:
						_player->_hight_score_hard = hight_score;
						break;
					}
				}
				_player->_money++;
				//_map->spawnTreat();
			}
		}
		
		exit.drawButton(*_window);
		score_text.draw(*_window);
		snake_size_text.draw(*_window);
		money_text.draw(*_window);
		hight_score_t.draw(*_window);
		speed.draw(*_window);
		_map->draw(*_window);
		_window->display();
		_window->clear(Color::Yellow);
	}
	_song.stop();
	delete _snake; _snake = nullptr;
	delete _map; _map = nullptr;
	return false;
}

bool MainSystem::gameOverScreen(){
	fl::DynamicArray<SimpleButton*> buttons;
	buttons(new SimpleButton(400, 300, 300, 60, Color::Transparent));
	buttons.end()->addText("PLAY AGAIN", default_font, 400, 300, 50, default_text_color);
	buttons(new SimpleButton(400, 360, 200, 60, Color::Transparent));
	buttons.end()->addText("MAIN MENU", default_font, 400, 360, 50, default_text_color);
	buttons(new SimpleButton(400, 420, 200, 60, Color::Transparent));
	buttons.end()->addText("EXIT", default_font, 400, 420, 50, default_text_color);
	SimpleText text(default_font, 300, 100,100);
	text.text.setString("GAME OVER");
	text.text.setFillColor(Color::Red);
	while (_window->isOpen()) {
		_window->clear(Color::White);
		Event e;
		Mouse m;
		while (_window->pollEvent(e)) {

			LOOP(buttons.size())buttons[i]->isPointed(m.getPosition(*_window), Vector3f(255, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT);



			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonPressed:
				if (buttons[0]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT))return true;
				else if (buttons[1]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT))return false;
				else if (buttons[2]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)){
					_window->close();
					return false;
				}
				break;

			}
		}
		LOOP(buttons.size())buttons[i]->drawButton(*_window);
		text.draw(*_window);
		_window->display();
	}
	return false;
}

bool MainSystem::gameOverScreenMul(const unsigned short &player){
	fl::DynamicArray<SimpleButton*> buttons;
	buttons(new SimpleButton(400, 300, 300, 60, Color::Transparent));
	buttons.end()->addText("PLAY AGAIN", default_font, 400, 300, 50, default_text_color);
	buttons(new SimpleButton(400, 360, 200, 60, Color::Transparent));
	buttons.end()->addText("MAIN MENU", default_font, 400, 360, 50, default_text_color);
	buttons(new SimpleButton(400, 420, 200, 60, Color::Transparent));
	buttons.end()->addText("EXIT", default_font, 400, 420, 50, default_text_color);
	SimpleText text(default_font, 300, 100, 50);
	text.text.setString("GAME OVER: PLAYER"+std::to_string(player+1)+" WIN");
	text.text.setFillColor(Color::Red);
	while (_window->isOpen()) {
		_window->clear(Color::White);
		Event e;
		Mouse m;
		while (_window->pollEvent(e)) {

			LOOP(buttons.size())buttons[i]->isPointed(m.getPosition(*_window), Vector3f(255, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonPressed:
				if (buttons[0]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT))return true;
				else if (buttons[1]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT))return false;
				else if (buttons[2]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					_window->close();
					return false;
				}
				break;

			}
		}
		LOOP(buttons.size())buttons[i]->drawButton(*_window);
		text.draw(*_window);
		_window->display();
	}
	return false;
}

MainSystem::DIFFICULTY MainSystem::selectDifficulty(){
	fl::DynamicArray<SimpleButton*> buttons;
	buttons(new SimpleButton(400, 300, 300, 60, Color::Transparent));
	buttons.end()->addText("EASY", default_font, 400, 300, 50, default_text_color);
	buttons(new SimpleButton(400, 360, 200, 60, Color::Transparent));
	buttons.end()->addText("NORMAL", default_font, 400, 360, 50, default_text_color);
	buttons(new SimpleButton(400, 420, 200, 60, Color::Transparent));
	buttons.end()->addText("HARD", default_font, 400, 420, 50, default_text_color);
	//buttons(new SimpleButton(400, 480, 200, 60, Color::Transparent));
	//buttons.end()->addText("BACK", default_font, 400, 480, 50, default_text_color);
	SimpleText text(default_font,150,100,80);
	text.text.setString("-SELECT YOUR DIFFICULTY-");
	while (_window->isOpen()) {
		_window->clear(Color::White);
		Event e;
		Mouse m;

		LOOP(buttons.size())buttons[i]->isPointed(m.getPosition(*_window), 255, 0, 0, *_window, WINDOW_WIDTH, WINDOW_HEIGHT);

		while (_window->pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonPressed:
				if (buttons[0]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) { 
					LOOP(buttons.size())delete buttons[i];
					return EASY; }
				else if (buttons[1]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					LOOP(buttons.size())delete buttons[i];
					return NORMAL;
				}
				else if (buttons[2]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					LOOP(buttons.size())delete buttons[i];
					return HARD;
				}
				break;
			}
		}
		text.draw(*_window);
		LOOP(buttons.size())buttons[i]->drawButton(*_window);
		_window->display();
	}
	LOOP(buttons.size())delete buttons[i];
}

void MainSystem::titleScreen(){
	static std::uniform_int_distribution<unsigned> randomSnake(0, 5);
	std::uniform_int_distribution<unsigned> randomDirection(1, 4);
	SimplePicture title(Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4), Vector2f(1000,WINDOW_HEIGHT/3),&texture_buffer[TITLE]);
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

	fl::DynamicArray<SimpleButton*> buttons;
	buttons(new SimpleButton(400, 400, 300, 60, Color::Transparent));
	buttons.end()->addText("SINGLE PLAYER", default_font, 400, 400, 50, default_text_color);
	buttons(new SimpleButton(400, 460, 200, 60, Color::Transparent));
	buttons.end()->addText("MULTIPLAYER", default_font, 400, 460, 50, default_text_color);
	buttons(new SimpleButton(400, 520, 200, 60, Color::Transparent));
	buttons.end()->addText("OPTIONS", default_font, 400, 520, 50, default_text_color);
	buttons(new SimpleButton(400, 580, 200, 60, Color::Transparent));
	buttons.end()->addText("EXIT", default_font, 400, 580, 50, default_text_color);
	RectangleShape backgroud(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	backgroud.setFillColor(Color(100,100,100,100));
	Clock move_clock;
	_clock.restart();
	while (_window->isOpen()) {
		_window->clear(Color::White);
#ifdef DEBUG
		std::cout << "Befor clear" << std::endl;
		_tmp_map->showBitmap();
#endif

		Event e;
		Mouse m;
		LOOP(buttons.size())buttons[i]->isPointed(m.getPosition(*_window), 0, 0, 0, *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		//LOOP(buttons.size())buttons[i].isPointed(m.getPosition(*_window), 0, 0, 0, *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		while (_window->pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonPressed:
				if (buttons[0]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) { 
					while(mainGame(selectDifficulty())); 
				}
				else if (buttons[1]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					while (multiplayer(selectDifficulty()));
				}
				else if (buttons[2]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					options();
				}
				else if (buttons[3]->isPointed(m.getPosition(*_window),Vector3f(0,0,0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT))_window->close();
				break;
			}
		}
		
#ifdef DEBUG
		std::cout << "After snake update" << std::endl;
		_tmp_map->showBitmap();
#endif
		
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
		LOOP(buttons.size())buttons[i]->drawButton(*_window);
		title.draw(_window);
		_window->display();
	}
	save_player();
	delete _player;
	delete snakes[0], snakes[1];
	snakes[0] = nullptr; snakes[1] = nullptr;
	delete[] snakes;
	LOOP(buttons.size())delete buttons[i];
}

void MainSystem::save_player() {
	std::fstream save("Data/player.dat", std::ios::binary | std::ios::out);
	save.write((char*)(_player), sizeof(Player));
	save.close();
}

void MainSystem::load_player() {
	std::fstream load("Data/player.dat", std::ios::binary | std::ios::in);
	load.read((char*)(_player), sizeof(Player));
	load.close();

}

void MainSystem::options(){

	RectangleShape volumeBar;
	volumeBar.setPosition(260, 100);
	volumeBar.setFillColor(Color::Green);

	SimpleText text1(default_font,450,25,60);
	text1.text.setString("Volume");
	fl::DynamicArray<SimpleButton*> buttons;
	buttons(new SimpleButton(150, 100, 80, 100, Color::Transparent));
	buttons.end()->addText("<", default_font, 150, 0, 200, Color::Black);
	bool decrease = false;
	buttons(new SimpleButton(900, 100, 80, 100, Color::Transparent));
	buttons.end()->addText(">", default_font, 900, 0, 200, Color::Black);
	bool increase = false;
	buttons(new SimpleButton(400, 500, 300, 75, Color::Transparent));
	buttons.end()->addText("-BACK-", default_font, 400, 500, 75, default_text_color);
	while (_window->isOpen()) {
		_window->clear(Color::White);
		Mouse m;
		Event e;
		buttons[2]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		while (_window->pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonPressed:
				LOOP(buttons.size())
				if (buttons[i]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT))
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
				break;
			case Event::MouseButtonReleased:
				increase = false;
				decrease = false;
				break;
			}

		}
		if (increase) {
			if (_volume < 100)_volume++;
		}
		if (decrease) {
			if (_volume > 0)_volume--;
		}
		text1.draw(*_window);
		LOOP(buttons.size())buttons[i]->drawButton(*_window);
		volumeBar.setSize(Vector2f(6 * _volume, 75));
		volumeBar.setFillColor(Color(255-255*((float)_volume/100), 255 * ((float)_volume / 100),0));
		_player->_volume = _volume;
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

	SimpleText score_text_P1(default_font, 820, 100, 50);
	SimpleText score_text_P2(default_font, 820, 150, 50);
	SimpleText snake_size_text_P1(default_font, 820, 200, 50);
	SimpleText snake_size_text_P2(default_font, 820, 250, 50);
	SimpleText speed(default_font, 820, 300, 50);
	unsigned long score[2] = {};
	float difficulty_multiplier = 1;
	SimpleButton exit(850, 400, 200, 60, Color::Transparent);
	exit.addText("-END-", default_font, 850, 400, 50, default_text_color);

	_song.setBuffer(*_sound_buffer[SONG1]);
	_song.setVolume(_volume);
	_song.play();
	
	_map->clear();
	while (_window->isOpen()) {
		_window->clear(Color::White);
		score_text_P1.text.setString("Score P1: " + std::to_string(score[0]));
		score_text_P2.text.setString("Score P2: " + std::to_string(score[1]));
		snake_size_text_P1.text.setString("Size P1: " + std::to_string(snakes[0]->getSize()));
		snake_size_text_P2.text.setString("Size P2: " + std::to_string(snakes[1]->getSize()));
		speed.text.setString("Speed: " + std::to_string((int)(((BASE_GAME_SPEED_S) / ((BASE_GAME_SPEED_S)*difficulty_multiplier)) * 100)) + " %");
		Event e;
		Mouse m;
		exit.isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		while (_window->pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				_window->close();
				break;
			case Event::MouseButtonPressed:
				if (exit.isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
					_song.stop();
					delete snakes[0], snakes[1];
					delete _map; _map = nullptr;
					snakes[0] = nullptr; snakes[1] = nullptr;
					delete[] snakes;
					return false;
				}
				break;
			case Event::KeyPressed:
				snakes[0]->inputP1(e.key.code);
				snakes[1]->inputP2(e.key.code);
				break;
			}
		}
		//update snakes
		if (_clock.getElapsedTime().asSeconds() > (BASE_GAME_SPEED_S)*difficulty_multiplier) {
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
		
		score_text_P1.draw(*_window);
		score_text_P2.draw(*_window);
		snake_size_text_P1.draw(*_window);
		snake_size_text_P2.draw(*_window);
		speed.draw(*_window);
		exit.drawButton(*_window);
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
	fl::DynamicArray<SimpleButton*> buttons;
	buttons(new SimpleButton(200, 250, 80, 100, Color::Transparent));
	buttons.end()->addText("<", default_font, 200, 150, 200, default_text_color);
	buttons(new SimpleButton(850, 250, 80, 100, Color::Transparent));
	buttons.end()->addText(">", default_font, 850, 150, 200, default_text_color);
	buttons(new SimpleButton(400, 600, 300, 100, Color::Transparent));
	buttons.end()->addText("SELECT", default_font, 400, 600, 75, default_text_color);
	Map::Tile **snake = new Map::Tile*[4];
	LOOP(4)snake[i] = new Map::Tile(Vector2f(200, 200), Vector2f(300 + 100 * i, 200));
	snake[0]->setType(Map::SNAKE_TAIL);
	snake[1]->setType(Map::SNAKE_BODYI);
	snake[2]->setType(Map::SNAKE_BODYI);
	snake[3]->setType(Map::SNAKE_HEAD);
	SimpleText text(default_font, 175, 100, 100);
	text.text.setString("Choose your SNAKE!!!!");
	SimpleText lock_unlock_text(default_font, 250, 400, 50);
	lock_unlock_text.text.setString("LOCKED");
	SimpleText price(default_font, 250, 450, 50);
	SimpleText money(default_font, 250, 500, 50);




	int short type = 0;
	while (_window->isOpen()) {
		_window->clear(Color::White);
		Mouse m;
		LOOP(buttons.size())buttons[i]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT);
		Event e;
		while (_window->pollEvent(e)) {
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
			case Event::MouseButtonPressed: 
					LOOP(buttons.size()) {
						if (buttons[i]->isPointed(m.getPosition(*_window), Vector3f(0, 0, 0), *_window, WINDOW_WIDTH, WINDOW_HEIGHT)) {
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
				break;
			}
		}
		text.draw(*_window);
		if (_player->unlocked_snakes[type]) {
			lock_unlock_text.text.setString("UNLOCKED");
			buttons.end()->changeString("SELECT");
		}
		else {
			lock_unlock_text.text.setString("LOCKED");
			price.text.setString("UNLOCK FOR " + std::to_string(_snake_pricec[type]) + " $");
			money.text.setString("YOU HAVE : " + std::to_string(_player->_money) + " $");
			if(_snake_pricec[type]<=_player->_money)buttons.end()->changeString("PURCHASE");
			else { buttons.end()->changeString("NOT ENOUGH MONEY!"); }
			price.draw(*_window);
			money.draw(*_window);
		}
		LOOP(buttons.size())buttons[i]->drawButton(*_window);
		lock_unlock_text.draw(*_window);
		LOOP(4)snake[i]->updateSnakeParts(type);
		LOOP(4)snake[i]->draw(*_window);
		_window->display();
	}
	LOOP(buttons.size())delete buttons[i];
	LOOP(4)delete snake[i];
	delete[] snake;
}