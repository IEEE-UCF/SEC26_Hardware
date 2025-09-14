/**
 * @file led.cpp
 * @brief Implements the LCD wrapper class.
 * @author Aldem Pido
 */
#include "lcd.h"
#include <Arduino.h>
#include <string.h>

namespace Driver {
LCD::LCD(LCDConfig &config)
    : _config(config),
      _lcd(config.rs, config.en, config.d4, config.d5, config.d6, config.d7) {
}

/**
 * Begins the LCD.
 */
void LCD::begin() { _lcd.begin(_config.cols, _config.rows); }

/**
 * Clears the LCD.
 */
void LCD::clear() { _lcd.clear(); }

/**
 * Set the LCD text.
 */
void LCD::setText(LCDText text) {
  _buffer = text;
}

/**
 * Print the LCD text.
 */
void LCD::printText() {
  _lcd.clear();
  _lcd.setCursor(0, 0);
  _lcd.print(_buffer.line1);
  _lcd.setCursor(0, 1);
  _lcd.print(_buffer.line2);
}

const LCDText LCD::getText() const {
  return _buffer;
}
} // namespace Driver