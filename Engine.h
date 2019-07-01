#pragma once
//#include <iostream>
//#include <string>
//#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#define DEFAULT_FONT "fonty/default_font.otf"
using namespace sf;
namespace fel {
	struct SimplePicture {
		SimplePicture(const Vector2f& pos, const Vector2f& size, Texture* texture) {
			_body.setPosition(pos);
			_body.setOrigin(size.x / 2, size.y / 2);
			_body.setSize(size);
			_body.setTexture(texture);
		}
		void setTexture(Texture* texture) { _body.setTexture(texture); }
		void draw(RenderWindow* window) { window->draw(_body); }
	private:
		RectangleShape _body;
	};

	struct TextureBuffer {
		TextureBuffer(const size_t& size) :_size(size) {
			_texture_buffer = new Texture[_size];
		}
		~TextureBuffer() { delete[] _texture_buffer; }
		Texture& operator[](const size_t& n) {
			if (n < _size && n >= 0)return _texture_buffer[n];
		}
	private:
		const size_t _size;
		Texture* _texture_buffer;
	};


	struct MediaHandle {
		MediaHandle() { texture = nullptr; soundBuffer = nullptr; }
		~MediaHandle() { delete texture; texture = nullptr; delete soundBuffer; soundBuffer = nullptr; }
		MediaHandle(const sf::String& tex_path, const sf::String& sound_path) {
			texture = new sf::Texture;
			texture->loadFromFile(tex_path);
			soundBuffer = new sf::SoundBuffer;
			if (!soundBuffer->loadFromFile(sound_path))soundBuffer->loadFromFile(default_sound_path);

		}
		MediaHandle(const MediaHandle& mh) {
			texture = new sf::Texture(*mh.texture);
			soundBuffer = new sf::SoundBuffer(*mh.soundBuffer);
		}
		void operator=(const MediaHandle& mh) {
			texture = new sf::Texture(*mh.texture);
			soundBuffer = new sf::SoundBuffer(*mh.soundBuffer);
		}
		sf::Texture* texture = nullptr;
		sf::SoundBuffer* soundBuffer = nullptr;
		static sf::String default_sound_path;
	};


	struct DefaultButtonHandle {
		DefaultButtonHandle(sf::String Text, float X, float Y, unsigned Size_y) {
			text = Text;
			x = X;
			y = Y;
			size_y = Size_y;
		}
		sf::String text;
#ifdef DEFAULT_FONT
		sf::String font = DEFAULT_FONT;
#else 
		sf::String font;
#endif
		float x;
		float y;
		unsigned size_y;
		sf::Color col1 = sf::Color::Transparent;
		sf::Color col2 = sf::Color::Transparent;
		sf::Color col3 = sf::Color::Transparent;
		sf::Color col4 = sf::Color::Transparent;
	};

