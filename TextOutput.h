#pragma once
#include "Output.h"

class TextOutput : public OutputElement{
public:
	TextOutput(float pos_x, float pos_y, float size,Font* font,RenderWindow* window);
	TextOutput(Vector2f pos, float size, Font* font, RenderWindow* window);
	~TextOutput();
	void draw();
	void setValue(const string &value);
	void setColor(Color fillColor, Color outlineColor = Color::Transparent);
	void setFont(Font* font);
	void setFont(string path);
	void setBackgroundTexture(string path) {}
	void setBackgroundTexture(Texture* texture) {}
	void setBackgroundRectTexture(string path, IntRect rect) {}
	void setBackgroundRectTexture(Texture* texture, IntRect rect) {}
	void setSpecialTexture(Texture* texture) {}
	void setSpecialTexture(string path) {}
	void setSpecialRectTexture(Texture* texture, IntRect rect) {}
	void setSpecialRectTexture(string path, IntRect rect) {}

	Vector2f getSize();
	void changePos(const float &x,const float &y);
	void update(Event* _event, Mouse* _mouse);
private:
	Text* _text = nullptr;
	Font* _font = nullptr;
	Vector2f _position;
	bool loadedFont = false;
};

