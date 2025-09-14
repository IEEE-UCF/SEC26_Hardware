/**
 * @file led.h
 * @brief Defines led methods
 * @author Aldem Pido
 */

#ifndef LED_h
#define LED_h

#include "../driver/pca9685.h"

namespace Helper {
class Led {
public:
  Led(Driver::Pca9685 &driver, uint8_t c);
  bool set(uint8_t str);

private:
  Driver::Pca9685 &_driver;
  uint8_t _c;
};
}; // namespace Helper

#endif