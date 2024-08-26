#include <Wire.h>

int x = 0;
int y = 0;
bool buttonState = false;

void setup() {
  Wire.begin();
  Serial.begin(9600); 
  randomSeed(analogRead(0));  
}

void loop() {
  x = random(-1024, 1025);  
  y = random(-1024, 1025);
  buttonState = random(0, 2); 
  String xString = formatJoystickValue(x);
  String yString = formatJoystickValue(y);
  String joystickData = "LJ " + xString + "," + yString;
  String buttonData = "PB " + String(buttonState);
  sendI2CData(joystickData);
  delay(500); 
  sendI2CData(buttonData);
  delay(500); 
}

String formatJoystickValue(int value) {
  String result;

  if (value < 0) {
    result = "-" + String(abs(value), DEC);
  } else {
    result = "+" + String(value, DEC);
  }

  while (result.length() < 5) {
    result = "0" + result;   }

  return result;
}

void sendI2CData(String data) {
  Wire.beginTransmission(9);   
  Wire.write(data.c_str());
  Wire.endTransmission(); 

  // Debugging output
  Serial.print("Sent data: ");
  Serial.println(data);
}