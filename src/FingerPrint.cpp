#include "myconfig.h"

// 17��Rx�� �� 16��Tx��
HardwareSerial mySerial2(2);

#define u8  uint8_t
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial2);

char user_order = ' '; // �洢�û�����Ĵ���ָ��
u8 ID_NUM = 0;         // �洢��ӵ�ָ��ID
/**
 * @brief ָ��ģ���ʼ��
 * 
 */
void Finger_Init(void)
{
    mySerial2.begin(57600, SERIAL_8N1, 17, 16);
    finger.begin(57600);
}
/**
 * @brief ���ָ��
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
            // �밴��ָ
            Serial.println("Please Press...");
            ensure = finger.getImage();
            if (ensure == FINGERPRINT_OK)
            {
                ensure = finger.image2Tz(1); // ��������
                if (ensure == FINGERPRINT_OK)
                {
                    // ָ������
                    Serial.println("Press OK");
                    i = 0;
                    processnum = 1; // �����ڶ���
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
            // �ٰ�һ��
            Serial.println("Please Press Again...");
            ensure = finger.getImage();
            if (ensure == FINGERPRINT_OK)
            {
                ensure = finger.image2Tz(2); // ��������
                if (ensure == FINGERPRINT_OK)
                {
                    // ָ������
                    Serial.println("Press Again OK");
                    i = 0;
                    processnum = 2; // ����������
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
            // ����ģ��
            Serial.print("Finger Create...");
            ensure = finger.createModel();
            if (ensure == FINGERPRINT_OK)
            {
                Serial.println("Success");
                processnum = 3; // �������Ĳ�
            }
            else
            {
                // ����ģ��ʧ��
                Serial.println("Fail");
                i = 0;
                processnum = 0; // ���ص�һ��
            }
            delay(500);
            break;
        case 3:
            // д��ָ��ID
            ID_NUM++;
            ensure = finger.storeModel(ID_NUM); // ����ģ��
            if (ensure == 0x00)
            {
                // ¼��ָ�Ƴɹ� ID��ӡ
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
        if (i == 10) // ����5��û�а���ָ���˳�
        {
            Serial.println("Timeout !!!");
            break;
        }
    }
}
/**
 * @brief ��ָ֤��
 * 
 */
void Press_FR()
{
    u8 ensure, i, j = 0;
    // �밴��ָ
    Serial.println("---------------------------");
    Serial.print("Please Press Fingerprint...");
    for (i = 0; i < 15; i++)
    {
        ensure = finger.getImage();
        if (ensure == 0x00) // ��ȡͼ��ɹ�
        {
            ensure = finger.image2Tz();
            if (ensure == 0x00) // ���������ɹ�
            {
                ensure = finger.fingerFastSearch();
                if (ensure == 0x00) // �����ɹ�
                {
                    // ָ����֤�ɹ�
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
                    // δ������ָ��
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
            // �ȴ���ʱ
            Serial.println("Timeout !!!");
            Serial.println(" ");
        }
    }
}

