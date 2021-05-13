#ifndef AButt_h
#define AButt_h

class AButt
{
public:
	AButt(int pin, bool inverted = false, bool isDigital = true, unsigned int debounce = 50);

	void update();

	void onClick(void(*callback)(unsigned short));
	void onHold(void(*start)(), void(*end)());

	//return state including de-bounce
	bool isPressed();
	bool isHeld();

	//return state excluding de-bounce, including inversion etc
	bool getState(); 

	// time before a click is registered as a new sequense
	void setClickDelay(unsigned int delay);
	int getClickDelay();

	// time before a click becomes a hold
	void setHoldDelay(unsigned int delay);
	int getHoldDelay();

	// maximum number of clicks in a sequence
	void setMaxClicks(unsigned short clicks);
	short getMaxClicks();

private:
	int _pin; //pin conected to the button
	bool _inverted; //if inv is true, the button is pressed when there is 0V on Pin
	bool _isDigital; //is the pin digital, digital read on analog pins can cause problems sometime

	bool _isPressed; //is btn pressed. This INCLUDES the debounce compensation.
	bool _isHeld; //is the btn beeing hold?
	bool _wasPressed; //was btn pressed. This INCLUDES the debounce compensation.

	short clickCount; //counting the number of double clicks

	bool _lastState; //state of button in previous update 
	unsigned long _lastDebounceTime; //start last btn state change
	unsigned long _lastPressTime; //start of the last btn press, including de-bounce
	unsigned long _lastClickTime; //start of the last btn release, including de-bounce

	unsigned int _debounceDelay; //nr of ms the btn needs to stay pressed
	unsigned int _clickDelay; //time before a click is registered as a new sequense
	unsigned int _holdDelay; //time before a click becomes a hold

	unsigned short _maxClicks; //maximum number of clicks in a sequence

	void(*_clickCallback)(unsigned short);
	void(*_holdStartCallback)();
	void(*_holdEndCallback)();

	void finishClick();

	void startHold();
	void endHold();
};

#include "Arduino.h"

AButt::AButt(int pin, bool inverted, bool isDigital, unsigned int debounce):
	_pin(pin),
	_inverted(inverted),
	_isDigital(isDigital),

	_isPressed(false),
	_isHeld(false),
	_wasPressed(false),

	_lastState(false),
	_lastDebounceTime(0),
	_lastPressTime(0),
	_lastClickTime(0),

	_debounceDelay(debounce),
	_clickDelay(500),
	_holdDelay(700),

	_maxClicks(5),

	_clickCallback(nullptr),
	_holdStartCallback(nullptr),
	_holdEndCallback(nullptr)
{
}

void AButt::update() {
	//Check the real button state
	bool state = getState();
	
	//Manage _isPressed var
	if (state != _lastState) {
		// reset the debouncing timer
		_lastDebounceTime = millis();
	}

	if ((millis() - _lastDebounceTime) > _debounceDelay) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:
		_isPressed = state;
	}

	// do the callback and actual logics
	if (_isPressed) {
		if (!_wasPressed) {
			_lastPressTime = millis();
		} else if (!_isHeld && millis() - _lastPressTime >= _holdDelay) {
			startHold();			
		}
	} else {
		if (_isHeld) {
			endHold();
		} else if (_wasPressed) {
			++clickCount;
			//start was pressed timer
			_lastClickTime = millis();

			if (clickCount >= _maxClicks) {
				finishClick();
			}
		}
	}

	//if not pressed and released again within the time send the callback and number of clicks
	if (millis() - _lastClickTime >= _clickDelay) {
		finishClick();
	}

	// remember last states
	_lastState = state;
	_wasPressed = _isPressed;
}

void AButt::onClick(void(*callback)(unsigned short)) {
	_clickCallback = callback;
}

void AButt::onHold(void(*start)(), void(*end)()){
	_holdStartCallback = start;
	_holdEndCallback = end;
}

bool AButt::isPressed() {
	return _isPressed;
}
bool AButt::isHeld() {
	return _isHeld;
}

bool AButt::getState() {
	bool state;
	if (_isDigital) {
		state = digitalRead(_pin);
	} else {
		state = analogRead(_pin) > 512;
	}

	if (_inverted) {
		state = !state;
	}
	
	return state;
}

void AButt::setClickDelay(unsigned int delay) {
	_clickDelay = delay;
}
int AButt::getClickDelay() {
	return _clickDelay;
}

void AButt::setHoldDelay(unsigned int delay) {
	_holdDelay = delay;
}
int AButt::getHoldDelay() {
	return _holdDelay;
}

void AButt::setMaxClicks(unsigned short clicks) {
	_maxClicks = clicks;
}
short AButt::getMaxClicks() {
	return _maxClicks;
}

void AButt::finishClick() {
	// only do something when there where actual clicks
	if (clickCount > 0) {
		if (_clickCallback) {
			(*_clickCallback)(clickCount);
		}

		clickCount = 0;
	}
}

void AButt::startHold() {
	// just in case there was a click before the hold finish up the clicks
	finishClick();

	_isHeld = true;
	if (_holdStartCallback) {
		(*_holdStartCallback)();
	}
}
void AButt::endHold() {
	_isHeld = false;
	if (_holdEndCallback) {
		(*_holdEndCallback)();
	}
}

#endif	
