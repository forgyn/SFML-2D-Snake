#include "Button.h"

Button::Button(float size_x, float size_y, float pos_x, float pos_y,RenderWindow* window){
	_window = window;
	_size.x = size_x;
	_size.y = size_y;
	_position.x = pos_x;
	_position.y = pos_y;
	_backgroud = new RectangleShape(_size);
	_backgroud->setPosition(_position);
	_basicColor = BASIC_COLOR;
	_backgroud->setFillColor(_basicColor);
}

Button::Button(Vector2f size,Vector2f pos, RenderWindow* window){
	_window = window;
	_size = size;
	_position = pos;
	_backgroud = new RectangleShape(_size);
	_backgroud->setPosition(_position);
	_basicColor = BASIC_COLOR;
	_backgroud->setFillColor(_basicColor);
}

Button::~Button() {
	delete _backgroud;
	if (!loadedTexture)delete _texture;
}

void Button::setBackgroundTexture(string path) {
	_texture = new Texture();
	_texture->loadFromFile(path);
	_backgroud->setTexture(_texture);
	_basicColor = Color::White;
}

void Button::setBackgroundTexture(Texture* texture) {
	_texture = texture;
	_backgroud->setTexture(_texture);
	loadedTexture = true;
	_basicColor = Color::White;
}

void Button::setBackgroundRectTexture(string path, IntRect rect) {
	_texture = new Texture();
	_texture->loadFromFile(path);
	_backgroud->setTexture(_texture);
	_backgroud->setTextureRect(rect);
	_basicColor = Color::White;
}

void Button::setBackgroundRectTexture(Texture* texture, IntRect rect) {
	_texture = texture;
	_backgroud->setTexture(_texture);
	loadedTexture = true;
	_backgroud->setTextureRect(rect);
	_basicColor = Color::White;
}

bool Button::isPressed(bool unpress){
	if (PRESSED) {
		if (unpress)PRESSED = false;
		return true;
	}
	return false;
}

void Button::draw()
{
	_window->draw(*_backgroud);
}

void Button::update(Event* _event,Mouse* _mouse) {
	updateRatio(_event);

	_backgroud->setPosition(_position);
	_backgroud->setSize(_size);
	if (isPointed(_mouse) || PRESSED) {
		if (_event->type != Event::MouseButtonPressed) {
			if(_backgroud->getFillColor() != Color::Transparent)
			_backgroud->setFillColor(Color(
				_basicColor.r * 0.5f,
				_basicColor.g * 0.5f,
				_basicColor.b * 0.5f));
		}
		else {
			if (!PRESSED) {
				PRESSED = true;
			}
			else {
				PRESSED = false;
			}
			_backgroud->setFillColor(_basicColor);
		}
	}
	else {
		_backgroud->setFillColor(_basicColor);
	}
}

bool Button::isPointed(Mouse* _mouse) {
	
	//check x
	if (_mouse->getPosition(*_window).x >= (_position.x-_backgroud->getOrigin().x)*_ratio.x
		&& _mouse->getPosition(*_window).x <= ((_position.x - _backgroud->getOrigin().x) + _size.x) * _ratio.x)
		//check y
		if (_mouse->getPosition(*_window).y >= (_position.y - _backgroud->getOrigin().y) * _ratio.y
			&& _mouse->getPosition(*_window).y <= ((_position.y - _backgroud->getOrigin().y) + _size.y)* _ratio.y)
			return true;

	return false;
}

void Button::setColor(const Color& color){
	_basicColor = color;
}

void Button::reset()
{
	PRESSED = false;
}

void Button::changePos(Vector2f pos) {
	_position.x = pos.x - _backgroud->getOrigin().x;
	_position.y = pos.y - _backgroud->getOrigin().y;
	_backgroud->setPosition(_position);
}

void Button::changeSize(Vector2f size){
	_size = size;
	_backgroud->setSize(_size);
}

DraggableButton::DraggableButton(float size_x, float size_y, float pos_x, float pos_y, RenderWindow* window)
	:Button(size_x, size_y, pos_x, pos_y,window)
{
	_window = window;
	_limit_max = Vector2f(_window->getSize().x, _window->getSize().y);
	_limit_min = Vector2f(0, 0);
}

DraggableButton::DraggableButton(Vector2f size, Vector2f pos, RenderWindow* window)
	:Button(size,pos,window)
{
	_window = window;
	_limit_max = Vector2f(_window->getSize().x, _window->getSize().y);
	_limit_min = Vector2f(0, 0);
}

