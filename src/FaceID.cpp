#include "myconfig.h"

uint8_t FACE_REGISTRATION[] = {0XEF, 0XAA, 0X13, 0X00, 0X00, 0X00, 0X00, 0X13};   // ����ע��
uint8_t ON_BACKLIGHT[] = {0XEF, 0XAA, 0XC1, 0X00, 0x00, 0x00, 0x01, 0X01, 0XC3};  // ����ʾ
uint8_t OFF_BACKLIGHT[] = {0XEF, 0XAA, 0XC1, 0X00, 0x00, 0x00, 0x01, 0X00, 0XC2}; // �ر���ʾ
uint8_t RECOGNITION[] = {0xEF, 0xAA, 0x12, 0x00, 0x00, 0x00, 0x00, 0x12};         // ʶ����֤
uint8_t JUDGE[10];                                                                // �ж��Ƿ�ɹ�
uint8_t RX_data[100];

// �������ڵ�RX��TX����
const int RX_PIN = 4;
const int TX_PIN = 5;

// ����SoftwareSerial����
SoftwareSerial FaceSerial(RX_PIN, TX_PIN);

void Face_Init()
{
    FaceSerial.begin(115200);
    delay(100); // �����ӳ���ȷ��ģ��׼����;
    FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
    FaceSerial.println();
    delay(1000);
    FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
    memset(RX_data, 0, sizeof(RX_data)); // ��ʼ������Ϊ0
}
/**
 * @brief ����ע��
 * 
 * @return true ע��ɹ�
 * @return false ע��ʧ��
 */
bool Face_registration()
{
    FaceSerial.write(ON_BACKLIGHT, sizeof(ON_BACKLIGHT));
    delay(50);
    FaceSerial.write(FACE_REGISTRATION, sizeof(FACE_REGISTRATION));
    delay(5000); // ��ģ��һЩʱ�䴦������
    // Read the incoming data into RX_data array
    int index = 0;
    while (FaceSerial.available() || index < 10)
    {
        RX_data[index++] = FaceSerial.read();
    }
    if (memcmp(RX_data, "\xEF\xAA\x00\x00\x00\x00\x04\x13\x00\x00", 10) == 0) // ע��ɹ�
    {
        memset(RX_data, 0, sizeof(RX_data)); // ��ʼ������Ϊ0
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        return true;
    }
    else // ע��ʧ��
    {
        memset(RX_data, 0, sizeof(RX_data)); // ��ʼ������Ϊ0
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        return false;
    }
}

bool Face_verification()
{
    FaceSerial.write(ON_BACKLIGHT, sizeof(ON_BACKLIGHT));
    delay(50);
    FaceSerial.write(RECOGNITION, sizeof(RECOGNITION));
    delay(10000); // ��ģ��һЩʱ�䴦������
    // Read the incoming data into RX_data array
    int index = 0;
    while (FaceSerial.available() || index < 10)
    {
        RX_data[index++] = FaceSerial.read();
    }
    if (memcmp(RX_data, "\xEF\xAA\x00\x00\x00\x00\x04\x12\x00", 9) == 0) // ʶ��ɹ�
    {
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        memset(RX_data, 0, 100); // ��ʼ������Ϊ0
        return true;
    }
    else
    {
        FaceSerial.write(OFF_BACKLIGHT, sizeof(OFF_BACKLIGHT));
        return false;
    }
}