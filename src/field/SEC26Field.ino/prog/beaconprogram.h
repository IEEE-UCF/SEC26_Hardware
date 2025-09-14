/**
 * @file BeaconProgram.h
 * @brief Defines a program that controls a beacon.
 * @author Aldem Pido
 */
#ifndef BEACONPROGRAM_H
#define BEACONPROGRAM_H
#include "../driver/pca9685.h"
#include "../helper/rgbled.h"
#include "baseprogram.h"


namespace Program {
class BeaconProgram : public BaseProgram {
public:
  BeaconProgram(Driver::Pca9685 &driver, uint8_t activation_c,
                Configuration::RgbSetup beacon,
                String identifier = "beacon_program");

  //void begin() override;
  //void update() override;
  // void pause() override;
  void reset() override;
  void stateSwitch() override;

protected:
  Helper::Led _led_activation;
  Helper::RGBLed _led_beacon;
  const Helper::ColorProperties *_randomColor;
  const Helper::ColorProperties &getRandomColor();
  void updateLed();
};
}; // namespace Program

#endif