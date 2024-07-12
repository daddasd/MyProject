#include <Arduino.h>
#include "myconfig.h"
// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Servo_Init();
  OTA_Init();
}

void loop() 
{
  OTA_Doing();
  delay(600);
}

