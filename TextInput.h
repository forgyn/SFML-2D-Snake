#pragma once
#include "Input.h"
#include "Button.h"
#include <string>
#define OFFSET_X 0.02
class TextInput : InputElement{
public:
	TextInput(float size_x, float size_y, float pos_x, float pos_y,Font* font, RenderWindow* window);
	TextInput(Vector2f size, Vector2f pos, Font* font, RenderWindow* window);
	void draw();
	void update(Event* _event, Mouse* _mouse);
	bool ACTIVATED = false;
	void addChar(Event* _event);
	void setFont(Font* font);
	void setBackgroundTexture(string path);
	void setBackgroundTexture(Texture* texture);
	void setBackgroundRectTexture(string path, IntRect rect);
	void setBackgroundRectTexture(Texture* texture, IntRect rect);
	void setSpecialTexture(Texture* texture) {}
	void setSpecialTexture(string path) {}
	void setSpecialRectTexture(Texture* texture, IntRect rect) {}
	void setSpecialRectTexture(string path, IntRect rect) {}
	string getInput() { return _input; };
	~TextInput();
private:
	Texture* _texture = nullptr;
	bool loadedTexture = false;
	Font* _font = nullptr;
	Clock _clock;
	Button* _button = nullptr;
	RectangleShape* _background = nullptr;
	RectangleShape* _cursor = nullptr;
	string _input = "";
	Text* _inputText = nullptr;
	Vector2f _size;
	Vector2f _position;
};

