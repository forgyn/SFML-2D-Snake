#include "Snake.h"
#include "Map.h"
#include "MainSystem.h"


Snake::Snake(Map &map,const unsigned short &type)
	:_map(&map),type(type){
	//starting direction
	_direction = RIGHT;
	_state = NORMAL;
	_sound_effect.setVolume(MainSystem::_volume);
	_feeded = false;
}

bool Snake::addHead(Vector2u pos){
	_map->setTileType(pos, Map::SNAKE_HEAD);
	_part_pos.push_back(pos);
	_snake_fat_body.push_back(false);
	return true;
}

bool Snake::moveSnake(const int &x, const int &y) {
	if (x > (int)_map->getSize()-1) {
		nextPos = Vector2u(0, y);
	}
	else if (y > (int)_map->getSize()-1) {
		nextPos = Vector2u(x, 0);
	}
	else if (x < 0) {
		nextPos = Vector2u((int)_map->getSize() - 1, y);
	}
	else if (y < 0) {
		nextPos = Vector2u(x, (int)_map->getSize() - 1);
	}
	else {
		nextPos = Vector2u(x, y);
	}

	if (nextPos != _part_pos[_part_pos.size() - 2]) {
	if (_map->checkTile(nextPos, Map::TREAT) || _map->checkTile(nextPos,Map::FINISH)) {
		if (_map->checkTile(nextPos, Map::FINISH))_VICTORY = true;
			_map->setTileType(nextPos, Map::SNAKE_HEAD);
			_part_pos.push_back(nextPos);
			_snake_fat_body.push_back(true);
			_sound_effect.setBuffer(*MainSystem::_sound_buffer[MainSystem::EATING_SOUND]);
			_sound_effect.play();
			_feeded = true;
			//_map->spawnTreat();
		return true;
	}
	
	else /*if (_map->checkTile(nextPos, Map::AIR))*/ {
		_map->resetTile(_part_pos.front());
			for (int i = 0; i < _part_pos.size() - 1; i++) {
				_part_pos[i] = _part_pos[i + 1];
			}
			_part_pos.back() = nextPos;
			return true;
		}
	//else return false;
	}
	else {
		switch (_direction) {
		case LEFT:
			_direction = RIGHT;
			if (!moveSnake((int)_part_pos.back().x + 1, _part_pos.back().y));// return false;
			break;
		case RIGHT:
			_direction = LEFT;
			if (!moveSnake(_part_pos.back().x - 1, _part_pos.back().y));// return false;
			break;
		case UP:
			_direction = DOWN;
			if (!moveSnake(_part_pos.back().x, _part_pos.back().y - 1));// return false;
			break;
		case DOWN:
			_direction = UP;
			if (!moveSnake(_part_pos.back().x, (int)_part_pos.back().y + 1));// return false;
			break;
		}
	}
	
}
bool Snake::addHead(const int & x, const int & y){
	_map->setTileType(Vector2u(x, y), Map::SNAKE_HEAD);
	_map->rotateTile(Vector2u(x, y), _direction);
	_part_pos.push_back(Vector2u(x, y));
	_snake_fat_body.push_back(false);
	return true;
}
bool Snake::addBody(Vector2u pos){
	_map->setTileType(pos, Map::SNAKE_BODYI);
	_part_pos.push_back(pos);
	return true;
}
bool Snake::addBody(const unsigned &x,const unsigned &y) {
	_map->setTileType(Vector2u(x, y), Map::SNAKE_BODYI);
	_part_pos.push_back(Vector2u(x, y));
	return true;
}
bool Snake::addTail(Vector2u pos){
	_map->setTileType(pos, Map::SNAKE_TAIL);
	_part_pos.push_back(pos);
	_snake_fat_body.push_back(false);
	return false;
}
bool Snake::addTail(const unsigned &x, const unsigned &y) {
	_map->setTileType(Vector2u(x, y), Map::SNAKE_TAIL);
	_part_pos.push_back(Vector2u(x, y));
	_snake_fat_body.push_back(false);
	return false;
}

inline unsigned Snake::isOnEdge(const Vector2u & pos){
	if (pos.y == 0 && pos.x != 0)
		if (pos.y == 0 && pos.x != _map->getSize() - 1)
			return 1;
	if (pos.x == _map->getSize() - 1 && pos.y != _map->getSize() - 1)
		if (pos.x == _map->getSize() - 1 && pos.y != 0)
			return 2;
	if (pos.y == _map->getSize() - 1 && pos.x != _map->getSize() - 1)
		if (pos.y == _map->getSize() - 1 && pos.x != 0)
			return 3;
	if (pos.x == 0 && pos.y != 0)
		if (pos.x == 0 && pos.y != _map->getSize() - 1)
			return 4;							
	return 0;
}

