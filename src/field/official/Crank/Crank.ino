// Code for Antenna 2 for the Southeast Con 2026 Hardware Competition

/*
 * This Arduino Nano code was developed by newbiely.com
 * This Arduino Nano code is made available for public use without any restriction
 */

int CLK_PIN = 2;
int DT_PIN = 3;

int DIRECTION_CW = 0;   // clockwise direction
int DIRECTION_CCW = 1;  // anticlockwise direction

int counter = 0;

int direction = DIRECTION_CW;
int CLK_state;
int prev_CLK_state;

int red = 9;
int green = 10;
int blue = 11;
int led_pin = 13;

int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;

int randomColor;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(led_pin, OUTPUT);

  randomColor = random(1, 5);
  Serial.println(randomColor);

  prev_CLK_state = digitalRead(CLK_PIN);
}

void loop() {
  // read the current state of the rotary encoder's CLK pin
  CLK_state = digitalRead(CLK_PIN);

  // If the state of CLK is changed, then pulse occurred
  // React to only the rising edge (from LOW to HIGH) to avoid double count
  if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
    // if the DT state is HIGH
    // The encoder is rotating in anticlockwise direction => decrease the counter
    if (digitalRead(DT_PIN) == HIGH) {
      counter--;
      direction = DIRECTION_CCW;
    } else {
      // The encoder is rotating in clockwise direction => increase the counter
      counter++;
      direction = DIRECTION_CW;
    }

    Serial.print(" - counter: ");
    Serial.println(counter);
    if((counter == 18) || (counter == -18)){
      Serial.print("LED On");
      digitalWrite(led_pin, HIGH);

      if(randomColor == 1)
      {
        redBrightness = 150;
        blueBrightness = 0;
        greenBrightness = 0;
      }
      else if(randomColor == 2)
      {
        redBrightness = 0;
        blueBrightness = 150;
        greenBrightness = 0;
      }
      else if(randomColor == 3)
      {
        redBrightness = 0;
        blueBrightness = 0;
        greenBrightness = 100;
      }
      else
      {
        redBrightness = 200;
        blueBrightness = 150;
        greenBrightness = 0; 
      }

      analogWrite(red, redBrightness);
      analogWrite(green, greenBrightness);
      analogWrite(blue, blueBrightness);
      delay(3000);
    }
  }

  // save last CLK state
  prev_CLK_state = CLK_state;
}