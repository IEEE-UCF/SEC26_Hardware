#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <ezButton.h>

struct ButtonConfig {
  uint8_t led;
  uint8_t SW_Pin;
};

// defines button program
class Button {
private:
  int _counter;
  uint8_t _r;
  uint8_t _y;
  uint8_t _g;
  ButtonConfig &_config;

public:
  Button(ButtonConfig &_config);
  void begin();
  void update();
  bool getStatus();
  void reset();

  enum TASK { COMPLETE, NOTCOMPLETE } task;
};

#endif
