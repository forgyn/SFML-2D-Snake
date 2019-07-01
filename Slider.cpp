#include "Slider.h"

Slider::Slider(float size_x, float size_y, float pos_x, float pos_y, RenderWindow* window){
	_window = window;
	_size.x = size_x;
	_size.y = size_y;
	_position.x = pos_x;
	_position.y = pos_y;
	_backgroud = new RectangleShape(_size);
	_backgroud->setPosition(_position);
	_backgroud->setFillColor(Color::Yellow);
	//_button = new DraggableButton(_size.x / _ration, _size.y, _position.x, _position.y);
	if (_size.x > _size.y) { 
		_mod = 1;
		_button = new DraggableButton(_size.x / _ration, _size.y, _position.x, _position.y,_window);
		_button->setLimit(_position.x, _position.x + _size.x, 0, 0);
		
	}
	else if (_size.x < _size.y) {
		_mod = 2;
		_button = new DraggableButton(_size.x, _size.y / _ration, _position.x, _position.y, _window);
		_button->setLimit(0, 0, _position.y, _position.y + _size.y);
		
	}
	else { 
		_mod = 0;
		_button = new DraggableButton(_size.x / _ration, _size.y / _ration, _position.x, _position.y, _window);
		_button->setLimit(_position.x, _position.x + _size.x, _position.y, _position.y + _size.y);
		
	}
	_button->changeMod(_mod);
	_button->setColor(Color::Green);
}

Slider::Slider(Vector2f size, Vector2f pos, RenderWindow* window){
	_window = window;
	_size = size;
	_position = pos;
	_backgroud = new RectangleShape(_size);
	_backgroud->setPosition(_position);
	_backgroud->setFillColor(Color::White);
	_button = new DraggableButton(_size.x / _ration, _size.y, _position.x, _position.y, _window);
	if (_size.x > _size.y) {
		_button->changeMod(1);
		_button->setLimit(_position.x, _position.x + _size.x, 0, 0);
	}
	else if (_size.x < _size.y) {
		_button->changeMod(2);
		_button->setLimit(0, 0, _position.y, _position.y + _size.y);
	}
	else {
		_button->changeMod(0);
		_button->setLimit(_position.x, _position.x + _size.x, _position.y, _position.y + _size.y);
	}
	_button->setColor(Color::Green);
}

Slider::~Slider()
{
	delete _button;
	delete _backgroud;
	if (!_loadedTexture)delete _texture;
}

void Slider::update(Event* _event, Mouse* _mouse){
	updateRatio(_event);
	_button->update(_event,_mouse);
	if (_event->type == Event::MouseButtonReleased)release();
	if (_button->FOLLOWING) {
		if (_mouse->getPosition(*_window).y / _ratio.y > _position.y + _size.y) release();
		if (_mouse->getPosition(*_window).y / _ratio.y < _position.y) release();
		if (_mouse->getPosition(*_window).x / _ratio.x > _position.x + _size.x) release();
		if (_mouse->getPosition(*_window).x / _ratio.x < _position.x) release();
	}
	_sliderPos.x = ((_button->getPos().x - _position.x) / (_size.x - _size.x / _ration)) * 100;
	_sliderPos.y = ((_button->getPos().y - _position.y) / (_size.y - _size.y / _ration)) * 100;
}

void Slider::setBackgroundTexture(Texture* texture)
{
	_texture = texture;
	_backgroud->setTexture(texture);
	_loadedTexture = true;
}

void Slider::setBackgroundTexture(string path){
	_texture = new Texture;
	_texture->loadFromFile(path);
	_backgroud->setTexture(_texture);
	_loadedTexture = false;
}

void Slider::setBackgroundRectTexture(Texture* texture, IntRect rec)
{
	_texture = texture;
	_backgroud->setTexture(texture);
	_backgroud->setTextureRect(rec);
	_loadedTexture = true;
}

void Slider::setBackgroundRectTexture(string path, IntRect rec){
	_texture = new Texture;
	_texture->loadFromFile(path);
	_backgroud->setTexture(_texture);
	_backgroud->setTextureRect(rec);
	_loadedTexture = false;
}

void Slider::setSpecialTexture(Texture* texture)
{
	_button->setBackgroundTexture(texture);
	_button->setColor(Color::White);
}

void Slider::setSpecialTexture(string path){
	_button->setBackgroundTexture(path);
	_button->setColor(Color::White);
}

void Slider::setSpecialRectTexture(Texture* texture, IntRect rec)
{
	_button->setBackgroundRectTexture(texture,rec);
	_button->setColor(Color::White);
}

void Slider::setSpecialRectTexture(string path, IntRect rect){
	_button->setBackgroundRectTexture(path,rect);
	_button->setColor(Color::White);
}


void Slider::changeRatio(float ration)
{
	if (ration > 0 && ration < 100) { 
		_ration = ration; 
		switch (_mod) {
		case 0:
			_button->changeSize(Vector2f(_size.x / _ration, _size.y / _ration));
			_button->setLimit(_position.x, _position.x + _size.x, _position.y, _position.y + _size.y);
			break;
		case 1:
			_button->changeSize(Vector2f(_size.x / _ration, _size.y));
			_button->setLimit(_position.x, _position.x + _size.x, 0, 0);
			break;
		case 2:
			_button->changeSize(Vector2f(_size.x, _size.y / _ration));
			_button->setLimit(0, 0, _position.y, _position.y + _size.y);
			break;
		}
		
	}
}

void Slider::draw(){
	_window->draw(*_backgroud);
	_button->draw();
}

void Slider::release()
{
	_button->reset();
}

void Slider::reset(){
	release();
	_sliderPos = Vector2f(0,0);
	_button->changePos(Vector2f(_position.x, _position.y));
}

void Slider::setInput(Vector2f value){
	if(value.x>=0 && value.x<=100 && value.y >= 0 && value.y <= 100){
	reset();
	_sliderPos = value;
	_button->changePos(Vector2f(_position.x + ((_size.x - _size.x / _ration) * (value.x / 100)), _position.y + ((_size.y - _size.y / _ration) * (value.y / 100))));
	}
}

