#ifndef _MYCONFIG_H
#define _MYCONFIG_H

#include "Arduino.h"
#include "ESP32Servo.h"
#include "Update.h"
#include "WiFi.h"
#include "esp_log.h"
#include "WebServer.h"
#include "math.h"

#define LOCK_ON Servo_ON();
void Servo_Init(void);
void Servo_ON(void);

/*OTA*/
void handleRoot(void);
void OTA_Init(void);
void Web_Doing(void);
/*OTA*/


#endif
