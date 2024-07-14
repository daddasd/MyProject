#include "myconfig.h"

/* 
NFC���� SDA:15 SCK:14 MOSI:13 MISO:12 IRQ:NULL GND RST:9 3.3V
*/

#define SS_PIN 15
#define RST_PIN 9

#define PIN_NUM_MISO  12
#define PIN_NUM_MOSI  13
#define PIN_NUM_CLK   14
#define PIN_NUM_CS    15

MFRC522 rfid(SS_PIN, RST_PIN); // ʵ������
byte nuidPICC[4]={0,0,0,0};
uint8_t IC_ATTEST[32][4] = 
{{0X23, 0X38, 0X3B, 0X60}, {0X7B, 0X01, 0X52, 0X11}, {0XCF, 0XAE, 0XF2, 0X2E}, {0X5F, 0X87, 0XF6, 0X2E}, {0XEF, 0XBA, 0X1B, 0X2F}, {0XBF, 0X6E, 0XFA, 0X2D}, {0X3F, 0XE7, 0X1C, 0X2F}, {0XCF, 0X07, 0XF4, 0X2E}, {0XBF, 0X2E, 0XF4, 0X2E}, {0X2F, 0XE7, 0X08, 0X2E}, {0X3F, 0XC0, 0X1F, 0X2F}, {0X9F, 0X64, 0X1E, 0X2F}, {0XBF, 0XE0, 0XF2, 0X2E}, {0X0F, 0XB2, 0XF4, 0X2E}, {0X8F, 0XB2, 0XD9, 0X2E}, {0XCF, 0X9D, 0XC2, 0X2E}, {0X2F, 0XD4, 0XF4, 0X2E}, {0XEF, 0X7F, 0XF9, 0X2E}, {0X1F, 0XA8, 0XF2, 0X2E}, {0X5F, 0X9F, 0XF5, 0X2E}, {0X5F, 0X77, 0XFA, 0X2E}, {0X6F, 0XBB, 0XF3, 0X2E}, {0X4F, 0X72, 0XDB, 0X2E}, {0X1F, 0XF4, 0XC1, 0X2E}, {0X6F, 0X36, 0X89, 0X2E}, {0XCF, 0XC9, 0X91, 0X2E}, {0X7F, 0X87, 0X8F, 0X2E}, {0XFF, 0XE3, 0XFA, 0X2E}, {0X7F, 0X8B, 0XC1, 0X2E}, {0X1F, 0XA8, 0XF2, 0X2E}, {0XDF, 0X55, 0XBE, 0X2E}, {0X6F, 0X55, 0XBF, 0X2E}};

/**
 * @brief NFC��ʼ��
 * 
 */
void NFC_Init()
{
    SPI.begin(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CS);
    rfid.PCD_Init(); // ��ʼ�� MFRC522
}
/**
 * @brief NFC��ȡID��
 * 
 */
void loopNFC()
{
    // Look for new cards
    if (!rfid.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!rfid.PICC_ReadCardSerial())
        return;

    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

    // Check for MIFARE cards
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println("��֧�ֶ�ȡ�˿�����");
        return;
    }

    // Save NUID into nuidPICC array
    for (byte i = 0; i < 4; i++)
    {
        nuidPICC[i] = rfid.uid.uidByte[i];
    }

    Serial.print("ʮ������UID��");
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    Serial.print("ʮ����UID��");
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
}
/**
 * @brief ��֤ID
 * 
 * @return true ��֤�ɹ�
 * @return false ��֤ʧ��
 */
bool NFC_Authenticate()
{
    loopNFC(); // �������������������

    for (int j = 0; j < 32; j++)
    {
        bool match = true; // ���赱ǰ��ƥ��
        for (int i = 0; i < 4; i++)
        {
            if (nuidPICC[i] != IC_ATTEST[j][i])
            {
                match = false; // ֻҪ��һ��Ԫ�ز�ƥ�䣬��ǰ�оͲ�ƥ��
                break;
            }
        }
        if (match)
        {
            memset(nuidPICC, 0, 4); // �������
            return true;
        }
    }
    return false; // ���û��ƥ����У����� false
}

void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 10 ? " 0" : "");
        Serial.print(buffer[i], DEC);
    }
}