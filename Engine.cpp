#include "Engine.h"
//button main class
bool Button::isPointed(sf::Vector2i poloha) {
	if (
		(poloha.x > poloha_x_y.x && poloha.x < poloha_x_y.x + velikost_x_y.x) && (poloha.y > poloha_x_y.y && poloha.y < poloha_x_y.y + velikost_x_y.y)) {
		return true;
	}
	else return false;
}

bool Button::isPointed(sf::Vector2i poloha, const sf::RenderWindow &window, const float init_width, const float init_height) {
	if ((poloha.x > poloha_x_y.x*(window.getSize().x / init_width) && poloha.x < (poloha_x_y.x + velikost_x_y.x)*(window.getSize().x / init_width))
		&&
		(poloha.y > poloha_x_y.y*(window.getSize().y / init_height) && poloha.y < (poloha_x_y.y + velikost_x_y.y)*(window.getSize().y / init_height))
		)return true;
	else return false;
}

void Button::changeTextColor(sf::Color fill_color, sf::Color outline_color) {
	default_text_color = fill_color;
	default_outline_text_color = outline_color;
	text_tlacitka.setFillColor(fill_color);
	text_tlacitka.setOutlineColor(outline_color);
}

void Button::changeTextThickness(float thicc) {
	text_tlacitka.setOutlineThickness(thicc);
}

void Button::playSound(const unsigned short &v){
	if (sound->getStatus() != sf::Sound::Playing) { 
		sound->setVolume(v);
		sound->play(); }
}

void Button::addSound(const sf::String &audio_path){
	buffer = new sf::SoundBuffer;
	if (sound == nullptr)sound = new sf::Sound(*buffer);
	else {
		delete sound; sound = nullptr;
		sound = new sf::Sound(*buffer);
	}
	received_sound = false;
}

void Button::setSound(sf::SoundBuffer *copy_buffer){
	buffer = copy_buffer;
	if(sound==nullptr)sound = new sf::Sound(*buffer);
	else {
		delete sound; sound = nullptr;
		sound = new sf::Sound(*buffer);
	}
	received_sound = true;
}

//simplebutton class
SimpleButton::SimpleButton(sf::Vector2f poloha, sf::Vector2u velikost, sf::Color color, sf::Color color2, float t) {
	poloha_x_y = poloha;
	velikost_x_y = velikost;
	pozadi = sf::RectangleShape(sf::Vector2f((float)velikost_x_y.x, (float)velikost_x_y.y));
	pozadi.setPosition(poloha_x_y);

	default_color = color;
	pozadi.setFillColor(default_color);
	pozadi.setOutlineColor(default_color_2);

	thicc = t;
	pozadi.setOutlineThickness(thicc);
}

SimpleButton::SimpleButton(float poloha_x, float poloha_y, unsigned velikost_x, unsigned velikost_y, sf::Color color, sf::Color color2, float t) {
	poloha_x_y.x = poloha_x; poloha_x_y.y = poloha_y;
	velikost_x_y.x = velikost_x; velikost_x_y.y = velikost_y;
	pozadi = sf::RectangleShape(sf::Vector2f((float)velikost_x_y.x, (float)velikost_x_y.y));
	pozadi.setPosition(poloha_x_y);

	default_color = color;
	default_color_2 = color2;
	pozadi.setFillColor(default_color);
	pozadi.setOutlineColor(default_color_2);

	thicc = t;
	pozadi.setOutlineThickness(thicc);

}


SimpleButton::SimpleButton(const SimpleButton & button) {
	velikost_x_y = button.velikost_x_y;
	poloha_x_y = button.poloha_x_y;
	default_color = button.default_color;
	default_color_2 = button.default_color_2;
	thicc = button.thicc;
	pozadi = button.pozadi;

	font = button.font;
	text_tlacitka = button.text_tlacitka;
	text_tlacitka.setFont(font);
	haveText = button.haveText;
}

