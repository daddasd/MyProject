#include <Arduino.h>
#include "myconfig.h"
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Servo_Init();
  handleRoot();
  OTA_Init();
}

void loop() {
  Web_Doing();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}