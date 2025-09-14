/**
 * @file keypad.h
 * @brief Defines the keypad beacon program.
 * @author Aldem Pido
 */
#include "beaconprogram.h"
#include <Keypad.h>
namespace Program {
constexpr char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'*', '0', '#'}};
const String password = "73738";
constexpr int ROW_NUM = 4;    // four rows
constexpr int COLUMN_NUM = 3; // three columns

struct KeypadConfig {
  uint8_t activation_c;
  Configuration::RgbSetup beacon;
  String identifier;
  byte pin_rows[ROW_NUM];
  byte pin_columns[COLUMN_NUM];
};

class KeypadProgram : public BeaconProgram {
public:
  KeypadProgram(Driver::Pca9685 &driver, KeypadConfig &config);
  void begin() override;
  void update() override;
  void reset() override;
private:
  KeypadConfig &_config;

  Keypad _keypad;
  String _input_password;
};
}; // namespace Program