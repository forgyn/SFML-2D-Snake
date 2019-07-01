#pragma once
#include "Output.h"
class Bar : OutputElement{
public:
	Bar(float pos_x, float pos_y, float size_x, float size_y,RenderWindow* window);
	Bar(Vector2f pos, Vector2f size, RenderWindow* window);
	void setValue(const string &value);
	void setBackgroundTexture(string path);
	void setBackgroundTexture(Texture* texture);
	void setBackgroundRectTexture(string path, IntRect rect);
	void setBackgroundRectTexture(Texture* texture, IntRect rect);
	void setFrontTexture(Texture* texture);
	void setFrontTexture(string path);
	void setFrontRectTexture(Texture* texture, IntRect rect);
	void setFrontRectTexture(string path, IntRect rect);
	void setIndicatorTexture(string path);
	void setIndicatorTexture(Texture* texture);
	void setIndicatorRectTexture(string path, IntRect rect);
	void setIndicatorRectTexture(Texture* texture, IntRect rect);
	void setColor(Color fillColor, Color outlineColor = Color::Transparent);
	void update(Event* _event, Mouse* _mouse);
	void draw();
	~Bar();
private:
	Vector2f _size;
	Vector2f _position;
	bool _loadedBackgroundTexture = false;
	bool _loadedFrontgroundTexture = false;
	bool _loadedIndicatorTexture = false;
	RectangleShape* _background = nullptr;
	RectangleShape* _frontground = nullptr;
	RectangleShape* _indicator = nullptr;
	Texture* _backgroundTexture = nullptr;
	Texture* _frontgroundTexture = nullptr;
	Texture* _indicatorTexture = nullptr;
};

