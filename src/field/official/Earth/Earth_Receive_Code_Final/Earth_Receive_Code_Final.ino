/*
This code is for testing purposes.  It is not the actual competition code.  
This code will count any color not Red, Green, Blue or Purple as an error.
The competition code will only count valid colors that are incorrect as errors.
For example the correct color for antenna 1 is Red, but Blue is received for 
antenna 1 will be counted as an error.  See the rules for further explaination.
*/
#include <IRremote.hpp>
#include <LiquidCrystal.h>
#define DECODE_NEC

/* for lcd connections*/
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/* end of lcd connections */

/* start of LCD display control information */
// antenna result matrix for display of received
// transmission information
// column 1: antenna name, column 2 status, column 3 color
// antenna status: ON or OFF
// antenna color: Red, Blu, Pur, Gre
const int ROWS = 6;
String result[ROWS][3] = { { "Ant_1", "OFF", "??" },
                        { "Ant_2", "OFF", "??" },
                        { "Ant_3", "OFF", "??" },
                        { "Ant_4", "OFF", "??" },
                        { "Wrong Color", "---", "" },
                        { "Connected","OFF",""} };

int row;                         // row of array to be printed out
int antSent = -1;                // received antenna id  -1 is invalid antenna designation
unsigned long previousTime = 0;  // last time LCD display was updated
unsigned long currentTime;       // current time value used to determine if timeLimit reached for LCD update
const long timeLimit = 1500;     // how long to wait before updating LCD display
int antName = 0;                 // column index for the antenna name
int antStatus = 1;               // column index for antenna status
int antColor = 2;                // column index for antenna status
int antNamePos = 0;              // antenna name start position in lcd display
int antStatusPos = 7;            // antenna status start position in lcd display
int antColorPos = 13;            // color start position in lcd display
int wrongPos = 13;               // LCD start position for wrong color and rcvd transmission status
int wrongColor = ROWS-2;              // array row index for wrong count
int rcvdComm = ROWS-1;       // array row index for transmission received 
const int MAX_WRONG = 8;         // cap on number of wrong colors guessed
int wrong = 0;                   // count of wrong colors guessed
/* end of LCD display control information */

int baudRate = 9600;

// IR receiver information
int IRpin = 10;
IRrecv IR(IRpin);
char antenna_mask = 0xf0;  //antenna is first nibble of command
char color_mask = 0x0f;    //color is second nibble
char rec_cmd;              // command received via IR for processing
char antenna_rec;          // decoded received antenna number from rec_cmd
char color_rec;            // decoded received color of antenna from rec_cmd

void setup() {
  // start earth
  lcd.begin(16, 2);
  row = 0;              // row in the result array to display on the LCD
  PrintResult(0, row);  // print to first row of LCD
  row++;
  PrintResult(1, row);  // print to second row of LCD

  // end earth
  Serial.begin(baudRate);
  IrReceiver.begin(IRpin, ENABLE_LED_FEEDBACK);
}

void loop() {
  currentTime = millis();  // get current time for seeing if LCD needs updating
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == NEC) {  // make sure valid communication is tested
      rec_cmd = IrReceiver.decodedIRData.command;    //pulls out command
      antenna_rec = rec_cmd & antenna_mask;          //extract antenna code from received signal
      color_rec = rec_cmd & color_mask;              //extract color code from received signal

      /* ================================================ */
      /* code below is for informational purposes on received signal 

      IrReceiver.printIRResultShort(&Serial);

      char rec_add = IrReceiver.decodedIRData.address;
      Serial.print("Received Address: ");
      Serial.println(rec_add, HEX);

      Serial.print("Received Command: ");
      Serial.println(rec_cmd, HEX);
      
      Serial.print("Antenna code: ");
      Serial.println(antenna_rec, HEX);
      
      Serial.print("Color code: ");
      Serial.println(color_rec, HEX);
     // */

      if (antenna_rec == 0x00) {
        Serial.println("Antenna 1");
        antSent = 0;

      } else if (antenna_rec == 0x30) {
        Serial.println("Antenna 2");
        antSent = 1;

      } else if (antenna_rec == 0x50) {
        Serial.println("Antenna 3");
        antSent = 2;

      } else if (antenna_rec == 0x60) {
        Serial.println("Antenna 4");
        antSent = 3;

      } else {
        Serial.println("Invalid Antenna");
        result[rcvdComm][antStatus] = "ON";  // for rcvd transmission with wrong antenna
        antSent = -1;   // set invalid antenna code
      }

      if (antSent != -1) {
        result[antSent][antStatus] = "ON";
        result[rcvdComm][antStatus] = "ON";  // have received a transmission with correct antenna
        if (color_rec == 0x09) {
          Serial.println("Color: Red");
          result[antSent][antColor] = "RED";

        } else if (color_rec == 0x0A) {
          Serial.println("Color: Green");
          result[antSent][antColor] = "GRE";

        } else if (color_rec == 0x0C) {
          Serial.println("Color: Blue");
          result[antSent][antColor] = "BLU";

        } else if (color_rec == 0x0F) {
          Serial.println("Color: Purple");
          result[antSent][antColor] = "PUR";

        } else {
          Serial.println("Invalid Color");
          if (wrong < MAX_WRONG) {
            wrong++;
            result[wrongColor][antStatus] = String(wrong);
          }
        }
      }
    }
    IrReceiver.resume();  //receive next transmission
  }

  if (currentTime - previousTime >= timeLimit) {
    previousTime = currentTime;   // reset start point for delay of LCD update
    lcd.clear();
    // print current line in array in first row of lcd
    PrintResult(0, row);
    row++;      // go to next row in array
    row = row % ROWS; // make sure only row index values of 0-4 are used
    // print next line in array in second row of lcd
    PrintResult(1, row);
  }
}
void PrintResult(int lcdRow, int arrayRow) {
  // lcdRow indicates which row of the lcd is to be printed
  // lcdRow will be 0 or 1
  // arrayRow indicates which row of the result array to print out on the
  // lcdRow provided
  lcd.setCursor(antNamePos, lcdRow);
  lcd.print(result[arrayRow][antName]);
  if (arrayRow == ROWS-2)
  { // print out wrong color count
    // use antColorPos to get a 
    lcd.setCursor(wrongPos, lcdRow);
    lcd.print(result[arrayRow][antStatus]);
  }
  else if (arrayRow == ROWS -1)
  {
    lcd.setCursor(wrongPos, lcdRow);
    lcd.print(result[arrayRow][antStatus]);
  }
  else
  {
  lcd.setCursor(antStatusPos, lcdRow);
  lcd.print(result[arrayRow][antStatus]);
  
  lcd.setCursor(antColorPos, lcdRow);
  lcd.print(result[arrayRow][antColor]);
  }
}
