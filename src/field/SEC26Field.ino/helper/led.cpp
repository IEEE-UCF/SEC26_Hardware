/**
 * @file rgbled.cpp
 * @brief Defines rgb led methods
 * @author Aldem Pido
 */

#include "led.h"

namespace Helper {
/**
 * Instantiates a LED object.
 * @param driver Pca9685 object.
 * @param c Channel led is on.
 */
Led::Led(Driver::Pca9685 &driver, uint8_t c) : _driver(driver), _c(c) {}

/**
 * Set power of LED.
 * @param str strength 0-255
 */
bool Led::set(uint8_t str) { return _driver.setPwm(_c, str); }
} // namespace Helper