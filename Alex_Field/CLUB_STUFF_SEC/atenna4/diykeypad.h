
#ifndef DIYables_Keypad_h
#define DIYables_Keypad_h

#include <Arduino.h>
// just a pointer to whatever variable x maybe.....
#define makeKeymap(x) ((char *)x)
#define sizepassword 6

class DIYables_Keypad {
private:
  byte _num_rows;
  byte _num_cols;
  char *_keymap;
  //example r1,r2,r3,r4-->23,22,21,19
  //Pinout for keypad-->2,7,6,4
  byte *_row_pins;
  //example c1,c2,c3-->25,33,18
  //Pinout for keypad-->3,1,5
  byte *_column_pins;
  char _prev_key;
  unsigned long _debounce_time;
  unsigned long _prev_time;
  const String actualPassword = "73738#";
  String password;

public:
  DIYables_Keypad(char *keymap, byte *rowPins, byte *columnPins, byte numRows,
                  byte numCols);

    enum TASK
  {
    COMPLETE,
    NOTCOMPLETE
  } task;
  char getKey();
  void setDebounceTime(unsigned long time);
  void update();
  int getStatus();
};

#endif