void DraggableButton::update(Event* _event, Mouse* _mouse){

	updateRatio(_event);
	
	//cout << _mouse->getPosition(*_window).x << " " << _mouse->getPosition(*_window).y << endl;
	//cout << _mouse->getPosition(*_window).x / _ratio.x << " " << _mouse->getPosition(*_window).y / _ratio.y << endl;
	if (_event->type == Event::MouseButtonReleased && FOLLOWING)reset();
	if (FOLLOWING) {
		if (_mod == 0) {
			if((_mouse->getPosition(*_window).x / _ratio.x - _backgroud->getOrigin().x + _size.x) <= _limit_max.x
				&& _mouse->getPosition(*_window).x / _ratio.x - _backgroud->getOrigin().x >= _limit_min.x)
			_position.x = (_mouse->getPosition(*_window).x / _ratio.x);

			if (_mouse->getPosition(*_window).y / _ratio.y - _backgroud->getOrigin().y + _size.y <= _limit_max.y
				&& _mouse->getPosition(*_window).y / _ratio.y - _backgroud->getOrigin().y >= _limit_min.y)
			_position.y = (_mouse->getPosition(*_window).y / _ratio.y);
		}
		else if (_mod == 1) {
			
			if (_mouse->getPosition(*_window).x / _ratio.x - _backgroud->getOrigin().x + _size.x <= _limit_max.x
				&& _mouse->getPosition(*_window).x / _ratio.x - _backgroud->getOrigin().x >= _limit_min.x)
			_position.x = (_mouse->getPosition(*_window).x / _ratio.x);
		}
		else if (_mod == 2) {
			if (_mouse->getPosition(*_window).y / _ratio.y - _backgroud->getOrigin().y + _size.y <= _limit_max.y
				&& _mouse->getPosition(*_window).y / _ratio.y - _backgroud->getOrigin().y >= _limit_min.y)
			_position.y = (_mouse->getPosition(*_window).y / _ratio.y);
			
		}
		_backgroud->setPosition(_position);
	}

	if (isPointed(_mouse)) {
		//set follow
		if (_event->type == Event::MouseButtonPressed && !FOLLOWING) {
			_backgroud->setOrigin(
			Vector2f(
			_size.x - ((_position.x + _size.x) - (_mouse->getPosition(*_window).x / _ratio.x))
			,_size.y - ((_position.y + _size.y) - (_mouse->getPosition(*_window).y / _ratio.y))));
			_position.x = (_mouse->getPosition(*_window).x / _ratio.x);
			_position.y = (_mouse->getPosition(*_window).y / _ratio.y);
			FOLLOWING = true;
			PRESSED = true;
		}
		//change color if pointed
		_backgroud->setFillColor(Color(
			_basicColor.r * 0.5f,
			_basicColor.g * 0.5f,
			_basicColor.b * 0.5f));
	}
	else {
		_backgroud->setFillColor(_basicColor);
	}
	_backgroud->setPosition(_position);
}

void DraggableButton::reset() {
	_position.x = _position.x - _backgroud->getOrigin().x;
	_position.y = _position.y - _backgroud->getOrigin().y;
	_backgroud->setOrigin(Vector2f(0, 0));
	_backgroud->setPosition(_position);
	FOLLOWING = false;
}

void DraggableButton::setLimit(float min_x, float max_x, float min_y, float max_y)
{
	_limit_max.x = max_x;
	_limit_max.y = max_y;
	_limit_min.x = min_x;
	_limit_min.y = min_y;
}

TextButton::TextButton(float size_x, float size_y, float pos_x, float pos_y,  const string& text, RenderWindow* window)
	:Button(size_x, size_y,pos_x,pos_y,window){
	_window = window;
	_text = new Text;
	_text->setString(text);
	_text_ratio = TEXT_RATIO;
	_text->setCharacterSize(_size.y * _text_ratio);
	_text->setOrigin(_text->getLocalBounds().left, _text->getLocalBounds().top);
	_text->setPosition(_position.x + ((_size.x - _text->getLocalBounds().width) / 2), _position.y + ((_size.y - _text->getLocalBounds().height) / 2));
	_basic_text_color = Color::Black;
	_text->setFillColor(_basic_text_color);
}

