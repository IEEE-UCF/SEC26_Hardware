/**
 * @file BeaconProgram.cpp
 * @brief Implements a program that controls a beacon.
 * @author Aldem Pido
 */
#include "beaconprogram.h"
namespace Program {
  BeaconProgram::BeaconProgram(Driver::Pca9685 &driver, uint8_t activation_c, Configuration::RgbSetup beacon, String identifier) : BaseProgram(identifier), _led_activation(driver, activation_c), _led_beacon(driver, beacon) {
    reset();
  }
using namespace Helper::Colors;
using  Helper::Colors::ColorType;
/**
 * Returns a random color
 * @return Random ColorProperties
 */
const Helper::ColorProperties &BeaconProgram::getRandomColor() {
  int index = random(1,static_cast<int>(Helper::Colors::ColorType::COUNT));
  return colorData[index];
}

/**
 * Updates generic beacon leds
 */
void BeaconProgram::updateLed() {
  switch(_state) {
    case 0:
    case 1:
      _led_activation.set(0);
      _led_beacon.set(colorData[static_cast<int>(Helper::Colors::ColorType::OFF)]);
      break;
    case 2:
      _led_activation.set(0);
      _led_beacon.set(*_randomColor);
      break;
    case 3:
    default:
      break;
  }
}

void BeaconProgram::reset() {
  BaseProgram::reset();
  _randomColor = &getRandomColor();
  updateLed();
}

/**
 * Function runs every time state change is detected.
 */
void BeaconProgram::stateSwitch() {
  if(BaseProgram::stateChanged()) {
      updateLed();
  }
}
}