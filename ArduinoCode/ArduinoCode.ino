#include <Wire.h>
#include <XInput.h>

const int I2C_SLAVE_ADDRESS = 9; 
const int BUFFER_SIZE = 32; 

char buffer[BUFFER_SIZE]; 
int bufferIndex = 0;   

const int ADC_Max = 1023;  

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS); 
  Wire.onReceive(receiveEvent);

  Serial.begin(115200);
  Serial.println("I2C Slave Ready");

  XInput.setJoystickRange(0, ADC_Max);  
  XInput.setAutoSend(false); 

  XInput.begin();
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

  
  parseControlData();
  
  
  XInput.send();
}

void parseControlData() {
  if (buffer[0] == 'L' && buffer[1] == 'J') {
    parseJoystick(true); 
  } else if (buffer[0] == 'R' && buffer[1] == 'J') {
    parseJoystick(false); 
  } else {
   
    char button = buffer[0];
    char state = buffer[2];

    bool pressed = (state == '1');

    switch (button) {
      case 'A': XInput.setButton(BUTTON_A, pressed); break;
      case 'B': XInput.setButton(BUTTON_B, pressed); break;
      case 'X': XInput.setButton(BUTTON_X, pressed); break;
      case 'Y': XInput.setButton(BUTTON_Y, pressed); break;
      
      case 'L': XInput.setButton(BUTTON_LB, pressed); break;
      case 'R': XInput.setButton(BUTTON_RB, pressed); break;
      
      case 'T': XInput.setTrigger(TRIGGER_LEFT, pressed ? ADC_Max : 0); break; 
      case 'G': XInput.setTrigger(TRIGGER_RIGHT, pressed ? ADC_Max : 0); break;

      case 'D':
        if (buffer[1] == 'U') XInput.setDpad(pressed, false, false, false); 
        else if (buffer[1] == 'D') XInput.setDpad(false, pressed, false, false); 
        else if (buffer[1] == 'L') XInput.setDpad(false, false, pressed, false);  
        else if (buffer[1] == 'R') XInput.setDpad(false, false, false, pressed);  
        break;

      case 'S': XInput.setButton(BUTTON_START, pressed); break; 
      case 'P': XInput.setButton(BUTTON_BACK, pressed); break; 

      case 'M': XInput.setButton(BUTTON_BACK, pressed); break; 

      default: Serial.print("Unhandled data: "); Serial.println(buffer); break;
    }
  }
}

void parseJoystick(bool isLeft) {
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
  xValue = map(xValueRaw, -1024, 1024, 0, ADC_Max);

  char ySign = buffer[7]; 
  char yDigits[5] = {0};
  strncpy(yDigits, &buffer[8], 4);
  yDigits[4] = '\0';

  int yValueRaw = atoi(yDigits);
  if (ySign == '-') {
    yValueRaw = -yValueRaw;
  }
  yValue = map(yValueRaw, -1024, 1024, 0, ADC_Max);

  if (isLeft) {
    XInput.setJoystickX(JOY_LEFT, xValue);
    XInput.setJoystickY(JOY_LEFT, yValue);
  } else {
    XInput.setJoystickX(JOY_RIGHT, xValue);
    XInput.setJoystickY(JOY_RIGHT, yValue);
  }

  Serial.println(isLeft ? "Left Joystick" : "Right Joystick");
  Serial.print("X: ");
  Serial.println(xValue);
  Serial.print("Y: ");
  Serial.println(yValue);
}