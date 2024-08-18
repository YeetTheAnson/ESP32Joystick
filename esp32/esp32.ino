#include <Adafruit_TinyUSB.h>

#define JOYSTICK_X 0x01
#define JOYSTICK_Y 0x02
#define BUTTON_A   0x01
#define BUTTON_B   0x02
#define BUTTON_X   0x04
#define BUTTON_Y   0x08
#define LEFT_TRIGGER 0x10
#define RIGHT_TRIGGER 0x20
#define LEFT_BUMPER 0x40
#define RIGHT_BUMPER 0x80

Adafruit_USBD_HID hid;

struct JoystickReport {
  uint8_t buttons;
  int16_t x;
  int16_t y;
};

JoystickReport report = {0, 0, 0};

const int actionDuration = 500; 
const int buttonPressDuration = 1000; 

void setup() {
  hid.begin();
}

void loop() {
  simulateJoystickMovement(0, -32767); 
  delay(actionDuration);
  simulateJoystickMovement(0, 32767);  
  delay(actionDuration);
  simulateJoystickMovement(-32767, 0); 
  delay(actionDuration);
  simulateJoystickMovement(32767, 0);  
  delay(actionDuration);

  simulateDpadMovement(0);      
  delay(actionDuration);
  simulateDpadMovement(18000);   
  delay(actionDuration);
  simulateDpadMovement(27000);  
  delay(actionDuration);
  simulateDpadMovement(9000);   
  delay(actionDuration);

  // Button presses
  simulateButtonPress(BUTTON_A);  
  delay(buttonPressDuration);
  simulateButtonRelease(BUTTON_A);

  simulateButtonPress(BUTTON_B); 
  delay(actionDuration);
  simulateButtonRelease(BUTTON_B);

  simulateButtonPress(BUTTON_X);   
  delay(actionDuration);
  simulateButtonRelease(BUTTON_X);

  simulateButtonPress(BUTTON_Y);  
  delay(actionDuration);
  simulateButtonRelease(BUTTON_Y);

  simulateButtonPress(LEFT_TRIGGER); 
  delay(actionDuration);
  simulateButtonRelease(LEFT_TRIGGER);

  simulateButtonPress(RIGHT_TRIGGER);
  delay(actionDuration);
  simulateButtonRelease(RIGHT_TRIGGER);

  simulateButtonPress(LEFT_BUMPER);  
  delay(actionDuration);
  simulateButtonRelease(LEFT_BUMPER);

  simulateButtonPress(RIGHT_BUMPER); 
  delay(actionDuration);
  simulateButtonRelease(RIGHT_BUMPER);

}

void simulateJoystickMovement(int16_t x, int16_t y) {
  report.x = x;
  report.y = y;
  hid.sendReport(0, &report, sizeof(report));
}

void simulateDpadMovement(uint16_t angle) {
  report.x = angle; 
  hid.sendReport(0, &report, sizeof(report));
}

void simulateButtonPress(uint8_t button) {
  report.buttons |= button;
  hid.sendReport(0, &report, sizeof(report));
}

void simulateButtonRelease(uint8_t button) {
  report.buttons &= ~button;
  hid.sendReport(0, &report, sizeof(report));
}
