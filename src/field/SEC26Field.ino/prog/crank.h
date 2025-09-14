/**
 * @file crank.h
 * @brief Defines the crank beacon program.
 * @author Aldem Pido
 */
#include "beaconprogram.h"
namespace Program {
struct CrankConfig {
  uint8_t activation_c, clk_k, dt_k;
  Configuration::RgbSetup beacon;
  String identifier;

};
class CrankProgram : public BeaconProgram {
public:
  CrankProgram(Driver::Pca9685 &driver, CrankConfig &config);
  void begin() override;
  void update() override;
  void reset() override;
private:
  CrankConfig &_config;
  int _counter;
  int _prev_CLK_state;
};
};