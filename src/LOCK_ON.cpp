#include "myconfig.h"


Servo myservo;
/**
 * @brief 初始化舵机引脚为PIN_1
 * 
 */
void Servo_Init(void)
{
    myservo.attach(1, 500, 2500);
}


void Servo_ON(void)
{
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(500);
}