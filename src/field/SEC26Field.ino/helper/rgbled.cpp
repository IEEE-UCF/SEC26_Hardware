/**
 * @file rgbled.cpp
 * @brief Implements rgb led methods
 * @author Aldem Pido
 */
#include "rgbled.h" // Include the corresponding header file
namespace Helper {

/**
 * Instantiates an RGB Led.
 * @param driver Pca9685 driver object.
 * @param cR Channel for Red.
 * @param cG Channel for Green.
 * @param cB Channel for Blue.
 */
RGBLed::RGBLed(Driver::Pca9685 &driver, uint8_t cR, uint8_t cG, uint8_t cB)
    : _rLed(driver, cR), _gLed(driver, cG), _bLed(driver, cB) {
}

/**
 * Instantiates an RGB Led.
 * @param driver Pca9685 driver object.
 * @param setup RgbSetup configuration object.
 */
RGBLed::RGBLed(Driver::Pca9685 &driver, RgbSetup setup)
    : _rLed(driver, setup.r), _gLed(driver, setup.g), _bLed(driver, setup.b) {
}

/**
 * Set rgb with a Color object.
 * @param Color color object.
 */
bool RGBLed::set(ColorProperties color) { return setRaw(color.color.r, color.color.g, color.color.b); }

/**
 * Set rgb with raw values.
 * @param strr red strength.
 * @param strg green strength.
 * @param strb blue strength.
 */
bool RGBLed::setRaw(uint8_t strr, uint8_t strg, uint8_t strb) {
  return _rLed.set(strr) && _gLed.set(strg) && _bLed.set(strb);
}

} // namespace Helper