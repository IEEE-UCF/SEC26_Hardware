/**
 * @file button.cpp
 * @brief Implements the button beacon program
 * @author Aldem Pido
 */
#include "button.h"
namespace Program {
ButtonProgram::ButtonProgram(Driver::Pca9685 &driver, BeaconConfig &config)
    : BeaconProgram(driver, config.activation_c, config.beacon, config.identifier), _config(config),
      _redButton(config.kBut), _r(driver, config.cR), _y(driver, config.cY),
      _g(driver, config.cG), _count(0) {}

void ButtonProgram::begin() {
  reset();
  _state = 1;
}

void ButtonProgram::update() {
  BeaconProgram::update();
  _redButton.loop(); // update the button
  int newCount =  _redButton.getCount();
  switch (_state) {
  case 1:
  case 2:
    if (newCount >= 3) {
      _state = 2;
    } else {
      _state = 1;
    }
    break;
  case 0:
  case 3:
  default:
    break;
  }
  if (_count != newCount || BaseProgram::stateChanged()) { // updates led on count or state switch
    _count = newCount;
    BeaconProgram::updateLed();
    ButtonProgram::updateTrafficLight();
  }

}

/**
 * Updates the button's traffic light.
 * @param count button count.
 */
void ButtonProgram::updateTrafficLight() {
  switch (_count) {
  case 0:
  default:
    _r.set(0);
    _y.set(0);
    _g.set(0);
    break;
  case 1:
    _r.set(255);
    _y.set(0);
    _g.set(0);
    break;
  case 2:
    _r.set(0);
    _y.set(255);
    _g.set(0);
    break;
  case 3:
    _r.set(0);
    _y.set(0);
    _g.set(255);
    break;
  }
}

void ButtonProgram::reset() {
  BeaconProgram::reset();
  _redButton.setCountMode(COUNT_RISING);
  _redButton.resetCount();
  _count = 0;
  updateTrafficLight(); // should pass 0 always
}
} // namespace Program