	struct Button {
		Button() {  }
		virtual Button* Clone() const = 0;
		virtual ~Button() {
			delete sound; sound = nullptr;
			if (!received_sound)delete buffer;
		}
		virtual void drawButton(sf::RenderWindow& window) {};
		virtual void drawButton(const sf::String& str, sf::RenderWindow& window) {};
		virtual bool isPointed(sf::Vector2i);
		virtual bool isPointed(sf::Vector2i, const sf::RenderWindow&, const float, const float);
		virtual bool isPointed(sf::Vector2i, sf::Vector3f) { return false; }
		virtual bool isPointed(sf::Vector2i, float, float, float) { return false; };
		virtual bool isPointed(sf::Vector2i poloha, float r, float g, float b, const sf::RenderWindow& window, const float init_width, const float init_height) { return false; }
		virtual bool isPointed(sf::Vector2i poloha, sf::Vector3f, const sf::RenderWindow& window, const float init_width, const float init_height) { return false; }
		virtual void changeTextColor(sf::Color) {};
		virtual void addText(sf::String t, sf::String f, sf::Vector2f poloha, unsigned = 0, sf::Color = sf::Color::White) {};
		virtual void addText(sf::String t, sf::String f, float poloha_x, float poloha_y, unsigned = 0, sf::Color = sf::Color::White) {};
		virtual void addText(sf::String f, float poloha_x, float poloha_y, unsigned = 0, sf::Color = sf::Color::White) {};
		virtual void addText(sf::Font& f, float poloha_x, float poloha_y, unsigned = 0, sf::Color = sf::Color::White) {};
		virtual void changeTextColor(sf::Color, sf::Color = sf::Color::Transparent);
		virtual void changeTextThickness(float);
		virtual void setPosition(float, float) {};
		virtual void setTexture(sf::Texture* tex) {}
		virtual void update() {};
		virtual sf::String getString()const { return text_tlacitka.getString(); }
		void playSound(const unsigned short& = 100);
		void addSound(const sf::String&);
		void setSound(sf::SoundBuffer*);
		sf::Sound& getSound() { return *sound; }
		unsigned short button_id = 0;
		void setVolume(const unsigned short& vol) { volume = vol; }
	protected:
		sf::Vector2f poloha_x_y;
		sf::Vector2u velikost_x_y;
		sf::Color default_color;
		sf::Color default_text_color;
		sf::Color default_outline_text_color = sf::Color::Transparent;
		sf::Text text_tlacitka;
		sf::Font font;
		sf::SoundBuffer* buffer = nullptr;
		sf::Sound* sound = nullptr;
		bool haveText = false;
		bool received_sound = false;
		unsigned short volume = 100;
	};
	struct SimpleButton : public Button {
		SimpleButton(const SimpleButton& button);
		SimpleButton() {}
		SimpleButton(sf::Vector2f poloha, sf::Vector2u velikost = sf::Vector2u(100, 100), sf::Color color = sf::Color(255, 255, 255), sf::Color color2 = sf::Color(255, 255, 255), float thicc = 0.f);
		SimpleButton(float poloha_x, float poloha_y, unsigned velikost_x, unsigned velikost_y, sf::Color color = sf::Color(255, 255, 255), sf::Color color2 = sf::Color(255, 255, 255), float thicc = 0.f);

		Button* Clone() const { return new SimpleButton(*this); };
		void drawButton(sf::RenderWindow& window);
		void drawButton(const sf::String& str, sf::RenderWindow& window);
		void drawButton(sf::RenderWindow& window, const float& x, const float& y) {};
		bool isPointed(sf::Vector2i, sf::Vector3f);
		bool isPointed(sf::Vector2i, float, float, float);
		bool isPointed(sf::Vector2i poloha, float r, float g, float b, const sf::RenderWindow& window, const float init_width, const float init_height);
		bool isPointed(sf::Vector2i poloha, sf::Vector3f color, const sf::RenderWindow& window, const float init_width, const float init_height);
		void changeColor(sf::Color, sf::Color col2 = sf::Color(sf::Color::Transparent));
		void changeTextColor(sf::Color);
		void addText(const sf::String& t, const sf::String& f, const sf::Vector2f& poloha, unsigned char_size, const sf::Color& color);
		void addText(const sf::String& t, const sf::String& f, const float& poloha_x, const float& poloha_y, unsigned char_size, const sf::Color& color);
		void addText(sf::String f, float poloha_x, float poloha_y, unsigned = 0, sf::Color = sf::Color::White);
		void addText(sf::Font& f, float poloha_x, float poloha_y, unsigned = 0, sf::Color = sf::Color::White);
		void addText(const sf::String& t, const sf::Font& f, float poloha_x, float poloha_y, unsigned = 0, const sf::Color & = sf::Color::White);
		void changeString(const sf::String& s);
		void setPosition(float, float) {};
		void update() {};
	private:
		sf::RectangleShape pozadi;
		sf::Color default_color_2;
		float thicc;
	};

