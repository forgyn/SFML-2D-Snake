#include "Bar.h"

Bar::Bar(float pos_x, float pos_y, float size_x, float size_y, RenderWindow* window){
	_window  = window;
	_position.x = pos_x;
	_position.y = pos_y;
	_size.x = size_x;
	_size.y = size_y;

	_background = new RectangleShape(_size);
	_background->setPosition(_position);
	_background->setFillColor(Color::Yellow);

	_indicator = new RectangleShape(Vector2f(0, _size.y));
	_indicator->setPosition(_position);
	_indicator->setFillColor(Color::Green);

	_frontground = new RectangleShape(_size);
	_frontground->setPosition(_position);
	_frontground->setFillColor(Color::Green);

	_value = to_string(0.f);
}

Bar::Bar(Vector2f pos, Vector2f size, RenderWindow* window){
	_window = window;
	_position = pos;
	_size = size;

	_background = new RectangleShape(_size);
	_background->setPosition(_position);
	_background->setFillColor(Color::Yellow);

	_indicator = new RectangleShape(Vector2f(0, _size.y));
	_indicator->setPosition(_position);
	_indicator->setFillColor(Color::Green);

	_frontground = new RectangleShape(_size);
	_frontground->setPosition(_position);
	_frontground->setFillColor(Color::Transparent);

	_value = to_string(0.f);
}

void Bar::setValue(const string &value)
{
	if(stof(value)<=100 && stof(value) >= 0)
	_value = value;
}

void Bar::setBackgroundTexture(string path){
	_loadedBackgroundTexture = false;
	_backgroundTexture = new Texture;
	_backgroundTexture->loadFromFile(path);
	_background->setTexture(_backgroundTexture);
	_background->setFillColor(Color::White);
}

void Bar::setBackgroundTexture(Texture* texture){
	_loadedBackgroundTexture = true;
	_backgroundTexture = texture;
	_background->setTexture(_backgroundTexture);
	_background->setFillColor(Color::White);
}

void Bar::setBackgroundRectTexture(string path, IntRect rect)
{
	_loadedBackgroundTexture = false;
	_backgroundTexture = new Texture;
	_backgroundTexture->loadFromFile(path);
	_background->setTexture(_backgroundTexture);
	_background->setTextureRect(rect);
	_background->setFillColor(Color::White);
}

void Bar::setBackgroundRectTexture(Texture* texture, IntRect rect)
{
	_loadedBackgroundTexture = true;
	_backgroundTexture = texture;
	_background->setTexture(_backgroundTexture);
	_background->setTextureRect(rect);
	_background->setFillColor(Color::White);
}

void Bar::setFrontTexture(Texture* texture){
	_loadedFrontgroundTexture = true;
	_frontgroundTexture = texture;
	_frontground->setTexture(_frontgroundTexture);
	_frontground->setFillColor(Color::White);
}

void Bar::setFrontTexture(string path)
{
	_loadedFrontgroundTexture = false;
	_frontgroundTexture = new Texture;
	_frontgroundTexture->loadFromFile(path);
	_frontground->setTexture(_frontgroundTexture);
	_frontground->setFillColor(Color::White);
}

void Bar::setFrontRectTexture(Texture* texture, IntRect rect){
	_loadedFrontgroundTexture = true;
	_frontgroundTexture = texture;
	_frontground->setTexture(_frontgroundTexture);
	_frontground->setFillColor(Color::White);
	_frontground->setTextureRect(rect);
}

void Bar::setFrontRectTexture(string path, IntRect rect){
	_loadedFrontgroundTexture = false;
	_frontgroundTexture = new Texture;
	_frontgroundTexture->loadFromFile(path);
	_frontground->setTexture(_frontgroundTexture);
	_frontground->setTextureRect(rect);
	_frontground->setFillColor(Color::White);
}

void Bar::setIndicatorTexture(string path)
{
	_loadedIndicatorTexture = false;
	_indicatorTexture = new Texture;
	_indicatorTexture->loadFromFile(path);
	_indicator->setTexture(_frontgroundTexture);
	_indicator->setFillColor(Color::White);
}

void Bar::setIndicatorTexture(Texture* texture)
{
	_loadedIndicatorTexture = true;
	_indicatorTexture = texture;
	_indicator->setTexture(_frontgroundTexture);
	_indicator->setFillColor(Color::White);
}

void Bar::setIndicatorRectTexture(string path, IntRect rect)
{
	_loadedIndicatorTexture = false;
	_indicatorTexture = new Texture;
	_indicatorTexture->loadFromFile(path);
	_indicator->setTexture(_frontgroundTexture);
	_indicator->setTextureRect(rect);
	_indicator->setFillColor(Color::White);
}

void Bar::setIndicatorRectTexture(Texture* texture, IntRect rect)
{
	_loadedIndicatorTexture = true;
	_indicatorTexture = texture;
	_indicator->setTexture(_frontgroundTexture);
	_indicator->setTextureRect(rect);
	_indicator->setFillColor(Color::White);
}

void Bar::setColor(Color fillColor, Color outlineColor){
	_indicator->setFillColor(fillColor);
	_indicator->setOutlineColor(outlineColor);
}

void Bar::update(Event* _event, Mouse* _mouse){
	_indicator->setSize(Vector2f(_size.x*(stof(_value)/100),_size.y));
}

void Bar::draw()
{
	update(nullptr,nullptr);
	_window->draw(*_background);
	_window->draw(*_indicator);
	_window->draw(*_frontground);
}

Bar::~Bar()
{
	delete _background;
	delete _frontground;
	delete _indicator;
	if (!_loadedBackgroundTexture)delete _backgroundTexture;
	if (!_loadedFrontgroundTexture)delete _frontgroundTexture;
}
