#ifndef BUTTON_CPP
#define BUTTON_CPP
#include "..\include\button.h"

// implements button program

Button::Button(ButtonConfig &config)
    : _config(config), _counter(0), _r(26), _y(25), _g(33) {
  pinMode(_config.led, OUTPUT); // LED PIN
  task = NOTCOMPLETE;
};

void Button::begin() { reset(); }

void Button::update() {
  ezButton button(_config.SW_Pin);
  button.loop();
  if (button.isPressed()) {
    _counter++;

    switch (_counter) {
    case 1:
      digitalWrite(_r, HIGH);
      break;
    case 2:
      digitalWrite(_y, HIGH);
      break;
    case 3:
      digitalWrite(_g, HIGH);
      task = COMPLETE;
      break;
    default:
      break;
    }
  }
}

bool Button::getStatus() {
  if (task == COMPLETE) {
    return 1;
  } else {
    return 0;
  }
}

void Button::reset() {
  _counter = 0;
  task = NOTCOMPLETE;
}

#endif