TextButton::TextButton(float size_x, float size_y, float pos_x, float pos_y, const string& text, RenderWindow* window, Font* font)
:Button(size_x, size_y, pos_x, pos_y, window){
	_window = window;
	_text = new Text;
	_text->setString(text);
	_font = font;
	_text->setFont(*_font);
	_text_ratio = TEXT_RATIO;
	_text->setCharacterSize(_size.y * _text_ratio);
	_text->setOrigin(_text->getLocalBounds().left, _text->getLocalBounds().top);
	_text->setPosition(_position.x + ((_size.x - _text->getLocalBounds().width) / 2), _position.y + ((_size.y - _text->getLocalBounds().height) / 2));
	_basic_text_color = Color::Black;
	_text->setFillColor(_basic_text_color);
	loadedFont = true;
}

TextButton::~TextButton()
{
	delete _text;
	if (!loadedFont)delete _font;
}

void TextButton::draw(){
	
	_window->draw(*_backgroud);
	_window->draw(*_text);
}

void TextButton::update(Event* _event, Mouse* _mouse){
	updateRatio(_event);

	_backgroud->setPosition(_position);
	_backgroud->setSize(_size);
	if (isPointed(_mouse)) {
		if (_backgroud->getFillColor() != Color::Transparent)
			_backgroud->setFillColor(Color(
				_basicColor.r * 0.5f,
				_basicColor.g * 0.5f,
				_basicColor.b * 0.5f));
		if (_text->getFillColor() != Color::Transparent)
			_text->setFillColor(Color(
				_basic_text_color.r * 0.5f,
				_basic_text_color.g * 0.5f,
				_basic_text_color.b * 0.5f
			));
		if (_event->type == Event::MouseButtonPressed) {
			_backgroud->setFillColor(_basicColor);
			_text->setFillColor(_basic_text_color);
			if (!PRESSED) {
				PRESSED = true;
			}
			else {
				PRESSED = false;
			}
		}
	}
	else if (!PRESSED) {
		_backgroud->setFillColor(_basicColor);
		_text->setFillColor(_basic_text_color);
	}
	if (PRESSED) {
		if (_backgroud->getFillColor() != Color::Transparent)
			_backgroud->setFillColor(Color(
				_basicColor.r * 0.5f,
				_basicColor.g * 0.5f,
				_basicColor.b * 0.5f));
		if (_text->getFillColor() != Color::Transparent)
			_text->setFillColor(Color(
				_basic_text_color.r * 0.5f,
				_basic_text_color.g * 0.5f,
				_basic_text_color.b * 0.5f
			));
	}

}

void TextButton::changeStyle(Uint32 style){
	_text->setStyle(style);
	_text->setOrigin(_text->getLocalBounds().left, _text->getLocalBounds().top);
	_text->setPosition(_position.x + ((_size.x - _text->getLocalBounds().width) / 2), _position.y + ((_size.y - _text->getLocalBounds().height) / 2));
}

void TextButton::setFont(Font* font)
{
	_font = font; 
	_text->setFont(*_font);
	loadedFont = true;
}

void TextButton::setFont(string path)
{
	_font = new Font();
	_font->loadFromFile(path);
	_text->setFont(*_font);
	loadedFont = false;
}

void TextButton::setColor(Color bck_col, Color txt_col)
{
	_basicColor = bck_col;
	_basic_text_color = txt_col;
	_backgroud->setFillColor(_basicColor);
	_text->setFillColor(_basic_text_color);
}

void TextButton::setTextRatio(float ratio) {
	_text_ratio = ratio;
	_text->setCharacterSize(_size.y * _text_ratio * (_text->getCharacterSize()/_text->getLocalBounds().height));
	_text->setOrigin(_text->getLocalBounds().left, _text->getLocalBounds().top);
	_text->setPosition(_position.x + ((_size.x - _text->getLocalBounds().width) / 2), _position.y + ((_size.y - _text->getLocalBounds().height) / 2));
}

void TextButton::getDebugInfo(){
	cout << "Text origin            : " << _text->getOrigin().x << " " << _text->getOrigin().y << endl;
	cout << "Text size              : " << _text->getLocalBounds().width << " " << _text->getLocalBounds().height << endl;
	cout << "Background size        : " << _backgroud->getSize().x << " " << _backgroud->getSize().y << endl;
	cout << "Text position          : " << _text->getPosition().x << " " << _text->getPosition().y << endl;
	cout << "Background position    : " << _backgroud->getPosition().x << " " << _backgroud->getPosition().y << endl;
	cout << "Offset                 : " << _position.x + ((_size.x - _text->getLocalBounds().width) / 2) << " " << _position.y + ((_size.y - _text->getLocalBounds().height) / 2) << endl;
}
