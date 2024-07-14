#include <WiFi.h>
#include <HTTPUpdate.h>
#include <EEPROM.h>

const char *host = "esp32";
const char *ssid = "xzhi";
const char *password = "qwer1234";
String upUrl = "http://bin.bemfa.com/b/3BcY2IwNDJlZGE1MWY4NGU4MDliYWJhZDBiMDAzODc2NWY=ENLOCK.bin";

#define CURRENT_VERSION "1.0.0" // 当前版本
#define NEW_VERSION "1.5.5"     // 要更新的版本

void OTA_Init(void)
{
  int Flag = 0;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  EEPROM.begin(512); // 初始化 EEPROM

  Flag = EEPROM.read(64);
  if(Flag==1)
  {
    EEPROM.write(64,0);
  }
  else
    EEPROM.write(64, 1);
  Serial.printf("OTA_Flag: %d",Flag);
}

void update_started()
{
  Serial.println("CALLBACK: HTTP update process started");
}

void update_finished()
{
  Serial.println("CALLBACK: HTTP update process finished");
}

void update_progress(int cur, int total)
{
  Serial.printf("CALLBACK: HTTP update process at %d of %d bytes... %.2f%%\n", cur, total, (cur / (float)total) * 100);
}

void update_error(int err)
{
  Serial.printf("CALLBACK: HTTP update fatal error code %d\n", err);
}

void updateBin()
{
  Serial.println("Start update");
  WiFiClient UpdateClient;

  httpUpdate.onStart(update_started);
  httpUpdate.onEnd(update_finished);
  httpUpdate.onProgress(update_progress);
  httpUpdate.onError(update_error);

  t_httpUpdate_return ret = httpUpdate.update(UpdateClient, upUrl);
  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.println("[update] Update failed.");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("[update] No Update.");
    break;
  case HTTP_UPDATE_OK:
    Serial.println("[update] Update successful.");
    break;
  }
}

void Write_EEPROM_VERSION(int startAddress, String version)
{
  for (int i = 0; i < version.length(); i++)
  {
    EEPROM.write(startAddress + i, version[i]);
  }
  EEPROM.write(startAddress + version.length(), '\0'); // 以 null 终止字符串
  EEPROM.commit();
}

String Read_EEPROM_VERSION(int startAddress)
{
  char version[32 + 1]; // +1 for null terminator
  for (int i = 0; i < 32; i++)
  {
    version[i] = EEPROM.read(startAddress + i);
  }
  version[32] = '\0'; // null terminate the string
  return String(version);
}

void Check_VERSION()
{
  String currentVersion = Read_EEPROM_VERSION(0); //1.0.1
  String newVersion = Read_EEPROM_VERSION(32);   // 1.0.2



  Serial.print("EEPROM Current version: ");
  Serial.println(currentVersion);
  Serial.print("EEPROM New version: ");
  Serial.println(newVersion);

  if (currentVersion != CURRENT_VERSION)
  {
    Write_EEPROM_VERSION(0, CURRENT_VERSION); // 写入当前版本到 EEPROM
  }

  if (newVersion != NEW_VERSION)      //  1.0.5
  {
    Write_EEPROM_VERSION(32, NEW_VERSION); // 写入新版本到 EEPROM
  }

  if (currentVersion != NEW_VERSION || EEPROM.read(64)==1) // 1.0.1 不等于1.0.5
  {
    Serial.println("Version mismatch or update required. Performing OTA update...");
    Write_EEPROM_VERSION(0, NEW_VERSION);  //1.0.5替换
    updateBin();
  }
  else
  {
    Serial.println("Firmware is up to date.");
  }
}
