/**
 * @file crank.h
 * @brief Implements the crank beacon program.
 * @author Aldem Pido
 */
#include "crank.h"
namespace Program {
CrankProgram::CrankProgram(Driver::Pca9685 &driver, CrankConfig &config) : BeaconProgram(driver, config.activation_c, config.beacon, config.identifier), _config(config), _counter(0), _prev_CLK_state(0) {}

void CrankProgram::begin() {
  reset();
  _state = 1;
}

void CrankProgram::update() {
  switch(_state) {
    case 1:
    int CLK_state = digitalRead(_config.clk_k);
    if(CLK_state != _prev_CLK_state && CLK_state == HIGH) { // increment counter on rising edge of clk
      if(digitalRead(_config.dt_k) == HIGH) { // dt controls direction
        _counter--;
      } else {
        _counter++;
      }
    }
    break;
    case 0:
    case 2:
    case 3:
    default:
    break;
  }
  BeaconProgram::stateSwitch(); // handles leds when states switch
}

void CrankProgram::reset() {
  BeaconProgram::reset();
  _prev_CLK_state = digitalRead(_config.clk_k);
  _counter = 0;
} 
}