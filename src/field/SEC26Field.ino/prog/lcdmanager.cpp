/**
 * @file lcdmanager.cpp
 * @brief Implements the lcd manager.
 * @author Aldem Pido
 */
#include "lcdmanager.h"
namespace Program {
LcdManager::LcdManager(Driver::LCD &driver, LcdManagerConfig &config)
    : BaseProgram(config.identifier), _driver(driver), _config(config), _lastUpdate(0) {}

void LcdManager::update() {
  if (_state == 1) {
    unsigned long now = millis();
    if (now - _lastUpdate >= _config.LCDUpdateInterval) {
      drawScreen();
      advanceIterator();
      _lastUpdate = now;
    }
  }
}

void LcdManager::addLine(std::string key, std::string text) {
  _lines.insert({key, text});
}

void LcdManager::removeLine(std::string key) { _lines.erase(key); }

void LcdManager::clear() { _lines.clear(); }

void LcdManager::drawScreen() {
  Driver::LCDText out;
  _driver.clear(); // Clear screen first
  if (_lines.empty()) {
    out.line1 = "No data";
    out.line2 = "";
    return;
  }
  // Display current line
  out.line1 = _iteration->second.c_str();

  // Display next line
  std::map<std::string, std::string>::iterator lastIt = _iteration;
  advanceIterator();
  out.line2 = _iteration->second.c_str();
  _iteration = lastIt; // restore to original state before advancing
}

void LcdManager::advanceIterator() {
  if (_lines.empty()) {
    return;
  }
  _iteration++;
  if (_iteration == _lines.end()) {
    _iteration = _lines.begin();
  }
}
} // namespace Driver