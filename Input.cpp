#include "Input.h"

void InputElement::updateRatio(Event* _event){
	if (_event->type == Event::Resized) {
		if (_defaultWindowSize == _window->getSize())resized = false;
		else resized = true;
		_ratio.x = (float)_window->getSize().x / (float)_defaultWindowSize.x;
		_ratio.y = (float)_window->getSize().y / (float)_defaultWindowSize.y;
	}
	if (!resized)_defaultWindowSize = _window->getSize();
	
}
