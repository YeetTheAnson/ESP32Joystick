#include <Wire.h>

int joystickX = 0;
int joystickY = 0;
bool selectButtonPressed = false;

void setup() {
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  delay(1000);
  Serial.print("test begun");
}

void receiveEvent(int bytes) {
  String data = "";
  while (Wire.available()) {
    data += (char)Wire.read();
  }
  
  Serial.print("Received data: ");
  Serial.println(data);

  if (data.startsWith("LJ")) {
    parseJoystickData(data);
  } else if (data.startsWith("PB")) {
    parseButtonData(data);
  }
}

void parseJoystickData(String data) {
  int xPos = 0;
  int yPos = 0;

  int commaIndex = data.indexOf(',');
  if (commaIndex != -1) {
    String xString = data.substring(3, commaIndex);
    String yString = data.substring(commaIndex + 1);
    xPos = xString.toInt();
    yPos = yString.toInt();
    joystickX = xPos;
    joystickY = yPos;
    Serial.print("Joystick X: ");
    Serial.print(joystickX);
    Serial.print(", Y: ");
    Serial.println(joystickY);
  }
}

void parseButtonData(String data) {
  int state = data.substring(3).toInt();

  if (state == 0) {
    selectButtonPressed = false;
    Serial.println("Select button released.");
  } else if (state == 1) {
    selectButtonPressed = true;
    Serial.println("Select button pressed.");
  }
}

void loop() {
  Serial.print("Joystick X: ");
  Serial.print(joystickX);
  Serial.print(", Y: ");
  Serial.println(joystickY);
  
  Serial.print("Select Button Pressed: ");
  Serial.println(selectButtonPressed ? "Yes" : "No");
  
  delay(500);
}