inline unsigned Snake::isInCorner(const Vector2u & pos){
	if (pos.x == 0 && pos.y == 0)return 1;
	if (pos.x == _map->getSize() - 1 && pos.y == _map->getSize() - 1)return 3;
	if (pos.x == 0 && pos.y == _map->getSize() - 1) return 4;
	if (pos.x == _map->getSize() - 1 && pos.y == 0) return 2;
	return 0;
}

inline void Snake::moveFat(){
	for (int i = 0; i < _snake_fat_body.size()-1; i++) {
		_snake_fat_body[i] = _snake_fat_body[i + 1];
	}
	_snake_fat_body.back() = false;
}

void Snake::input(const sf::Keyboard::Key &key){
	_prev_direction = _direction;
	switch (key) {
	case Keyboard::W:
	case Keyboard::Up:
		if(_direction!=UP){
			_direction = DOWN; 	
		}
		break;
	case Keyboard::S:
	case Keyboard::Down:
		if (_direction != DOWN) {
			_direction = UP;
		}
		
		break;
	case Keyboard::A:
	case Keyboard::Left:
		if (_direction != RIGHT) {
			_direction = LEFT;
		}
	
		
		break;
	case Keyboard::D:
	case Keyboard::Right:
		if (_direction != LEFT) {
			_direction = RIGHT;
		}
		
		
		break;
	case Keyboard::BackSpace:
	case Keyboard::Escape:
		if(_state != PAUSE) _state = PAUSE;
		else _state = NORMAL;
		break;
	}
	
}
void Snake::inputP1(const sf::Keyboard::Key & key){
	_prev_direction = _direction;
	switch (key) {
	case Keyboard::W:
		if (_direction != UP) {
			_direction = DOWN;
		}
		break;
	case Keyboard::S:
		if (_direction != DOWN) {
			_direction = UP;
		}

		break;
	case Keyboard::A:
		if (_direction != RIGHT) {
			_direction = LEFT;
		}


		break;
	case Keyboard::D:
		if (_direction != LEFT) {
			_direction = RIGHT;
		}
		break;
	}
}
void Snake::inputP2(const sf::Keyboard::Key & key){
	_prev_direction = _direction;
	switch (key) {
	case Keyboard::Up:
		if (_direction != UP) {
			_direction = DOWN;
		}
		break;
	case Keyboard::Down:
		if (_direction != DOWN) {
			_direction = UP;
		}

		break;
	case Keyboard::Left:
		if (_direction != RIGHT) {
			_direction = LEFT;
		}


		break;
	case Keyboard::Right:
		if (_direction != LEFT) {
			_direction = RIGHT;
		}


		break;
	}
}
bool Snake::checkColide(){
	//check collide with itself
	LOOP(_part_pos.size() - 1) {
		if (_part_pos.back() == _part_pos[i])return true;
	}
	if (_map->checkTile(nextPos, Map::OBSTACLE))return true;
	return false;
}
bool checkColide(const Snake &s1,const Snake &s2){
	//check collide with other snake
	LOOP(s2._part_pos.size()) {
		if (s1._part_pos.back() == s2._part_pos[i])return true;
	}
	//check collide with itself
	LOOP(s1._part_pos.size()-1) {
		if (s1._part_pos.back() == s1._part_pos[i])return true;
	}
	return false;
}
inline bool Snake::updatePos(){
	switch (_direction) {
	case LEFT:
		if (!moveSnake((int)_part_pos.back().x - 1, _part_pos.back().y))return false;
		break;
	case RIGHT:
		if (!moveSnake(_part_pos.back().x + 1, _part_pos.back().y))return false;
		break;
	case UP:
		if (!moveSnake(_part_pos.back().x, _part_pos.back().y + 1))return false;
		break;
	case DOWN:
		if (!moveSnake(_part_pos.back().x, (int)_part_pos.back().y - 1))return false;
		break;
	}
	return true;
}
inline void Snake::updateSnakeTiles(){
	//update snakes parts, sets map tile types
	updateHead();
	updateBody();
	updateTail();
}
void Snake::updateSnakeTextures(){
	//update map with snake parts, set maps tile textures
	for (size_t i = 0; i < _part_pos.size(); i++)_map->updateSnakeTile(_part_pos[i], type);
}
bool Snake::update(){
	/*
	if (_state != PAUSE) {
		bool gameOver = false;
		if (_clock.getElapsedTime().asSeconds() > _speed/_state) {
				switch (_direction) {
				case LEFT:
					if (!moveSnake((int)_part_pos.back().x - 1, _part_pos.back().y))gameOver = true;// return false;
					break;
				case RIGHT:
					if (!moveSnake(_part_pos.back().x + 1, _part_pos.back().y))gameOver = true;// return false;
					break;
				case UP:
					if (!moveSnake(_part_pos.back().x, _part_pos.back().y + 1))gameOver = true;// return false;
					break;
				case DOWN:
					if (!moveSnake(_part_pos.back().x, (int)_part_pos.back().y - 1))gameOver = true;// return false;
					break;
				}

			//clean map
			_map->clear();

			//update snakes parts, sets map tile types
			updateHead();
			updateBody();
			updateTail();
			//spawn treat if snake have eaten one
			if (_feeded)_map->spawnTreat();

			//gameo over
			if (gameOver)return false;
#ifdef DEBUG
			//std::cout << "After moving snake" << std::endl;
			//_map->showBitmap();
#endif
			_clock.restart();
			
		}
		///bugy ASF
		//update map with snake parts, set maps tile textures
		//for (size_t i = 0; i < _part_pos.size(); i++)_map->updateTile(_part_pos[i], type);

		return true;
	}
	*/
	if (_state != PAUSE) {
		//static bool gameOver = false;

		if (!updatePos())return false;

		//clean map
		_map->clear();

		updateSnakeTiles();
		
		//spawn treat if snake have eaten one
		if (_feeded)_map->spawnTreat();

		///bugy ASF
		//update map with snake parts, set maps tile textures
		//for (size_t i = 0; i < _part_pos.size(); i++)_map->updateTile(_part_pos[i], type);
		return true;
	}
}


