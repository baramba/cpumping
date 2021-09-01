#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <FTPServer.h>
#include <TickerScheduler.h>
#include <pumper.h>

#if (defined ESP32)
#error "No LittlFS on the ESP32"
#endif

#define BAUDRATE 74880

// Определяем объект и порт сервера для работы с HTTP
ESP8266WebServer HTTP(80);

// tell the FtpServer to use LittleFS
FTPServer ftpSrv(LittleFS);

TickerScheduler taskSch(1);

byte RELAY = 4;

const char *ssid = "sosna-2";
const char *password = "+79119652485MM"; // Название генерируемой точки доступа

void setup()
{

  pinMode(RELAY, OUTPUT);
  Serial.begin(BAUDRATE);

  WiFi.begin(ssid, password);

  bool fsok = LittleFS.begin();
  Serial.printf_P(PSTR("FS init: %s\n"), fsok ? PSTR("ok") : PSTR("fail!"));

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.printf_P(PSTR("."));
  }
  Serial.printf_P(PSTR("\nConnected to %s, IP address is %s\n"), ssid, WiFi.localIP().toString().c_str());

  HTTP.begin();                   // Инициализируем Web-сервер
  ftpSrv.begin("relay", "relay"); // Инициализируем FTP-сервер

  httpInit();
}

void loop()
{
  HTTP.handleClient(); // Обработчик HTTP-событий (отлавливает HTTP-запросы к устройству и обрабатывает их в соответствии с выше описанным алгоритмом)
  ftpSrv.handleFTP();  // Обработчик FTP-соединений
  taskSch.update();
}