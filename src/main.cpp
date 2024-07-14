#include "myconfig.h"


/*
Servo���� GND 5V PIN:1
NFC���� SDA:15 SCK:14 MOSI:13 MISO:12  IRQ:NULL  GND  RST:9  3.3V
ָ������ 5v 5v GND RX:17 TX:16 GND
����ʶ��TX 4 RX 5
*/


void setup()
{
  // put your setup code here, to run once:
  Servo_Init();
  OTA_Init();
  Finger_Init();
  NFC_Init();
  Face_Init();
  Check_VERSION();
}

void loop()
{
  //Face_verification();
  Face_registration();
  delay(2000);
}