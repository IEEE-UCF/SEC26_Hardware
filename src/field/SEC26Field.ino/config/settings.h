/**
 * @file settings.h
 * @brief Defines ESP32 configuration
 * @author Aldem Pido
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Keypad.h>
#include <cmath>

/**
 * ESP32 PWM safe pins: GPIO 13, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33
 * Should be able to use PCA9685 as a PWM LED Driver.
 *  16 pins
 *  com over I2C
 * Need level shifters (?) for LEDs to match 5v level for LEDs? Could also
 *  just calculate equivalent current for the circuits
 * Could grab a couple of 74HC595 for out SR, 74HC165 for in SR
 *
 */

namespace Configuration {
struct RgbSetup {
  const int r;
  const int g;
  const int b;
  constexpr RgbSetup(const int r, const int g, const int b)
      : r(r), g(g), b(b) {};
};

namespace Button {
constexpr RgbSetup LIGHT(0, 0, 0);
constexpr int BUT_IN = 0;
constexpr int IND_RED = 0;
constexpr int IND_YELLOW = 0;
constexpr int IND_GREEN = 0;
}; // namespace Button

namespace Crank {
constexpr RgbSetup LIGHT(0, 0, 0);
constexpr int CLK_PIN = 0;
constexpr int DT_PIN = 0;
constexpr int IND = 0;
}; // namespace Crank

namespace Keypad {
constexpr RgbSetup LIGHT(0, 0, 0);
constexpr int IND = 0;
byte pin_rows[ROW_NUM] = {0, 0, 0,
                          0}; // connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {
    0, 0, 0}; // connect to the column pinouts of the keypad


}; // namespace Keypad

namespace Sensor {
constexpr int FSR_IN = 0;
constexpr int IND = 0;
}; // namespace Sensor

}; // namespace Configuration

#endif