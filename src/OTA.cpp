#include <Arduino.h>
#include <math.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
// #include <ArduinoMDNS.h> // ����mDNS��

const char *ssid = "ESP32-c3_OTAdemo"; // AP������
const char *password = "123456789";    // AP������

IPAddress local_IP(192, 168, 4, 1); // ��̬IP��ַ
IPAddress gateway(192, 168, 4, 1);  // ����IP��ַ

WebServer server(80);



const char *updateIndex =
    "<html>"
    "<head>"
    "<meta charset=\"UTF-8\">"
    "<title>ESP32 OTA ����</title>"
    "<style>"
    "body { font-family: Arial, sans-serif; text-align: center; }"
    "h1 { color: #333; }"
    "form { margin-top: 20px; }"
    "input[type=file] { display: block; margin: 20px auto; }"
    "input[type=submit] { margin-top: 20px; padding: 10px 20px; font-size: 18px; }"
    "</style>"
    "</head>"
    "<body>"
    "<h1>��ӭʹ�� ESP32 OTA ����</h1>"
    "<form method='POST' action='/update' enctype='multipart/form-data'>"
    "<input type='file' name='update' accept='.bin'>"
    "<input type='submit' value='�ϴ��̼�'>"
    "</form>"
    "</body>"
    "</html>";

void handleRoot()
{
    server.sendHeader("Location", "/update");
    server.send(302, "text/plain", "");
}

void OTA_Init()
{
    Serial.begin(115200); // ��ʼ�����ڣ�������Ϊ115200
    Serial.println("Booting...");

    // �� ESP32 ����Ϊ AP ģʽ��ָ����̬ IP ��ַ
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_IP, gateway, IPAddress(255, 255, 255, 0));

    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP()); // ��ӡ ESP32 �� AP IP ��ַ

    // ���÷�����������
    server.on("/", HTTP_GET, handleRoot); // ��·���ض��� OTA ҳ��

    server.on("/update", HTTP_GET, []()
              {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", updateIndex); });

    server.on("/update", HTTP_POST, []()
              {
    server.sendHeader("Connection", "close");

    //��̬��ʾ���
  String message = Update.hasError() ? "����ʧ��" : "���³ɹ�������������";
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  server.send(200, "text/html", "<span style='font-size: 24px;'>" + message + "</span>");


    delay(1000);
    ESP.restart(); }, []()
              {
    HTTPUpload& upload = server.upload(); //���ڴ����ϴ����ļ�����
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // �������ô�С��ʼ
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // �����յ�������д��Update����
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // ���ô�СΪ��ǰ��С
        Serial.printf("Update Success: %u bytes\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    } });

    server.begin();
    Serial.println("HTTP server started");

    // �����߼� �汾1.1
    Serial.println();
    Serial.println("NEW ESP32C3!!");
}


void Web_Doing(void)
{
    server.handleClient();
}
