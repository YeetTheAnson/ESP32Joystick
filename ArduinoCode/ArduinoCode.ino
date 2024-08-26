#include <Wire.h>
#include <Joystick.h>

const int I2C_SLAVE_ADDRESS = 9;  
const int BUFFER_SIZE = 32;       

char buffer[BUFFER_SIZE];        
int bufferIndex = 0;            

Joystick_ Joystick;             

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);  
  Wire.onReceive(receiveEvent);   

  Serial.begin(115200);
  Serial.println("I2C Slave Ready");
  
  Joystick.begin();

  Joystick.setXAxisRange(0, 1023); 
  Joystick.setYAxisRange(0, 1023); 
}

void loop() {
}

void receiveEvent(int byteCount) {
  bufferIndex = 0;  

  while (Wire.available()) {
    if (bufferIndex < BUFFER_SIZE - 1) {
      char c = Wire.read();
      buffer[bufferIndex++] = c;
    } else {
      Wire.read();  
    }
  }

  buffer[bufferIndex] = '\0';  

  Serial.print("Received: ");
  Serial.println(buffer);

  if (buffer[0] == 'L' && buffer[1] == 'J') {
    parseJoystick("Left Joystick", true);
  } else if (buffer[0] == 'R' && buffer[1] == 'J') {
    parseJoystick("Right Joystick", false);
  } else {
    // For buttons
    Serial.print("Button: ");
    Serial.print(buffer);
    Serial.print(" State: ");
    Serial.println(buffer[bufferIndex - 1]);
  }

  Serial.println();
}

void parseJoystick(const char* joystickName, bool isLeft) {
  int xValue = 0;
  int yValue = 0;

  char xSign = buffer[2];
  char xDigits[5] = {0};
  strncpy(xDigits, &buffer[3], 4);
  xDigits[4] = '\0';

  int xValueRaw = atoi(xDigits);
  if (xSign == '-') {
    xValueRaw = -xValueRaw;
  }
  xValue = map(xValueRaw, -1024, 1024, 0, 1023);

  char ySign = buffer[7];
  char yDigits[5] = {0};
  strncpy(yDigits, &buffer[8], 4);
  yDigits[4] = '\0';

  int yValueRaw = atoi(yDigits);
  if (ySign == '-') {
    yValueRaw = -yValueRaw;
  }
  yValue = map(yValueRaw, -1024, 1024, 0, 1023);

  if (isLeft) {
    Joystick.setXAxis(xValue); 
    Joystick.setYAxis(yValue); 
  } else {
    Joystick.setRxAxis(xValue); 
    Joystick.setRyAxis(yValue); 
  }

  Serial.println(joystickName);
  Serial.print("X: ");
  Serial.println(xValue);
  Serial.print("Y: ");
  Serial.println(yValue);
}