	struct AdvencedButton : public Button {
		AdvencedButton() {}
		~AdvencedButton();
		Button* Clone() const { return new AdvencedButton(*this); }
		AdvencedButton(sf::String textura_cesta, sf::Vector2f poloha, sf::Vector2u velikost, sf::Color col = sf::Color(255, 255, 255));
		AdvencedButton(sf::String textura_cesta);
		AdvencedButton(sf::Vector2f poloha, sf::Vector2u velikost, sf::Color col = sf::Color::Transparent);
		AdvencedButton(const AdvencedButton& button);
		//AdvencedButton(sf::Texture, sf::Sprite, sf::Vector2f, sf::Vector2u);
		//AdvencedButton(sf::Texture, sf::Sprite, float, float, float, float);
		void drawButton(sf::RenderWindow& window);
		AdvencedButton(sf::Texture* tex, float poloha_x, float poloha_y, float velikost_x, float velikost_y);
		AdvencedButton(const MediaHandle* mh, float poloha_x, float poloha_y, float velikost_x, float velikost_y);
		bool isPointed(sf::Vector2i mouse, sf::Vector3f color);
		bool isPointed(sf::Vector2i, float, float, float);
		bool isPointed(sf::Vector2i poloha, float r, float g, float b, const sf::RenderWindow& window, const float init_width, const float init_height);
		bool isPointed(sf::Vector2i poloha, sf::Vector3f color, const sf::RenderWindow& window, const float init_width, const float init_height);
		void changeColor(sf::Color);
		void setPosition(sf::Vector2f);
		void setPosition(float, float);
		void setSize(sf::Vector2u);
		void setSize(unsigned, unsigned);
		void setTexture(sf::Texture* tex) { textura = tex; received_texture = true; }
	private:
		sf::Sprite* pozadi_tlacitka = nullptr;
		sf::Texture* textura = nullptr;
		bool received_texture = false;
	};


	struct DefaultButton {
		DefaultButton() { tlacitko = nullptr; }
		DefaultButton(const sf::String& text, const sf::String& font, const float& x, const float& y, const unsigned& size_y, sf::Color col1 = sf::Color::Transparent, sf::Color col2 = sf::Color::Transparent, sf::Color col3 = sf::Color::White) {
			tlacitko = new SimpleButton(x, y, size_y * text.getSize() / 2, size_y + 5, col1, col2, 1);
			tlacitko->addText(text, font, x, y, size_y, col3);
		}
		DefaultButton(const DefaultButtonHandle& handle) {
			tlacitko = new SimpleButton(handle.x, handle.y, handle.size_y * handle.text.getSize() / 2, handle.size_y, handle.col1, handle.col2, 1);
			tlacitko->addText(handle.text, handle.font, handle.x, handle.y, handle.size_y, handle.col3);
		}
		DefaultButton(const DefaultButton& button) {
			tlacitko = button.tlacitko->Clone();
		}
		void draw(sf::RenderWindow& window) { tlacitko->drawButton(window); }
		bool isPointed(sf::Vector2i mouse) { return tlacitko->isPointed(mouse); }
		void isPointed(sf::Vector2i mouse, float r, float g, float b) { tlacitko->isPointed(mouse, r, g, b); }
		~DefaultButton() {
			if (tlacitko != nullptr) {
				delete tlacitko;
				tlacitko = nullptr;
			}
		}
	private:
		Button* tlacitko;
	};

