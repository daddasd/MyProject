#include "myconfig.h"


Servo myservo;
/**
 * @brief 初始化舵机引脚为PIN_1
 * 
 */
void Servo_Init(void)
{
    myservo.attach(1);
}


void Servo_ON(void)
{
    for (int angle = 0; angle < 90;angle++)
    {
        myservo.write(angle);
        delay(10);
    }
    delay(200);
    for (int angle = 90; angle > 0; angle--)
    {
        myservo.write(angle);
        delay(10);
    }
}