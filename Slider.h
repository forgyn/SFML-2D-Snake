#pragma once
#include "Input.h"
#include "Button.h"
class Slider : public InputElement{
public:
	Slider(float size_x, float size_y, float pos_x, float pos_y,RenderWindow* window);
	Slider(Vector2f size, Vector2f pos,RenderWindow* window);
	~Slider();
	void update(Event* _event, Mouse* _mouse);
	void setBackgroundTexture(Texture* texture);
	void setBackgroundTexture(string path);
	void setBackgroundRectTexture(Texture* texture,IntRect rec);
	void setBackgroundRectTexture(string path,IntRect rec);
	void setSpecialTexture(Texture* texture);
	void setSpecialTexture(string path);
	void setSpecialRectTexture(Texture* texture, IntRect rec);
	void setSpecialRectTexture(string path, IntRect rect);
	void changeRatio(float ration);
	void draw();
	void release();
	void reset();
	Vector2f getInput() { return _sliderPos; }
	void setInput(Vector2f value);
private:
	DraggableButton* _button = nullptr;
	uint8_t _mod;
	Texture* _texture = nullptr;
	bool _loadedTexture = false;
	Vector2f _size;
	Vector2f _position;
	Vector2f _sliderPos;
	float _ration = 10;
	RectangleShape *_backgroud = nullptr;
};

