#ifndef CRANK_H
#define CRANK_H

#include <Arduino.h>

// defines crank program
struct CrankConfig {
  uint8_t led, clk_k, dt_k;
};

class CrankP {
private:
  CrankConfig &_config;
  int _counter;
  int _prev_Clk, _clk_State;

public:
  CrankP(CrankConfig &config);
  void begin();
  void update();
  bool getStatus();
  void reset();

  enum TASK { COMPLETE, NOTCOMPLETE } task;
};

#endif