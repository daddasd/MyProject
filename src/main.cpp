#include <Arduino.h>
#include "myconfig.h"
// put function declarations here:

void setup()
{
  // put your setup code here, to run once:
  Servo_Init();
  OTA_Init();
  Finger_Init();
  NFC_Init();
}

void loop()
{
  OTA_Doing();
  if(NFC_Authenticate()==true)
  {
    printf("ok");
  }
  
    // if (Serial.available() > 0)
    // {
    //   // 读取完整的输入字符串
    //   String user_order = Serial.readStringUntil('\n');
    //   user_order.trim(); // 去除字符串前后的空白字符

    //   // 清空串口缓冲区
    //   Serial.flush();
    //   while (Serial.available() > 0)
    //   {
    //     Serial.read(); // 读取并丢弃所有剩余数据
    //   }

    //   if (user_order == "NFC")
    //   {
    //     if (NFC_Certification()==true)
    //       {
    //         printf
    //       }
    //   }
    // }
  }
