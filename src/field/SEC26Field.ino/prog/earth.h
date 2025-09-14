/**
 * @file earth.h
 * @brief Defines the earth program
 * @author Aldem Pido
 */
#include "baseprogram.h"
#include "lcdmanager.h"
#include "../helper/rgbled.h"
#include <IRremote.hpp>
#include <vector>
#include <string>
#include <array>
#define DECODE_NEC

namespace Program {
constexpr char MASK_ATTENNA = 0xf0;
constexpr char MASK_COLOR = 0x0f;
constexpr int RESULT_ROWS = 6;
constexpr int ANT_POS_NAME = 0;
constexpr int ANT_POS_STATUS = 7;
constexpr int ANT_POS_COLOR = 13;
std::string antennaLineNames[RESULT_ROWS] = {
  "Ant_1",
  "Ant_2",
  "Ant_3",
  "Ant_4",
  "Wrong Color",
  "Connected"
};
enum AntennaCode {
  BUTTON = 0,
  CRANK,
  SENSOR,
  KEYPAD,
  NONE
};
struct EarthConfig {
  String identifier;
  uint8_t ir_k;
};
class EarthProgram : public BaseProgram {
public:
  EarthProgram::EarthProgram(EarthConfig &config, LcdManager &manager);
  void begin() override;
  void update() override;
  void reset() override;

private:
  EarthConfig &_config;
  LcdManager &_manager;
  IRrecv _ir;
  AntennaCode _lastAntenna;
  Helper::Colors::ColorType _lastColor;
  bool _new;
  void updateLCD();
  void updateIR();
};
}; // namespace Program