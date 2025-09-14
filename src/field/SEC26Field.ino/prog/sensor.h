/**
 * @file sensor.h
 * @brief Defines the sensor beacon program.
 * @author Aldem Pido
 */
#ifndef SENSOR_PROGRAM_H
#define SENSOR_PROGRAM_H
#include "beaconprogram.h"
namespace Program {
struct SensorConfig {
  uint8_t activation_c, sensor_c;
  Configuration::RgbSetup beacon;
  String identifier;
};
class SensorProgram : public BeaconProgram {
public:
  SensorProgram(Driver::Pca9685 &driver, SensorConfig &config);
  void begin() override;
  void update() override;
  void reset() override;
private:
  SensorConfig &_config;
  uint8_t _reading;
  int _prevState;
};
};
#endif