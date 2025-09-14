/**
 * @file baseprogram.h
 * @brief methods for each program
 * @author Aldem Pido
 */
#include "baseprogram.h"
namespace Program {
/**
 * Constructor.
 */
BaseProgram::BaseProgram(String identifier)
    : _identifier(identifier), _state(0), _prevState(0) {}

/**
 * Begins the program. Calls the reset function, then sets the state to "1"
 * (running, not activated).
 */
void BaseProgram::begin() {
  BaseProgram::reset();
  _state = 1;
}

/**
 * Updates the program. Must be overridden.
 */
void BaseProgram::update() { return; }

/**
 * Pauses the program. Sets the state to "3" (paused).
 */
void BaseProgram::pause() { _state = 3; }

/**
 * Resets the program. Sets the state to "0" (reset).
 */
void BaseProgram::reset() { _state = 0; }

/**
 * Resumes the program. Sets the state to "1" (running).
 */
void BaseProgram::resume() { if(_state == 3) {_state = 1;}}

/**
 * Displays info of state.
 * @param output Serial pointer
 */
void BaseProgram::displayInfo(Print &output) const {
  output.print("Program: ");
  output.print(_identifier);
  output.print("State: ");
  output.print(_state);
}

/**
 * See if state has changed.
 * @return bool
 */
bool BaseProgram::stateChanged() {
  if(_state != _prevState) {
    _prevState = _state;
    return true;
  } else {
    return false;
  }
}
} // namespace Program