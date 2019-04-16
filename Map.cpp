#include "Map.h"
#include "Snake.h"
randomU Map::Tile::randomTreat = randomU(0, NUM_OF_TREATS-1);
randomU Map::Tile::randomAngle = randomU(0, 360);
Texture Map::Tile::_texture = Texture();
unsigned short Map::Tile::_animation = 1;
std::default_random_engine Map::randomGenerator = std::default_random_engine(time(0));

Map::Map(const size_t &num,const Vector2u &window_size,Color col) 
	: _size(num){
	_map = new Tile**[_size];
	Vector2f pos;
	Vector2f size;
	size.x = (float)window_size.x / _size;
	size.y = (float)window_size.y / _size;
	pos.x = MAP_OFFSET_X;
	pos.y = MAP_OFFSET_Y;
	for (size_t y = 0; y < _size; y++) {
		_map[y] = new Tile*[_size];
		for (size_t x = 0; x < _size; x++){
			_map[y][x] = new Tile(size, pos);
			pos.x += (float)window_size.x / _size;
			if (pos.x >= window_size.x) {
				pos.y += (float)window_size.y / _size;
				pos.x = MAP_OFFSET_X;
			}
		}
	}
	_background.setSize(Vector2f(window_size.x,window_size.y));
	_background.setFillColor(col);
	_background.setOutlineColor(Color::Black);
	_background.setOutlineThickness(2);
}


Map::~Map(){
	for (size_t y = 0; y < _size; y++){
		for (size_t x = 0; x < _size; x++){
			delete _map[y][x];
		}
		delete[] _map[y];
	}
	delete[] _map;
}


void Map::setTileType(const Vector2u &coor, const TILE_TYPE &type){
	_map[coor.y][coor.x]->setType(type);
}

void Map::updateTile(const Vector2u & pos, const unsigned short & type){
#ifdef DEBUG
	showBitmap();
#endif
	_map[pos.y][pos.x]->updateSnakeParts(type);
}

bool Map::checkTile(const Vector2u & pos, const TILE_TYPE &type){
	if (_map[pos.y][pos.x]->getType() == type)return true;
	else return false;
}

void Map::rotateTile(const Vector2u & pos, const float & angle){
	_map[pos.y][pos.x]->rotate(angle);
}

void Map::resetTile(const Vector2u &pos){
	_map[pos.y][pos.x]->reset();
}

void Map::updateNoSnake(){
	LOOPY(_size) {
		LOOPX(_size) {
			if (_map[y][x]->getType() != ERROR) _map[y][x]->updateNoSnake();
		}
	}
}


void Map::spawnTreat()
{
	std::uniform_int_distribution<unsigned> randomPos(0, _size-1);
	Vector2u spawnPos;
	while (true) {
		spawnPos.x = randomPos(randomGenerator);
		spawnPos.y = randomPos(randomGenerator);
		if (checkTile(spawnPos, AIR)) {
			setTileType(spawnPos, TREAT);
			_map[spawnPos.y][spawnPos.x]->setRandomTreat();
			_map[spawnPos.y][spawnPos.x]->setRandomAngle();
			break;
		}
	}
}



void Map::clear(){
	LOOPY(_size) {
		LOOPX(_size) {
			if(_map[x][y]->getType()!=TREAT)_map[x][y]->setType(AIR);
		}
	}
}


void Map::update(){

	LOOPY(_size) {
		LOOPX(_size) {
			if (_map[y][x]->getType() != ERROR) _map[y][x]->update();
		}
	}
}

void Map::draw(RenderWindow & window){
	window.draw(_background);
	LOOPY(_size) {
		LOOPX(_size) {
			if(_map[y][x]->getType() != ERROR) _map[y][x]->draw(window);
		}
	}
}

Map::Tile::Tile(const Vector2f & size, const Vector2f & pos) :_size(size), _pos(pos) {
	this->_body.setSize(_size);
	this->_body.setTexture(&_texture);
	this->_type = AIR;
	this->_body.setOrigin(size.x / 2, size.y / 2);
	this->_body.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);
	s_c = _texture.getSize().x / 10;
	_body.setFillColor(Color::White);
#ifdef DEBUG
	_body.setOutlineThickness(-2);
	this->_body.setOutlineColor(Color::Black);
#endif
}

void Map::Tile::updateSnakeParts(const unsigned short & type){
	_body.setFillColor(Color::White);
	switch (_type) {
	case SNAKE_BODYI:
		_body.setTextureRect(IntRect(1 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_BODYL:
		_body.setTextureRect(IntRect(3 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_HEAD:
		_body.setTextureRect(IntRect(2 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_TAIL:
		_body.setTextureRect(IntRect(0, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_BODYO:
		_body.setTextureRect(IntRect(4 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_BODYLO:
		_body.setTextureRect(IntRect(5 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	}
}

void Map::Tile::updateNoSnake() {
	_body.setFillColor(Color::White);
	switch (_type) {
	case AIR:
		_body.setFillColor(Color::Transparent);
		_body.setTextureRect(IntRect(0, 0, s_c, s_c));
		break;
	}
}

void Map::Tile::update() {
	_body.setFillColor(Color::White);
	unsigned type = 0;
	switch (_type) {
	case SNAKE_BODYI:
		_body.setTextureRect(IntRect(1 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_BODYL:
		_body.setTextureRect(IntRect(3 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_HEAD:
		_body.setTextureRect(IntRect(2 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_TAIL:
		_body.setTextureRect(IntRect(0, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_BODYO:
		_body.setTextureRect(IntRect(4 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case SNAKE_BODYLO:
		_body.setTextureRect(IntRect(5 * s_c, 2 * s_c + s_c * type, s_c, s_c));
		break;
	case AIR:
		_body.setFillColor(Color::Transparent);
		_body.setTextureRect(IntRect(0, 0, s_c, s_c));
		break;
	case TREAT:
		_body.setTextureRect(IntRect(s_c*_treatID, s_c, s_c, s_c));
		break;
	}
}

void Map::Tile::setRandomTreat() { 
	_treatID = randomTreat(randomGenerator);
	_body.setFillColor(Color::White);
	_body.setTextureRect(IntRect(s_c*_treatID, s_c, s_c, s_c));
}

void Map::Tile::reset(){
	_type = AIR;
	_body.setFillColor(Color::Transparent);
	_body.setTextureRect(IntRect(0, 0, s_c, s_c));
	_body.setRotation(0);
}

