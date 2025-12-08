
#include <DIYables_Keypad.h>

DIYables_Keypad::DIYables_Keypad(char *keymap, byte *rowPins, byte *columnPins,
                                 byte numRows, byte numCols) {
  _keymap = keymap;
  _row_pins = rowPins;
  _column_pins = columnPins;
  _num_rows = numRows;
  _num_cols = numCols;
  _prev_key = 0;
  _debounce_time = 300; // default value is 300ms
  _prev_time = 0;
  task = NOTCOMPLETE;
  for (int col = 0; col < _num_cols; col++) {
    pinMode(_column_pins[col], OUTPUT);
    digitalWrite(_column_pins[col], HIGH);
    for (int row = 0; row < _num_rows; row++) {
      pinMode(_row_pins[row], INPUT_PULLUP);
    }
  }
}

char DIYables_Keypad::getKey() {
  char key = _prev_key;
  _prev_key = 0;
  return key;
}
void DIYables_Keypad::setDebounceTime(unsigned long time) {
  _debounce_time = time;
}
void DIYables_Keypad::update() {
  static int count = 0;
  //------------------------------------------------------
  for (int col = 0; col < _num_cols; col++) {
    digitalWrite(_column_pins[col], LOW);
    for (int row = 0; row < _num_rows; row++) {
      if (digitalRead(_row_pins[row]) == LOW) {
        char key = _keymap[row * _num_cols + col];

        if ((_prev_key != key) ||
            (_prev_key == key && (millis() - _prev_time) > _debounce_time)) {
          _prev_key = key;
          _prev_time = millis();
          count++;
          password+=key;
          Serial.println(key);
          digitalWrite(_column_pins[col], HIGH);
          // return key;
        }
      }
    }
    digitalWrite(_column_pins[col], HIGH);
  }
  //----------------------------------------------------
  if (count == sizepassword) {
      if (password == actualPassword) {
    Serial.println("HOORAY!");
    task=COMPLETE;
  } else {
    password = "";
    Serial.println("NOPE!");
    task=NOTCOMPLETE;
    count=0;
  }
  }
}

int DIYables_Keypad::getStatus() {
  if (task == COMPLETE) {
    return 1;
  } else {
    return 0;
  }
}
