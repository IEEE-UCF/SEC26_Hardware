/**
 * @file pca9685.h
 * @brief Defines PCA9685 routines
 * The PCA9685 is a 16-Channel 12-bit resolution driver. It is being used to drive LEDs.
 * @author Aldem Pido
 */
#ifndef PCA9685_h
#define PCA9685_h

#include <Adafruit_PWMServoDriver.h> // PCA9685 library
#include <Arduino.h>
#include <Wire.h> // Required for I2C communication

namespace Driver {
class Pca9685 {
public:
  Pca9685(uint8_t addr = 0x40);
  bool begin();
  bool setPwm(uint8_t channel, uint8_t value);
  void setPWMFreq(float freq = 1000);

private:
  Adafruit_PWMServoDriver _pwm;
};
}; // namespace Driver
#endif
