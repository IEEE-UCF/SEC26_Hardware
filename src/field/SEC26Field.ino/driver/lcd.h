/**
 * @file lcd.h
 * @brief Defines the lcd wrapper class for LiquidCrystal.h
 * @author Aldem Pido
 */
#include <LiquidCrystal.h>
namespace Driver {
struct LCDText {
  String line1, line2;
};
struct LCDConfig {
  uint8_t rs, en, d4, d5, d6, d7;
  uint8_t rows, cols;
};
struct LCDAdvanced {};
class LCD {
public:
  LCD(LCDConfig &config);
  void begin();
  void clear();
  void setText(LCDText text);
  void printText();
  const LCDText getText() const;

private : 
  LCDConfig &_config;
  LiquidCrystal _lcd;
  LCDText _buffer;
};
}; // namespace Driver