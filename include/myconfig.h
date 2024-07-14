#ifndef _MYCONFIG_H
#define _MYCONFIG_H

#include "Arduino.h"
#include "ESP32Servo.h"
#include "Update.h"
#include "WiFi.h"
#include "esp_log.h"
#include "WebServer.h"
#include "math.h"
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include "esp_log.h"
#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <cstring> // °üº¬memsetº¯Êý
#include"SoftwareSerial.h"
#include <HTTPUpdate.h>
#include <EEPROM.h>

#define LOCK_ON Servo_ON();
void Servo_Init(void);
void Servo_ON(void);

/*OTA*/
void OTA_Init(void);
void updateBin(void);
void Check_VERSION(void);
String Read_EEPROM_VERSION(int startAddress);
void Write_EEPROM_VERSION(int startAddress, String version);
/*OTA*/

/*****FingerPrint******/
void Finger_Init(void);
void Add_FR();
void Press_FR();
/*****FingerPrint******/

/*****NFC******/
extern MFRC522 rfid;
extern byte nuidPICC[4];

void NFC_Init();
void loopNFC();
void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);
bool NFC_Authenticate();
/*****NFC******/

/*****FaceID******/
void Face_Init(void);
bool Face_registration();
bool Face_verification();
/*****FaceID******/



#endif
