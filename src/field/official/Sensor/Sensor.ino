// Code for Antenna 3 of the Southeast Con 2026 Hardware Compeition

int red = 9;
int green = 10;
int blue = 11;
int led_pin = 13;

int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;
int randomColor;

int begin = 1;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  randomColor = random(1, 5);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  int analogReading = analogRead(A7);
  if (begin == 1){
    begin = 0;
    while(analogReading <= 50){
      analogReading = analogRead(A7);
      redBrightness = 150;
      blueBrightness = 0;
      greenBrightness = 0;

      analogWrite(red, redBrightness);
      analogWrite(green, greenBrightness);
      analogWrite(blue, blueBrightness);
      Serial.println("Error State");

      delay(250);
      analogWrite(red, 0);
      delay(250);

    }
    analogWrite(red, 0);
    analogWrite(green, 100);
    delay(2000);
    analogWrite(green, 0);


  }

  Serial.print("Force sensor reading = ");
  Serial.println(analogReading); // print the raw analog reading

  if(analogReading <= 30){
    delay(500);
    if(analogReading <= 30){
      Serial.println("Removed");

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

  delay(500);
}
