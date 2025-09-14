/**
 * @file rgbled.h
 * @brief Defines rgb led methods
 * @author Aldem Pido
 */
#ifndef RGBLED_h
#define RGBLED_h

#include "../config/settings.h"
#include "../driver/pca9685.h"
#include "led.h"
#include <Arduino.h>
#include <array>
#include <string>

namespace Helper {
using Configuration::RgbSetup;
struct RawColor {
  const uint8_t r, g, b;
  constexpr RawColor(const uint8_t _r, const uint8_t _g, const uint8_t _b)
      : r(_r), g(_g), b(_b) {}
};

struct ColorProperties {
  String name;
  RawColor color;
};

namespace Colors {
enum class ColorType {
  OFF = 0,
  RED,
  BLUE,
  GREEN,
  PURPLE,
  COUNT // A convenient way to get the number of colors
};

const ColorProperties colorData[] = {
  {"OFF",    {0, 0, 0}},
  {"RED",    {150, 0, 0}},
  {"BLUE",   {0, 0, 150}},
  {"GREEN",  {0, 100, 0}},
  {"PURPLE", {200, 0, 150}},
};

}; // namespace Colors

class RGBLed {
public:
  RGBLed(Driver::Pca9685 &driver, uint8_t cR, uint8_t cG, uint8_t cB);
  RGBLed(Driver::Pca9685 &driver, RgbSetup setup);
  bool set(ColorProperties color);
  bool setRaw(uint8_t strr, uint8_t strg, uint8_t strb);

private:
  Led _rLed;
  Led _gLed;
  Led _bLed;
};
}; // namespace Helper

#endif