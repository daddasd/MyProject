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
    //   // ��ȡ�����������ַ���
    //   String user_order = Serial.readStringUntil('\n');
    //   user_order.trim(); // ȥ���ַ���ǰ��Ŀհ��ַ�

    //   // ��մ��ڻ�����
    //   Serial.flush();
    //   while (Serial.available() > 0)
    //   {
    //     Serial.read(); // ��ȡ����������ʣ������
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
