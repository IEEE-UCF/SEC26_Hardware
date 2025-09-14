/**
 * @file lcdmanager.h
 * @brief Defines a lcd manager class
 * Defines a lcd manager class. Is necessary for smooth display control
 * @author Aldem Pido
 */
#include "../driver/lcd.h"
#include "baseprogram.h"
#include <vector>
#include <string>
#include <map>
#include <Arduino.h>
namespace Program {
struct LcdManagerConfig {
  String identifier;
  unsigned long LCDUpdateInterval;
};
class LcdManager : public BaseProgram {
public:
  LcdManager(Driver::LCD &driver, LcdManagerConfig &config);
  void update() override;
  void addLine(std::string key, std::string text);
  void removeLine(std::string key);
  void clear();
  const std::map<std::string, std::string> getLines() const;

private:
  Driver::LCD &_driver;
  LcdManagerConfig &_config;
  unsigned long _lastUpdate;
  /**
   * 0 - waiting for start
   * 1 - active
   * 2 - paused
   */
  std::map<std::string, std::string> _lines;
  std::map<std::string, std::string>::iterator _iteration;
  void drawScreen();
  void advanceIterator();
};
};