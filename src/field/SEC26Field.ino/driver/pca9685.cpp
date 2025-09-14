/**
 * @file pca9685.cpp
 * @brief Implements PCA9685 routines
 * @author Aldem Pido
 */
#include "pca9685.h"

namespace Driver {

Pca9685::Pca9685(uint8_t addr) : _pwm(addr) {}

/**
 * Begins the pca9685.
 */
bool Pca9685::begin() {
  Wire.begin();
  if(!_pwm.begin()) {
    return false;
  }
  _pwm.setPWMFreq(1000);
  return true;
}

/**
 * Sets the PWM a channel.
 * @param channel channel 0-15.
 * @param value pwm value 0-255.
 */
bool Pca9685::setPwm(uint8_t channel, uint8_t value) {
  if (channel < 16 && channel >= 0) {
    uint16_t pwmValue = map(constrain(value,0,255), 0, 255, 0, 4095);
    return _pwm.setPWM(channel, 0, pwmValue);
  }
}

/**
 * Sets the PWM frequency up to 1.6khz.
 * @param freq frequency
 */
void Pca9685::setPWMFreq(float freq) { _pwm.setPWMFreq(freq); }

} // namespace Driver