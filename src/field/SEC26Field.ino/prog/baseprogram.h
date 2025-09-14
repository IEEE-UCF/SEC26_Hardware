/**
 * @file baseprogram.h
 * @brief defines shared functions for each program
 * @author Aldem Pido
 */
#ifndef BASEPROGRAM_H
#define BASEPROGRAM_H

#include <Arduino.h>

namespace Program {
class BaseProgram {
public:
  BaseProgram(String identifier = "base_program");
  virtual ~BaseProgram() = default;
  virtual void begin();
  virtual void update();
  virtual void pause();
  virtual void reset();
  virtual void resume();
  virtual void stateSwitch();
  virtual void displayInfo(Print &output) const;

protected:
  String _identifier; // program name
  /**
   * 0: reset
   * 1: running, not activated
   * 2: running, activated
   * 3: paused
   * */
  int _state;
  int _prevState;
  bool stateChanged();
};
}; // namespace Program

#endif