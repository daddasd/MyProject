#include "myconfig.h"

// 17（Rx） 和 16（Tx）
HardwareSerial mySerial2(2);

#define u8  uint8_t
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial2);

char user_order = ' '; // 存储用户输入的串口指令
u8 ID_NUM = 0;         // 存储添加的指纹ID
/**
 * @brief 指纹模块初始化
 * 
 */
void Finger_Init(void)
{
    mySerial2.begin(57600, SERIAL_8N1, 17, 16);
    finger.begin(57600);
}
/**
 * @brief 添加指纹
 * 
 */
void Add_FR()
{
    u8 i = 0, ensure, processnum = 0;
    Serial.println("---------------------------");
    while (1)
    {
        switch (processnum)
        {
        case 0:
            i++;
            // 请按手指
            Serial.println("Please Press...");
            ensure = finger.getImage();
            if (ensure == FINGERPRINT_OK)
            {
                ensure = finger.image2Tz(1); // 生成特征
                if (ensure == FINGERPRINT_OK)
                {
                    // 指纹正常
                    Serial.println("Press OK");
                    i = 0;
                    processnum = 1; // 跳到第二步
                }
                else
                {
                };
            }
            else
            {
            };
            break;

        case 1:
            i++;
            // 再按一次
            Serial.println("Please Press Again...");
            ensure = finger.getImage();
            if (ensure == FINGERPRINT_OK)
            {
                ensure = finger.image2Tz(2); // 生成特征
                if (ensure == FINGERPRINT_OK)
                {
                    // 指纹正常
                    Serial.println("Press Again OK");
                    i = 0;
                    processnum = 2; // 跳到第三步
                }
                else
                {
                };
            }
            else
            {
            };
            break;

        case 2:
            // 创建模板
            Serial.print("Finger Create...");
            ensure = finger.createModel();
            if (ensure == FINGERPRINT_OK)
            {
                Serial.println("Success");
                processnum = 3; // 跳到第四步
            }
            else
            {
                // 创建模板失败
                Serial.println("Fail");
                i = 0;
                processnum = 0; // 跳回第一步
            }
            delay(500);
            break;
        case 3:
            // 写入指纹ID
            ID_NUM++;
            ensure = finger.storeModel(ID_NUM); // 储存模板
            if (ensure == 0x00)
            {
                // 录入指纹成功 ID打印
                Serial.print("Add Fingerprint Success --- ");
                if ((ID_NUM > 0) && (ID_NUM < 10))
                    Serial.print("ID = 0");
                if (ID_NUM >= 10)
                    Serial.print("ID = ");
                Serial.println(ID_NUM);
                Serial.println(" ");
                delay(1500);
                return;
            }
            else
            {
                processnum = 0;
            }
            break;
        }
        delay(400);
        if (i == 10) // 超过5次没有按手指则退出
        {
            Serial.println("Timeout !!!");
            break;
        }
    }
}
/**
 * @brief 验证指纹
 * 
 */
void Press_FR()
{
    u8 ensure, i, j = 0;
    // 请按手指
    Serial.println("---------------------------");
    Serial.print("Please Press Fingerprint...");
    for (i = 0; i < 15; i++)
    {
        ensure = finger.getImage();
        if (ensure == 0x00) // 获取图像成功
        {
            ensure = finger.image2Tz();
            if (ensure == 0x00) // 生成特征成功
            {
                ensure = finger.fingerFastSearch();
                if (ensure == 0x00) // 搜索成功
                {
                    // 指纹验证成功
                    Serial.println(" ");
                    Serial.println("Fingerprint verification successful !!!");
                    if ((0 < finger.fingerID) && (finger.fingerID < 10))
                        Serial.print("Welcome ID : 0");
                    if (finger.fingerID >= 10)
                        Serial.print("Welcome ID : ");
                    Serial.println(finger.fingerID);
                    Serial.println(" ");
                    j = 0;
                    delay(2000);
                    break;
                }
                else
                {
                    // 未搜索到指纹
                    Serial.println(" ");
                    Serial.println("!!! Fingerprint not found !!!");
                    Serial.println(" ");
                    delay(2000);
                    break;
                }
            }
            else
            {
            }
        }
        j++;
        delay(200);

        if (j >= 15)
        {
            // 等待超时
            Serial.println("Timeout !!!");
            Serial.println(" ");
        }
    }
}

