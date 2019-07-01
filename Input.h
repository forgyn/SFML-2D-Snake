#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;
class InputElement{
public:
	void setWindow(RenderWindow* window) { _window = window; }
	virtual void draw() = 0;
	virtual void update(Event* _event, Mouse* _mouse) = 0;
	Vector2f getRatio() { return _ratio; }
	void updateRatio(Event* _event);
	virtual void setBackgroundTexture(Texture *texture) = 0;
	virtual void setBackgroundTexture(string path) = 0;
	virtual void setBackgroundRectTexture(Texture* texture,IntRect rect) = 0;
	virtual void setBackgroundRectTexture(string path, IntRect rect) = 0;
	virtual void setSpecialTexture(Texture* texture) = 0;
	virtual void setSpecialTexture(string path) = 0;
	virtual void setSpecialRectTexture(Texture* texture, IntRect rect) = 0;
	virtual void setSpecialRectTexture(string path, IntRect rect) = 0;
protected:
	RenderWindow* _window = nullptr;
	//Mouse* _mouse = nullptr;
	//Event* _event = nullptr;
	Vector2f _ratio = Vector2f(1,1);
	Vector2u _defaultWindowSize;
	bool resized = false;
};

