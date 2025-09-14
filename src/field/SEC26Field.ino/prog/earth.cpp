/**
 * @file earth.cpp
 * @brief Implements the earth program
 * @author Aldem Pido
 */
#include "earth.h"
namespace Program {
EarthProgram::EarthProgram(EarthConfig &config, LcdManager &manager)
    : BaseProgram(config.identifier), _config(config), _manager(manager),
      _ir(config.ir_k), _lastAntenna(AntennaCode::NONE),
      _lastColor(Helper::Colors::ColorType::OFF), _new(false) {
  reset();
}
void EarthProgram::begin() {
  BaseProgram::begin();
  reset();
  // shouldn't be necessary to do anything else
}

void EarthProgram::reset() {
  BaseProgram::reset();
  for (int i = 0; i < RESULT_ROWS; i++) {
    std::string strAntenna =
        antennaLineNames[static_cast<int>(_lastAntenna)].substr(0, 4);
    std::string strStatus = "OFF";
    std::string strColor =
        std::string(Helper::Colors::colorData[static_cast<int>(Helper::Colors::ColorType::OFF)]
                        .name.c_str())
            .substr(0, 3);
    std::string out(16, ' '); // initialize 16 long with space
    out.replace(0, strAntenna.length(), strAntenna);
    out.replace(7, strStatus.length(), strStatus);
    out.replace(13, strColor.length(), strColor);
    std::string key = "an_" + strAntenna;
    _manager.addLine(key, out);
  }
}

void EarthProgram::update() {
  switch (_state) {
  case 0:
  case 3:
    break;
  case 1:
  case 2:
    updateIR();
    updateLCD();
    break;
  }
}

void EarthProgram::updateLCD() {
  if (_new) {
    _new = false;
    std::string strAntenna =
        antennaLineNames[static_cast<int>(_lastAntenna)].substr(0, 4);
    std::string strStatus = "ON";
    std::string strColor =
        std::string(Helper::Colors::colorData[static_cast<int>(_lastColor)]
                        .name.c_str())
            .substr(0, 3);
    std::string out(16, ' '); // initialize 16 long with space
    out.replace(0, strAntenna.length(), strAntenna);
    out.replace(7, strStatus.length(), strStatus);
    out.replace(13, strColor.length(), strColor);
    std::string key = "an_" + strAntenna;
    _manager.addLine(key, out);
  }
}

void EarthProgram::updateIR() {
  using namespace Helper::Colors;
  if (_ir.decode()) {
    const IRData data = _ir.decodedIRData;
    if (data.protocol == NEC) {
      char rec_cmd = data.command;
      char attenna_rec = rec_cmd & MASK_ATTENNA;
      char color_rec = rec_cmd & MASK_COLOR;
      switch (attenna_rec) {
      case 0x00:
        _lastAntenna = AntennaCode::BUTTON; // <> todo: set correct
        break;
      case 0x30:
        _lastAntenna = AntennaCode::CRANK; // <>
        break;
      case 0x50:
        _lastAntenna = AntennaCode::KEYPAD; // <>
        break;
      case 0x60:
        _lastAntenna = AntennaCode::SENSOR; // <>
        break;
      default:
        _lastAntenna = AntennaCode::NONE; // <>
        return; // exits, don't set color or change _new
      }
      switch (color_rec) {
      case 0x09:
        _lastColor = ColorType::RED;
        break;
      case 0x0A:
        _lastColor = ColorType::GREEN;
        break;
      case 0x0C:
        _lastColor = ColorType::BLUE;
        break;
      case 0x0F:
        _lastColor = ColorType::PURPLE;
        break;
      default:
        _lastColor = ColorType::OFF;
      }
    }
    _new = true;
    return;
  } else {
    _new = false;
    return;
  }
}

} // namespace Program