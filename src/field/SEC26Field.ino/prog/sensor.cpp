/**
 * @file sensor.cpp
 * @brief Defines the sensor beacon program.
 * @author Aldem Pido
 */
#include "sensor.h"
namespace Program {
SensorProgram::SensorProgram(Driver::Pca9685 &driver, SensorConfig &config)
    : BeaconProgram(driver, config.activation_c, config.beacon,
                    config.identifier),
      _config(config), _reading(0), _prevState(0) {
  reset();
}

void SensorProgram::begin() {
  reset();
  _state = 4;
}

void SensorProgram::update() {
  switch(_state) {
    case 1:
    case 2:
      _reading = analogRead(_config.sensor_c);
      if(_reading <= 30) { // duck removed
        _state = 2;
      } else { // duck not removed
        _state = 1;
      }
    break;
    case 0:
    case 3:
    default:
    break;
    case 4: // special case between 0 and 1, checks if sensor is setup
     _reading = analogRead(_config.sensor_c);
      if(_reading <= 50) { // duck not on starting position
        _led_beacon.setRaw(255,255,255);
      } else {
        _state = 1;
      }
    break;
  }
  BeaconProgram::stateSwitch(); // updates led when state switch
}

void SensorProgram::reset() {
 BeaconProgram::reset();
}
} // namespace Program