void SimpleButton::drawButton(sf::RenderWindow &window) {
	window.draw(pozadi);
	if (haveText)window.draw(text_tlacitka);
}
void SimpleButton::drawButton(const sf::String &str,sf::RenderWindow &window) {
	window.draw(pozadi);
	text_tlacitka.setString(str);
	window.draw(text_tlacitka);
}
bool SimpleButton::isPointed(sf::Vector2i poloha, sf::Vector3f color) {
	if (
		(poloha.x > poloha_x_y.x && poloha.x < poloha_x_y.x + velikost_x_y.x)
		&&
		(poloha.y > poloha_x_y.y && poloha.y < poloha_x_y.y + velikost_x_y.y)
		)
	{
		//zmìna barvy pozadi
		sf::Color current_color = pozadi.getFillColor();
		if (default_color.r > 0) if (current_color.r / default_color.r > color.x) current_color.r *= color.x;
		if (default_color.g > 0) if (current_color.g / default_color.g > color.y) current_color.g *= color.y;
		if (default_color.b > 0) if (current_color.b / default_color.b > color.z) current_color.b *= color.z;
		pozadi.setFillColor(current_color);
		sf::Color current_color_2 = pozadi.getOutlineColor();
		if (default_color_2.r > 0) if (current_color_2.r / default_color_2.r > color.x) current_color_2.r *= color.x;
		if (default_color_2.g > 0) if (current_color_2.g / default_color_2.g > color.y) current_color_2.g *= color.y;
		if (default_color_2.b > 0) if (current_color_2.b / default_color_2.b > color.z) current_color_2.b *= color.z;
		pozadi.setOutlineColor(current_color_2);
		//zmìna barvy textu
		sf::Color current_text_color = text_tlacitka.getFillColor();
		if (default_text_color.r > 0) if (text_tlacitka.getFillColor().r / default_text_color.r > color.x) current_text_color.r *= color.x;
		if (default_text_color.g > 0) if (text_tlacitka.getFillColor().g / default_text_color.g > color.y) current_text_color.g *= color.y;
		if (default_text_color.b > 0) if (text_tlacitka.getFillColor().b / default_text_color.b > color.z) current_text_color.b *= color.z;
		text_tlacitka.setFillColor(current_text_color);
		return true;
	}
	else {
		pozadi.setFillColor(default_color);
		pozadi.setOutlineColor(default_color_2);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}

bool SimpleButton::isPointed(sf::Vector2i poloha, float r, float g, float b) {
	if (
		(poloha.x > poloha_x_y.x && poloha.x < poloha_x_y.x + velikost_x_y.x)
		&&
		(poloha.y > poloha_x_y.y && poloha.y < poloha_x_y.y + velikost_x_y.y)
		)
	{
		//zmìna barvy pozadi
		sf::Color current_color = pozadi.getFillColor();
		if (default_color.r > 0) if (current_color.r / default_color.r > r) current_color.r *= r;
		if (default_color.g > 0) if (current_color.g / default_color.g > g) current_color.g *= g;
		if (default_color.b > 0) if (current_color.b / default_color.b > b) current_color.b *= b;
		pozadi.setFillColor(current_color);
		sf::Color current_color_2 = pozadi.getOutlineColor();
		if (default_color_2.r > 0) if (current_color_2.r / default_color_2.r > r) current_color_2.r *= r;
		if (default_color_2.g > 0) if (current_color_2.g / default_color_2.g > g) current_color_2.g *= g;
		if (default_color_2.b > 0) if (current_color_2.b / default_color_2.b > b) current_color_2.b *= b;
		pozadi.setOutlineColor(current_color_2);
		//zmìna barvy textu
		sf::Color current_text_color = text_tlacitka.getFillColor();
		if (default_text_color.r > 0) if (text_tlacitka.getFillColor().r / default_text_color.r > r) current_text_color.r *= r;
		if (default_text_color.g > 0) if (text_tlacitka.getFillColor().g / default_text_color.g > g) current_text_color.g *= g;
		if (default_text_color.b > 0) if (text_tlacitka.getFillColor().b / default_text_color.b > b) current_text_color.b *= b;
		text_tlacitka.setFillColor(current_text_color);
		sf::Color current_outline_text_color = text_tlacitka.getOutlineColor();
		if (default_outline_text_color.r > 0) if (text_tlacitka.getOutlineColor().r / default_outline_text_color.r > r) current_outline_text_color.r *= r;
		if (default_outline_text_color.g > 0) if (text_tlacitka.getOutlineColor().g / default_outline_text_color.g > g) current_outline_text_color.g *= g;
		if (default_outline_text_color.b > 0) if (text_tlacitka.getOutlineColor().b / default_outline_text_color.b > b) current_outline_text_color.b *= b;
		text_tlacitka.setOutlineColor(current_outline_text_color);
		return true;
	}
	else {
		pozadi.setFillColor(default_color);
		pozadi.setOutlineColor(default_color_2);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}
bool SimpleButton::isPointed(sf::Vector2i poloha, float r, float g, float b, const sf::RenderWindow &window, const float init_width, const float init_height) {
	if (
		(poloha.x > poloha_x_y.x*(window.getSize().x / init_width) && poloha.x < (poloha_x_y.x + velikost_x_y.x)*(window.getSize().x / init_width))
		&&
		(poloha.y > poloha_x_y.y*(window.getSize().y / init_height) && poloha.y < (poloha_x_y.y + velikost_x_y.y)*(window.getSize().y / init_height))
		)
	{
		//zmìna barvy pozadi
		sf::Color current_color = pozadi.getFillColor();
		if (default_color.r > 0) if (current_color.r / default_color.r > r) current_color.r *= r;
		if (default_color.g > 0) if (current_color.g / default_color.g > g) current_color.g *= g;
		if (default_color.b > 0) if (current_color.b / default_color.b > b) current_color.b *= b;
		pozadi.setFillColor(current_color);
		sf::Color current_color_2 = pozadi.getOutlineColor();
		if (default_color_2.r > 0) if (current_color_2.r / default_color_2.r > r) current_color_2.r *= r;
		if (default_color_2.g > 0) if (current_color_2.g / default_color_2.g > g) current_color_2.g *= g;
		if (default_color_2.b > 0) if (current_color_2.b / default_color_2.b > b) current_color_2.b *= b;
		pozadi.setOutlineColor(current_color_2);
		//zmìna barvy textu
		sf::Color current_text_color = text_tlacitka.getFillColor();
		if (default_text_color.r > 0) if (text_tlacitka.getFillColor().r / default_text_color.r > r) current_text_color.r *= r;
		if (default_text_color.g > 0) if (text_tlacitka.getFillColor().g / default_text_color.g > g) current_text_color.g *= g;
		if (default_text_color.b > 0) if (text_tlacitka.getFillColor().b / default_text_color.b > b) current_text_color.b *= b;
		text_tlacitka.setFillColor(current_text_color);
		sf::Color current_outline_text_color = text_tlacitka.getOutlineColor();
		if (default_outline_text_color.r > 0) if (text_tlacitka.getOutlineColor().r / default_outline_text_color.r > r) current_outline_text_color.r *= r;
		if (default_outline_text_color.g > 0) if (text_tlacitka.getOutlineColor().g / default_outline_text_color.g > g) current_outline_text_color.g *= g;
		if (default_outline_text_color.b > 0) if (text_tlacitka.getOutlineColor().b / default_outline_text_color.b > b) current_outline_text_color.b *= b;
		text_tlacitka.setOutlineColor(current_outline_text_color);
		return true;
	}
	else {
		pozadi.setFillColor(default_color);
		pozadi.setOutlineColor(default_color_2);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}

bool SimpleButton::isPointed(sf::Vector2i poloha, sf::Vector3f color, const sf::RenderWindow & window, const float init_width, const float init_height) {
	if (
		(poloha.x > poloha_x_y.x*(window.getSize().x / init_width) && poloha.x < (poloha_x_y.x + velikost_x_y.x)*(window.getSize().x / init_width))
		&&
		(poloha.y > poloha_x_y.y*(window.getSize().y / init_height) && poloha.y < (poloha_x_y.y + velikost_x_y.y)*(window.getSize().y / init_height))
		)
	{
		//zmìna barvy pozadi
		sf::Color current_color = pozadi.getFillColor();
		if (default_color.r > 0) if (current_color.r / default_color.r > color.x) current_color.r *= color.x;
		if (default_color.g > 0) if (current_color.g / default_color.g > color.y) current_color.g *= color.y;
		if (default_color.b > 0) if (current_color.b / default_color.b > color.z) current_color.b *= color.z;
		pozadi.setFillColor(current_color);
		sf::Color current_color_2 = pozadi.getOutlineColor();
		if (default_color_2.r > 0) if (current_color_2.r / default_color_2.r > color.x) current_color_2.r *= color.x;
		if (default_color_2.g > 0) if (current_color_2.g / default_color_2.g > color.y) current_color_2.g *= color.y;
		if (default_color_2.b > 0) if (current_color_2.b / default_color_2.b > color.z) current_color_2.b *= color.z;
		pozadi.setOutlineColor(current_color_2);
		//zmìna barvy textu
		sf::Color current_text_color = text_tlacitka.getFillColor();
		if (default_text_color.r > 0) if (text_tlacitka.getFillColor().r / default_text_color.r > color.x) current_text_color.r *= color.x;
		if (default_text_color.g > 0) if (text_tlacitka.getFillColor().g / default_text_color.g > color.y) current_text_color.g *= color.y;
		if (default_text_color.b > 0) if (text_tlacitka.getFillColor().b / default_text_color.b > color.z) current_text_color.b *= color.z;
		text_tlacitka.setFillColor(current_text_color);
		return true;
	}
	else {
		pozadi.setFillColor(default_color);
		pozadi.setOutlineColor(default_color_2);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}


void SimpleButton::changeColor(sf::Color col, sf::Color col2) {
	default_color = col;
	pozadi.setFillColor(default_color);
	pozadi.setOutlineColor(col2);

}

void SimpleButton::changeTextColor(sf::Color c) {
	if (haveText) {
		default_text_color = c;
		text_tlacitka.setFillColor(default_text_color);
	}
}

void SimpleButton::addText(const sf::String &t,const sf::String &f,const sf::Vector2f &poloha, unsigned char_size,const sf::Color &color) {
	text_tlacitka.setString(t);
	font.loadFromFile(f);
	text_tlacitka.setFont(font);
	text_tlacitka.setPosition(poloha);
	if (char_size > 0)text_tlacitka.setCharacterSize(char_size);
	default_text_color = color;
	text_tlacitka.setFillColor(default_text_color);
	haveText = true;
}

void SimpleButton::addText(const sf::String &t, const sf::String &f, const float &poloha_x, const float &poloha_y, unsigned char_size, const sf::Color &color) {
	text_tlacitka.setString(t);
	font.loadFromFile(f);
	text_tlacitka.setFont(font);
	text_tlacitka.setPosition(sf::Vector2f(poloha_x, poloha_y));
	if (char_size > 0)text_tlacitka.setCharacterSize(char_size);
	default_text_color = color;
	text_tlacitka.setFillColor(default_text_color);
	haveText = true;
}

void SimpleButton::addText(sf::String f, float poloha_x, float poloha_y, unsigned char_size, sf::Color color){
	font.loadFromFile(f);
	text_tlacitka.setFont(font);
	text_tlacitka.setPosition(sf::Vector2f(poloha_x, poloha_y));
	if (char_size > 0)text_tlacitka.setCharacterSize(char_size);
	default_text_color = color;
	text_tlacitka.setFillColor(default_text_color);
	haveText = true;
}

void SimpleButton::addText(sf::Font & f, float poloha_x, float poloha_y, unsigned char_size, sf::Color color){
	font = f;
	text_tlacitka.setFont(font);
	text_tlacitka.setPosition(sf::Vector2f(poloha_x, poloha_y));
	if (char_size > 0)text_tlacitka.setCharacterSize(char_size);
	default_text_color = color;
	text_tlacitka.setFillColor(default_text_color);
	haveText = true;

}

void SimpleButton::addText(const sf::String & t, const sf::Font & f, float poloha_x, float poloha_y, unsigned char_size,const sf::Color &col){
	text_tlacitka.setString(t);
	font = f;
	text_tlacitka.setFont(font);
	text_tlacitka.setPosition(sf::Vector2f(poloha_x, poloha_y));
	if (char_size > 0)text_tlacitka.setCharacterSize(char_size);
	default_text_color = col;
	text_tlacitka.setFillColor(default_text_color);
	haveText = true;
}

void SimpleButton::changeString(const sf::String & s){
	text_tlacitka.setString(s);
}



//advenced button
AdvencedButton::AdvencedButton(sf::String textura_cesta, sf::Vector2f poloha, sf::Vector2u velikost, sf::Color col) {
	poloha_x_y = poloha;
	velikost_x_y = velikost;
	textura->loadFromFile(textura_cesta);
	pozadi_tlacitka->setTexture(*textura);
	pozadi_tlacitka->setScale((float)velikost_x_y.x / (float)textura->getSize().x, (float)velikost_x_y.y / (float)textura->getSize().y);
	default_color = col;
	pozadi_tlacitka->setColor(default_color);
	//nastaveni pozice
	pozadi_tlacitka->setPosition(poloha_x_y);
}

AdvencedButton::~AdvencedButton() {
	if (!received_texture) delete textura;textura = nullptr;
	delete pozadi_tlacitka; pozadi_tlacitka = nullptr;
}
AdvencedButton::AdvencedButton(sf::String textura_cesta) {
	textura = new sf::Texture;
	textura->loadFromFile(textura_cesta);
	pozadi_tlacitka = new sf::Sprite(*textura);
	default_color = sf::Color::White;
	received_texture = false;
}

AdvencedButton::AdvencedButton(sf::Vector2f poloha, sf::Vector2u velikost, sf::Color col){
	poloha_x_y = poloha;
	velikost_x_y = velikost;
	pozadi_tlacitka = new sf::Sprite;
	pozadi_tlacitka->setScale((float)velikost_x_y.x / (float)textura->getSize().x, (float)velikost_x_y.y / (float)textura->getSize().y);
	default_color = col;
	pozadi_tlacitka->setColor(default_color);
	//nastaveni pozice
	pozadi_tlacitka->setPosition(poloha_x_y);
}

AdvencedButton::AdvencedButton(const AdvencedButton & button) : Button(button) {
	//nastavi pozici
	poloha_x_y = button.poloha_x_y;
	velikost_x_y = button.velikost_x_y;
	//nastavi sprite
	pozadi_tlacitka = new sf::Sprite(*button.pozadi_tlacitka);
	//nastavi texturu
	received_texture = button.received_texture;
	if (received_texture) textura = button.textura;
	else textura = new sf::Texture(*button.textura);
	//nastavi zvuk
	received_sound = button.received_sound;
	if(received_sound)buffer = button.buffer;
	else buffer = new sf::SoundBuffer(*button.buffer);
	sound = new sf::Sound(*buffer);
	//nastavi pozadi
	pozadi_tlacitka->setTexture(*textura);
	default_color = button.default_color;
	//nastavi text
	font = button.font;
	text_tlacitka = button.text_tlacitka;
	text_tlacitka.setFont(font);
	haveText = button.haveText;
}

void AdvencedButton::drawButton(sf::RenderWindow & window) {
	window.draw(*pozadi_tlacitka);
	if (haveText)window.draw(text_tlacitka);
}

AdvencedButton::AdvencedButton(sf::Texture *tex, float poloha_x, float poloha_y, float velikost_x, float velikost_y) {
	poloha_x_y.x = poloha_x;
	poloha_x_y.y = poloha_y;
	velikost_x_y.x = velikost_x;
	velikost_x_y.y = velikost_y;
	textura = tex;
	pozadi_tlacitka = new sf::Sprite(*textura);
	pozadi_tlacitka->setScale((float)velikost_x_y.x / (float)textura->getSize().x, (float)velikost_x_y.y / (float)textura->getSize().y);
	pozadi_tlacitka->setPosition(poloha_x_y);
	default_color = sf::Color::White;
	haveText = false;
	received_texture = true;
}
AdvencedButton::AdvencedButton(const MediaHandle * mh, float poloha_x, float poloha_y, float velikost_x, float velikost_y){
	poloha_x_y.x = poloha_x;
	poloha_x_y.y = poloha_y;
	velikost_x_y.x = velikost_x;
	velikost_x_y.y = velikost_y;

	textura = mh->texture;
	pozadi_tlacitka = new sf::Sprite(*textura);
	pozadi_tlacitka->setScale((float)velikost_x_y.x / (float)textura->getSize().x, (float)velikost_x_y.y / (float)textura->getSize().y);
	pozadi_tlacitka->setPosition(poloha_x_y);

	buffer = mh->soundBuffer;
	sound = new sf::Sound(*buffer);

	default_color = sf::Color::White;
	haveText = false;
	received_texture = true;
	received_sound = true;
}
bool AdvencedButton::isPointed(sf::Vector2i poloha, sf::Vector3f color) {
	if ((poloha.x > poloha_x_y.x && poloha.x < poloha_x_y.x + velikost_x_y.x) && (poloha.y > poloha_x_y.y && poloha.y < poloha_x_y.y + velikost_x_y.y)) {
		sf::Color current_color = pozadi_tlacitka->getColor();
		current_color.r *= color.x;
		current_color.g *= color.y;
		current_color.b *= color.z;
		pozadi_tlacitka->setColor(current_color);
		sf::Color current_text_color = text_tlacitka.getFillColor();
		current_text_color.r *= color.x;
		current_text_color.g *= color.y;
		current_text_color.b *= color.z;
		text_tlacitka.setFillColor(current_text_color);
		return true;
	}
	else {
		pozadi_tlacitka->setColor(default_color);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}

bool AdvencedButton::isPointed(sf::Vector2i poloha, float r, float g, float b) {
	if ((poloha.x > poloha_x_y.x && poloha.x < poloha_x_y.x + velikost_x_y.x) && (poloha.y > poloha_x_y.y && poloha.y < poloha_x_y.y + velikost_x_y.y)) {
		sf::Color current_color = pozadi_tlacitka->getColor();
		current_color.r *= r;
		current_color.g *= g;
		current_color.b *= b;
		pozadi_tlacitka->setColor(current_color);
		sf::Color current_text_color = text_tlacitka.getFillColor();
		current_text_color.r *= r;
		current_text_color.g *= g;
		current_text_color.b *= b;
		text_tlacitka.setFillColor(current_text_color);
		return true;
	}
	else {
		pozadi_tlacitka->setColor(default_color);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}

}

bool AdvencedButton::isPointed(sf::Vector2i poloha, float r, float g, float b, const sf::RenderWindow & window, const float init_width, const float init_height) {
	if (
		(poloha.x > poloha_x_y.x*(window.getSize().x / init_width) && poloha.x < (poloha_x_y.x + velikost_x_y.x)*(window.getSize().x / init_width))
		&&
		(poloha.y > poloha_x_y.y*(window.getSize().y / init_height) && poloha.y < (poloha_x_y.y + velikost_x_y.y)*(window.getSize().y / init_height))
		)
	{
#ifdef DEBUG
		std::cout << "Ukazuje na tlacitko!" << std::endl;
#endif
		sf::Color current_color = pozadi_tlacitka->getColor();
		current_color.r *= r;
		current_color.g *= g;
		current_color.b *= b;
		pozadi_tlacitka->setColor(current_color);
		sf::Color current_text_color = text_tlacitka.getFillColor();
		current_text_color.r *= r;
		current_text_color.g *= g;
		current_text_color.b *= b;
		text_tlacitka.setFillColor(current_text_color);
		return true;
	}
	else {
		pozadi_tlacitka->setColor(default_color);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}

bool AdvencedButton::isPointed(sf::Vector2i poloha, sf::Vector3f color, const sf::RenderWindow & window, const float init_width, const float init_height) {
	if (
		(poloha.x > poloha_x_y.x*(window.getSize().x / init_width) && poloha.x < (poloha_x_y.x + velikost_x_y.x)*(window.getSize().x / init_width))
		&&
		(poloha.y > poloha_x_y.y*(window.getSize().y / init_height) && poloha.y < (poloha_x_y.y + velikost_x_y.y)*(window.getSize().y / init_height))
		) {
		sf::Color current_color = pozadi_tlacitka->getColor();
		current_color.r *= color.x;
		current_color.g *= color.y;
		current_color.b *= color.z;
		pozadi_tlacitka->setColor(current_color);
		sf::Color current_text_color = text_tlacitka.getFillColor();
		current_text_color.r *= color.x;
		current_text_color.g *= color.y;
		current_text_color.b *= color.z;
		text_tlacitka.setFillColor(current_text_color);
		return true;
	}
	else {
		pozadi_tlacitka->setColor(default_color);
		text_tlacitka.setFillColor(default_text_color);
		return false;
	}
}



void AdvencedButton::changeColor(sf::Color col) {
	default_color = col;
	pozadi_tlacitka->setColor(default_color);
}

void AdvencedButton::setPosition(sf::Vector2f poloha) {
	poloha_x_y = poloha;
	pozadi_tlacitka->setPosition(poloha_x_y);
}

void AdvencedButton::setPosition(float x, float y) {
	poloha_x_y.x = x;
	poloha_x_y.y = y;
	pozadi_tlacitka->setPosition(poloha_x_y);
}

void AdvencedButton::setSize(sf::Vector2u velikost) {
	velikost_x_y = velikost;
	pozadi_tlacitka->setScale((float)velikost_x_y.x / (float)textura->getSize().x, (float)velikost_x_y.y / (float)textura->getSize().y);
}

void AdvencedButton::setSize(unsigned x, unsigned y) {
	velikost_x_y.x = x;
	velikost_x_y.y = y;
	pozadi_tlacitka->setScale((float)velikost_x_y.x / (float)textura->getSize().x, (float)velikost_x_y.y / (float)textura->getSize().y);
}

