#pragma once
#include "Input.h"

#define TEXT_RATIO 0.5f
#define BASIC_COLOR Color::Yellow

class Button : public InputElement {
public:
	Button(float pos_x, float pos_y, float size_x, float size_y,RenderWindow* window);
	Button(Vector2f pos, Vector2f size, RenderWindow* window);
	~Button();
	void draw();
	virtual void update(Event* _event, Mouse* _mouse);
	bool isPointed(Mouse* _mouse);
	void setColor(const Color &color);
	void setBackgroundTexture(string path);
	void setBackgroundTexture(Texture* texture);
	void setBackgroundRectTexture(string path, IntRect rect);
	void setBackgroundRectTexture(Texture* texture, IntRect rect);
	void setSpecialTexture(Texture* texture) {}
	void setSpecialTexture(string path) {}
	void setSpecialRectTexture(Texture* texture, IntRect rect) {}
	void setSpecialRectTexture(string path, IntRect rect) {}
	bool isPressed(bool unpress = true);
	Vector2f getPos() { return Vector2f(_position.x-_backgroud->getOrigin().x,_position.y - _backgroud->getOrigin().y); }
	void reset();
	
	void changePos(Vector2f pos);
	void changeSize(Vector2f size);
protected:
	Vector2f _size;
	Vector2f _position;
	Vector2u _defaultWindowSize;
	RectangleShape* _backgroud = nullptr;
	Color _basicColor;
	Texture* _texture = nullptr;
	bool loadedTexture = false;
	bool PRESSED = false;
};

class DraggableButton : public Button {
public:
	DraggableButton(float size_x, float size_y, float pos_x, float pos_y, RenderWindow* window);
	DraggableButton(Vector2f size, Vector2f pos, RenderWindow* window);
	void update(Event* _event, Mouse* _mouse);
	bool FOLLOWING = false;
	void changeMod(unsigned mod) { _mod = mod; }
	void reset();
	Vector2f getLeftUpCorner() { return Vector2f(_position.x,_position.y); }
	Vector2f getRightDownCorner() { return Vector2f(_position.x + _size.x, _position.y + _size.y); }
	void setLimit(float min_x,float max_x, float min_y,float max_y);
	
private:
	//Vector2i _previousMousePos;
	unsigned _mod = 0;
	Vector2f _limit_max;
	Vector2f _limit_min;
};

class TextButton : public Button {
public:
	TextButton(float size_x, float size_y, float pos_x, float pos_y, const string& text, RenderWindow* window);
	TextButton(float size_x, float size_y, float pos_x, float pos_y, const string& text, RenderWindow* window, Font* font);
	~TextButton();
	void draw();
	void update(Event* _event, Mouse* _mouse);
	void changeStyle(Uint32 style);
	void setFont(Font* font);
	void setFont(string path);
	void setColor(Color bck_col,Color txt_col = Color::Black);
	void setString(string str) { _text->setString(str); }
	void setTextRatio(float ratio);
	void getDebugInfo();
private:
	Text *_text = nullptr;
	bool loadedFont = false;
	float _text_ratio = 1.f;
	Font* _font = nullptr;
	Color _basic_text_color;
};