Snake::~Snake(){}

inline void Snake::updateHead(){
	//change the head angle
	_map->setTileType(_part_pos.back(), Map::SNAKE_HEAD);
	_map->rotateTile(_part_pos.back(), _direction);
#ifdef DEBUG
	std::cout << "Head pos: x = " << _part_pos.back().x << " ; y = " << _part_pos.back().y << std::endl;
#endif
}

inline void Snake::updateBody(){
	moveFat();
	//change the snake bodys angle
	for (int i = 1; i < _part_pos.size() - 1; i++) {
		if (_part_pos[i].x == _part_pos[i + 1].x && _part_pos[i].x == _part_pos[i - 1].x) {
			if (_part_pos[i].y > _part_pos[i + 1].y)_map->rotateTile(_part_pos[i], DOWN);
			else _map->rotateTile(_part_pos[i], UP);
			if(_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYO);
			else _map->setTileType(_part_pos[i], Map::SNAKE_BODYI);
#ifdef DEBUG
			std::cout << i << " SNAKE_BODYI : UP" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif // DEBUG

		}
		else if (_part_pos[i].y == _part_pos[i + 1].y && _part_pos[i].y == _part_pos[i - 1].y) {
			if (_part_pos[i].x > _part_pos[i + 1].x)_map->rotateTile(_part_pos[i], LEFT);
			else _map->rotateTile(_part_pos[i], RIGHT);
			if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYO);
			else _map->setTileType(_part_pos[i], Map::SNAKE_BODYI);
#ifdef DEBUG
			std::cout << i << " SNAKE_BODYI : RIGHT" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif // DEBUG
		}
		///1
		else if (
			(_part_pos[i].x > _part_pos[i + 1].x && _part_pos[i].y < _part_pos[i - 1].y) &&
			(_part_pos[i].y == _part_pos[i + 1].y && _part_pos[i].x == _part_pos[i - 1].x)
			||
			(_part_pos[i].y < _part_pos[i + 1].y && _part_pos[i].x > _part_pos[i - 1].x) &&
			(_part_pos[i].x == _part_pos[i + 1].x && _part_pos[i].y == _part_pos[i - 1].y)
			) {
			if (isOnEdge(_part_pos[i]) && isOnEdge(_part_pos[i + 1]) && isOnEdge(_part_pos[i - 1])) {
				if (isOnEdge(_part_pos[i]) && isOnEdge(_part_pos[i + 1]) && isOnEdge(_part_pos[i - 1])) {
					if (isOnEdge(_part_pos[i]) == 1 && (isOnEdge(_part_pos[i + 1]) == 1 || isOnEdge(_part_pos[i - 1]) == 1)) {
						_map->rotateTile(_part_pos[i], 270);
					}
					else {
						_map->rotateTile(_part_pos[i], 90);
					}
					if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
					else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
				}

			}
			else if (isInCorner(_part_pos[i]) == 2 && (isInCorner(_part_pos[i + 1]) == 1 && isInCorner(_part_pos[i - 1]) == 3)) {
				_map->rotateTile(_part_pos[i], 0);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 2 && (isInCorner(_part_pos[i + 1]) == 3 && isInCorner(_part_pos[i - 1]) == 1)) {
				_map->rotateTile(_part_pos[i], 0);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 2 && (isInCorner(_part_pos[i + 1]) == 1 || isInCorner(_part_pos[i - 1]) == 1)) {
				_map->rotateTile(_part_pos[i], 90);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 2 && (isInCorner(_part_pos[i + 1]) == 3 || isInCorner(_part_pos[i - 1]) == 3)) {
				_map->rotateTile(_part_pos[i], 270);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else {
				_map->rotateTile(_part_pos[i], 180);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
#ifdef DEBUG
			std::cout << i << " SNAKE_BODYI : 180" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif // DEBUG
		}
		///2
		else if ((_part_pos[i].x < _part_pos[i + 1].x && _part_pos[i].y < _part_pos[i - 1].y) ||
			(_part_pos[i].y < _part_pos[i + 1].y && _part_pos[i].x < _part_pos[i - 1].x)
			) {
			if (isOnEdge(_part_pos[i]) && isOnEdge(_part_pos[i + 1]) && isOnEdge(_part_pos[i - 1])) {
				if (isOnEdge(_part_pos[i]) && isOnEdge(_part_pos[i + 1]) && isOnEdge(_part_pos[i - 1])) {
					if (isOnEdge(_part_pos[i]) == 4 && (isOnEdge(_part_pos[i + 1]) == 4 || isOnEdge(_part_pos[i - 1]) == 4)) {
						_map->rotateTile(_part_pos[i], 180);
					}
					else {
						_map->rotateTile(_part_pos[i], 0);
					}
					if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
					else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
				}
			}
			else if (isInCorner(_part_pos[i]) == 1 && (isInCorner(_part_pos[i + 1]) == 2 && isInCorner(_part_pos[i - 1]) == 4)) {
				_map->rotateTile(_part_pos[i], 270);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 1 && (isInCorner(_part_pos[i + 1]) == 4 && isInCorner(_part_pos[i - 1]) == 2)) {
				_map->rotateTile(_part_pos[i], 270);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 1 && (isInCorner(_part_pos[i + 1]) == 4 || isInCorner(_part_pos[i - 1]) == 4)) {
				_map->rotateTile(_part_pos[i], 0);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 1 && (isInCorner(_part_pos[i + 1]) == 2 || isInCorner(_part_pos[i - 1]) == 2)) {
				_map->rotateTile(_part_pos[i], 180);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else {
				_map->rotateTile(_part_pos[i], 90);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
#ifdef DEBUG
			std::cout << i << " SNAKE_BODYI : 90" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif // DEBUG
		}
		///3
		else if ((_part_pos[i].x > _part_pos[i + 1].x && _part_pos[i].y > _part_pos[i - 1].y) ||
			(_part_pos[i].y > _part_pos[i + 1].y && _part_pos[i].x > _part_pos[i - 1].x)
			) {
			if (isOnEdge(_part_pos[i]) && isOnEdge(_part_pos[i + 1]) && isOnEdge(_part_pos[i - 1])) {
				
				if (isOnEdge(_part_pos[i]) == 3 && (isOnEdge(_part_pos[i + 1]) == 3 || isOnEdge(_part_pos[i - 1]) == 3)) {
					_map->rotateTile(_part_pos[i], 180);
				}
				else {
					_map->rotateTile(_part_pos[i], 0);
				}
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 3 && (isInCorner(_part_pos[i + 1]) == 2 && isInCorner(_part_pos[i - 1]) == 4)) {
				_map->rotateTile(_part_pos[i], 90);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 3 && (isInCorner(_part_pos[i + 1]) == 4 && isInCorner(_part_pos[i - 1]) == 2)) {
				_map->rotateTile(_part_pos[i], 90);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 3 && (isInCorner(_part_pos[i + 1]) == 2 || isInCorner(_part_pos[i - 1]) == 2)) {
				_map->rotateTile(_part_pos[i], 180);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 3 && (isInCorner(_part_pos[i + 1]) == 4 || isInCorner(_part_pos[i - 1]) == 4)) {
				_map->rotateTile(_part_pos[i], 0);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else {
				_map->rotateTile(_part_pos[i], 270);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			
#ifdef DEBUG
			std::cout << i << " SNAKE_BODYI : 270" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif // DEBUG
		}
		///4
		else if ((_part_pos[i].x < _part_pos[i + 1].x && _part_pos[i].y > _part_pos[i - 1].y) ||
			(_part_pos[i].y > _part_pos[i + 1].y && _part_pos[i].x < _part_pos[i - 1].x)
			) {
			if (isOnEdge(_part_pos[i]) && isOnEdge(_part_pos[i + 1]) && isOnEdge(_part_pos[i - 1])) {
				if (isOnEdge(_part_pos[i]) == 3 && (isOnEdge(_part_pos[i + 1]) == 3 || isOnEdge(_part_pos[i - 1]) == 3)) {
					_map->rotateTile(_part_pos[i], 90);
				}
				else {
					_map->rotateTile(_part_pos[i], 270);
				}
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 4 && (isInCorner(_part_pos[i + 1]) == 1 && isInCorner(_part_pos[i - 1]) == 3)) {
				_map->rotateTile(_part_pos[i], 180);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 4 && (isInCorner(_part_pos[i + 1]) == 3 && isInCorner(_part_pos[i - 1]) == 1)) {
				_map->rotateTile(_part_pos[i], 180);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i])==4 && (isInCorner(_part_pos[i + 1])==1 || isInCorner(_part_pos[i - 1])==1)) {
				_map->rotateTile(_part_pos[i], 90);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else if (isInCorner(_part_pos[i]) == 4 && (isInCorner(_part_pos[i + 1]) == 3 || isInCorner(_part_pos[i - 1]) == 3)) {
				_map->rotateTile(_part_pos[i], 270);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
			else {
				_map->rotateTile(_part_pos[i], 0);
				if (_snake_fat_body[i])_map->setTileType(_part_pos[i], Map::SNAKE_BODYLO);
				else _map->setTileType(_part_pos[i], Map::SNAKE_BODYL);
			}
#ifdef DEBUG
			std::cout << i << " SNAKE_BODYI : 0" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif // DEBUG
		}
		else {
#ifdef DEBUG
			std::cout << i << " ERROR" << "X:" << _part_pos[i].x << " Y:" << _part_pos[i].y << std::endl;
#endif 
			//_map->setTileType(_part_pos[i], Map::SNAKE_BODYI);
		}
	}
}

inline void Snake::updateTail(){
	//change the snake tail angle
	_map->setTileType(_part_pos.front(), Map::SNAKE_TAIL);
	if ((isOnEdge(_part_pos.front()) && isOnEdge(_part_pos[1]))) {
		if (isOnEdge(_part_pos.front()) != isOnEdge(_part_pos[1])) {
			if (_part_pos.front().x == _part_pos[1].x) {
				if (_part_pos.front().y > _part_pos[1].y)_map->rotateTile(_part_pos.front(), UP);
				else _map->rotateTile(_part_pos.front(), DOWN);
			}
			else {
				if (_part_pos.front().x > _part_pos[1].x)_map->rotateTile(_part_pos.front(), RIGHT);
				else _map->rotateTile(_part_pos.front(), LEFT);
			}
		}
		else {
			if (_part_pos.front().x == _part_pos[1].x) {
				if (_part_pos.front().y > _part_pos[1].y)_map->rotateTile(_part_pos.front(), DOWN);
				else _map->rotateTile(_part_pos.front(), UP);
			}
			else {
				if (_part_pos.front().x > _part_pos[1].x)_map->rotateTile(_part_pos.front(), LEFT);
				else _map->rotateTile(_part_pos.front(), RIGHT);
			}
		}
	}
	else if (isInCorner(_part_pos.front()) && isInCorner(_part_pos[1])) {
		if (_part_pos.front().x == _part_pos[1].x) {
			if (_part_pos.front().y > _part_pos[1].y)_map->rotateTile(_part_pos.front(), UP);
			else _map->rotateTile(_part_pos.front(), DOWN);
		}
		else {
			if (_part_pos.front().x > _part_pos[1].x)_map->rotateTile(_part_pos.front(), RIGHT);
			else _map->rotateTile(_part_pos.front(), LEFT);
		}
	}

	else if (_part_pos.front().x == _part_pos[1].x) {
		if (_part_pos.front().y > _part_pos[1].y)_map->rotateTile(_part_pos.front(), DOWN);
		else _map->rotateTile(_part_pos.front(), UP);
	}
	else {
		if (_part_pos.front().x > _part_pos[1].x)_map->rotateTile(_part_pos.front(), LEFT);
		else _map->rotateTile(_part_pos.front(), RIGHT);
	}
}
