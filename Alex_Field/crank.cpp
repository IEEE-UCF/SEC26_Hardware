#ifndef CRANK_CPP
#define CRANK_CPP

#include "..\include\crank.h"

// Implements crank program
CrankP::CrankP(CrankConfig &config)
    : _config(config), _counter(0), _prev_Clk(0), _clk_State(0) {
  pinMode(_config.clk_k, INPUT); // CLK_PIN
  pinMode(_config.dt_k, INPUT);  // DT_PIN
  pinMode(_config.led, OUTPUT);  // LED PIN
  task = NOTCOMPLETE;
};

void CrankP::begin() {
  Serial.begin(9600);
  reset();
  _prev_Clk = digitalRead(_config.clk_k);
}

void CrankP::update() {
  _clk_State = digitalRead(_config.clk_k);

  if (_clk_State != _prev_Clk &&
      _clk_State == HIGH) { // counter increments on rising edge
    if (digitalRead(_config.dt_k) == HIGH) { // dt for direction
      _counter--;
    } else {
      _counter++;
      digitalWrite(2, HIGH);
    }

    Serial.print(" - counter: ");
    Serial.println(_counter);
    if ((_counter == 18) || (_counter == -18)) {
      digitalWrite(_config.led, HIGH);
      task = COMPLETE;
    }
  }
  _prev_Clk = _clk_State; // save last clock state
}

bool CrankP::getStatus() {
  if (task == COMPLETE) {
    return 1;
  } else {
    return 0;
  }
}

void CrankP::reset() {
  _prev_Clk = digitalRead(_config.clk_k);
  _counter = 0;
  task = NOTCOMPLETE;
}

#endif