	struct Pexeso {
		Pexeso() {
			def_side = nullptr;
			flip_side = nullptr;
			fliped = false;
			reveal = false;
		}
		~Pexeso() {
			delete def_side, def_side = nullptr;
			delete flip_side, flip_side = nullptr;
		}
		Pexeso(const SimpleButton& _tmp_bt_1, const SimpleButton& _tmp_bt_2) {
			def_side = new SimpleButton(_tmp_bt_1);
			flip_side = new SimpleButton(_tmp_bt_2);
			fliped = false;
			reveal = false;
		}
		Pexeso(const AdvencedButton& _tmp_bt_1, const AdvencedButton& _tmp_bt_2) {
			if (def_side == nullptr) def_side = new AdvencedButton(_tmp_bt_1);
			else {
				delete def_side; def_side = nullptr;
				def_side = new AdvencedButton(_tmp_bt_1);
			}
			if (flip_side == nullptr) flip_side = new AdvencedButton(_tmp_bt_2);
			else {
				delete flip_side; flip_side = nullptr;
				flip_side = new AdvencedButton(_tmp_bt_2);
			}
			fliped = false;
			reveal = false;
		}
		Pexeso(const Pexeso& pexeso) {
			def_side = pexeso.def_side->Clone();
			flip_side = pexeso.flip_side->Clone();
			fliped = pexeso.fliped;
			reveal = pexeso.reveal;
			pexeso_id = pexeso.pexeso_id;
		}
		void operator=(const Pexeso& pexeso) {
			def_side = pexeso.def_side->Clone();
			flip_side = pexeso.flip_side->Clone();
			fliped = pexeso.fliped;
			reveal = pexeso.reveal;
			pexeso_id = pexeso.pexeso_id;
		}
		void addSound(const sf::String& audio_path) {
			def_side->addSound(audio_path);
		}
		void setSound(sf::SoundBuffer* copy_buffer) {
			def_side->setSound(copy_buffer);
		}
		void setVolume(const unsigned short& vol) {
			def_side->setVolume(vol);
			flip_side->setVolume(vol);
		}
		void setTexture(sf::Texture* def_tex, sf::Texture* flip_texture) {
			def_side->setTexture(def_tex);
			flip_side->setTexture(def_tex);
		}
		void playDefSound() {
			if (def_side->getSound().getStatus() != sf::Sound::Playing)def_side->getSound().play();
		}
		void playFlipSound() {
			if (flip_side->getSound().getStatus() != sf::Sound::Playing)flip_side->getSound().play();
		}
		void changeVolume(float volume) {
			def_side->getSound().setVolume(volume);
			flip_side->getSound().setVolume(volume);
		}
		void draw(sf::RenderWindow& window) {
			if (!fliped)def_side->drawButton(window);
			else flip_side->drawButton(window);
		}
		bool isPointed(sf::Vector2i mouse, sf::Vector3f color) {
			if (!fliped)return def_side->isPointed(mouse, color);
			else return flip_side->isPointed(mouse, color);
		}
		bool isPointed(sf::Vector2i mouse, float r, float g, float b) {
			if (!fliped)return def_side->isPointed(mouse, r, g, b);
			else return flip_side->isPointed(mouse, r, g, b);
		}
		bool isPointed(const sf::Vector2i& poloha, const float& r, const float& g, const float& b, const sf::RenderWindow& window, const float& init_width, const float& init_height) {
			if (!fliped)return def_side->isPointed(poloha, r, g, b, window, init_width, init_height);
			else return flip_side->isPointed(poloha, r, g, b, window, init_width, init_height);
		}
		bool isPointed(const sf::Vector2i& poloha, const sf::RenderWindow& window, const float& init_width, const float& init_height) {
			if (!fliped)return def_side->isPointed(poloha, window, init_width, init_height);
			else return flip_side->isPointed(poloha, window, init_width, init_height);
		}
		bool fliped = false;
		bool reveal = false;
		unsigned pexeso_id = 0;
	private:
		Button* def_side = nullptr;
		Button* flip_side = nullptr;
	};

	struct SimpleText {
		SimpleText() {}
		SimpleText(const sf::String& str, const float& x, const float& y, const size_t& size = 50) {
			font.loadFromFile(DEFAULT_FONT);
			text = sf::Text(str, font, size);
			text.setPosition(x, y);
			text.setFillColor(sf::Color::Black);
		}
		SimpleText(const sf::String& str, const sf::String& f, const float& x, const float& y, const size_t& size = 50) {
			font.loadFromFile(f);
			text = sf::Text(str, font, size);
			text.setPosition(x, y);
			text.setFillColor(sf::Color::Black);
		}
		SimpleText(const sf::String& str, sf::Font& f, const float& x, const float& y, const size_t& size = 50) {
			font = f;
			text = sf::Text(str, font, size);
			text.setPosition(x, y);
			text.setFillColor(sf::Color::Black);
		}
		SimpleText(const float& x, const float& y, const size_t& size = 50) {
			font.loadFromFile(DEFAULT_FONT);
			text.setFont(font);
			text.setCharacterSize(size);
			text.setPosition(x, y);
			text.setFillColor(sf::Color::Black);
		}
		SimpleText(sf::Font& f, const float& x, const float& y, const size_t& size = 50) {
			font = f;
			text.setFont(font);
			text.setCharacterSize(size);
			text.setPosition(x, y);
			text.setFillColor(sf::Color::Black);
		}
		SimpleText(const SimpleText& txt) {
			text = txt.text;
			font.loadFromFile(DEFAULT_FONT);
			text.setFont(font);
		}
		void draw(sf::RenderWindow& window) { window.draw(text); }
		void draw(const sf::String& str, sf::RenderWindow& window) {
			text.setString(str);
			window.draw(text);
		}
		sf::Text text;
	private:
		sf::Font font;
	};

}