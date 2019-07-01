#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#define BASIC_TEXTBAR_BACKGROUND_COLOR Color::Yellow
#define BASIC_TEXTBAR_TEXT_COLOR Color::Black

class OutputElement{
public:
	void setWindow(RenderWindow* window) { _window = window; }
	virtual void setBackgroundTexture(Texture* texture) = 0;
	virtual void setBackgroundTexture(string path) = 0;
	virtual void setBackgroundRectTexture(Texture* texture, IntRect rect) = 0;
	virtual void setBackgroundRectTexture(string path, IntRect rect) = 0;
	virtual void setSpecialTexture(Texture* texture) = 0;
	virtual void setSpecialTexture(string path) = 0;
	virtual void setSpecialRectTexture(Texture* texture, IntRect rect) = 0;
	virtual void setSpecialRectTexture(string path, IntRect rect) = 0;
	virtual void draw() = 0;
	virtual void update(Event* _event,Mouse* _mouse) = 0;
	virtual void setValue(const string &) = 0;
protected:
	string _value;
	RenderWindow* _window;
};

