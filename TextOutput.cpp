#include "TextOutput.h"

TextOutput::TextOutput(float pos_x, float pos_y, float size, Font* font, RenderWindow* window){
	_window = window;
	_text = new Text();
	_text->setString("");
	_text->setCharacterSize(size);
	_position.x = pos_x;
	_position.y = pos_y;
	_font = font;
	loadedFont = true;
	_text->setFont(*_font);
	_text->setPosition(_position);
	_text->setFillColor(Color::Black);
}

TextOutput::TextOutput(Vector2f pos, float size, Font* font, RenderWindow* window){
	_window = window;
	_text = new Text();
	_text->setString("");
	_text->setCharacterSize(size);
	_position = pos;
	_font = font;
	loadedFont = true;
	_text->setFont(*_font);
	_text->setPosition(_position);
	_text->setFillColor(Color::Black);
}

TextOutput::~TextOutput(){
	delete _text;
	if (!loadedFont)delete _font;
}

void TextOutput::draw(){
	update(nullptr,nullptr);
	_window->draw(*_text);
}

void TextOutput::setValue(const string& value)
{
	_value = value;
}

void TextOutput::setColor(Color fillColor, Color outlineColor)
{
	_text->setFillColor(fillColor);
	_text->setOutlineColor(outlineColor);
}

Vector2f TextOutput::getSize()
{
	return Vector2f(_text->getLocalBounds().width, _text->getLocalBounds().height);
}

void TextOutput::changePos(const float& x, const float& y)
{
	_position.x = x;
	_position.y = y;
}

void TextOutput::update(Event* _event, Mouse* _mouse)
{
	_text->setString(_value);
	_text->setPosition(_position);
}

void TextOutput::setFont(Font* font){
	if (_font != nullptr && !loadedFont)delete _font;
	_font = font;
	_text->setFont(*_font);
	loadedFont = true;
}

void TextOutput::setFont(string path){
	if (_font != nullptr && !loadedFont)delete _font;
	_font = new Font();
	_font->loadFromFile(path);
	_text->setFont(*_font);
	loadedFont = false;
}