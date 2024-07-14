#include "myconfig.h"

uint8_t FACE_REGISTRATION[] = {0XEF, 0XAA, 0X13, 0X00, 0X00, 0X00, 0X00, 0X13};   // 人脸注册
uint8_t ON_BACKLIGHT[] = {0XEF, 0XAA, 0XC1, 0X00, 0x00, 0x00, 0x01, 0X01, 0XC3};  // 打开显示
uint8_t OFF_BACKLIGHT[] = {0XEF, 0XAA, 0XC1, 0X00, 0x00, 0x00, 0x01, 0X00, 0XC2}; // 关闭显示
uint8_t RECOGNITION[] = {0xEF, 0xAA, 0x12, 0x00, 0x00, 0x00, 0x00, 0x12};         // 识别认证
uint8_t JUDGE[10];                                                                // 判断是否成功
uint8_t RX_data[100];

// 定义软串口的RX和TX引脚
const int RX_PIN = 4;
const int TX_PIN = 5;

// 创建SoftwareSerial对象
SoftwareSerial FaceSerial(RX_PIN, TX_PIN);

void Face_Init()
{
    FaceSerial.begin(115200);
    delay(100); // 增加延迟以确保模块准备好;
    FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
    FaceSerial.println();
    delay(1000);
    FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
    memset(RX_data, 0, sizeof(RX_data)); // 初始化数组为0
}
/**
 * @brief 人脸注册
 * 
 * @return true 注册成功
 * @return false 注册失败
 */
bool Face_registration()
{
    FaceSerial.write(ON_BACKLIGHT, sizeof(ON_BACKLIGHT));
    delay(50);
    FaceSerial.write(FACE_REGISTRATION, sizeof(FACE_REGISTRATION));
    delay(5000); // 给模块一些时间处理请求
    // Read the incoming data into RX_data array
    int index = 0;
    while (FaceSerial.available() || index < 10)
    {
        RX_data[index++] = FaceSerial.read();
    }
    if (memcmp(RX_data, "\xEF\xAA\x00\x00\x00\x00\x04\x13\x00\x00", 10) == 0) // 注册成功
    {
        memset(RX_data, 0, sizeof(RX_data)); // 初始化数组为0
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        return true;
    }
    else // 注册失败
    {
        memset(RX_data, 0, sizeof(RX_data)); // 初始化数组为0
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        return false;
    }
}

bool Face_verification()
{
    FaceSerial.write(ON_BACKLIGHT, sizeof(ON_BACKLIGHT));
    delay(50);
    FaceSerial.write(RECOGNITION, sizeof(RECOGNITION));
    delay(10000); // 给模块一些时间处理请求
    // Read the incoming data into RX_data array
    int index = 0;
    while (FaceSerial.available() || index < 10)
    {
        RX_data[index++] = FaceSerial.read();
    }
    if (memcmp(RX_data, "\xEF\xAA\x00\x00\x00\x00\x04\x12\x00", 9) == 0) // 识别成功
    {
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        memset(RX_data, 0, 100); // 初始化数组为0
        return true;
    }
    else
    {
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        return false;
    }
}