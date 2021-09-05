#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <FTPServer.h>
#include <pumper.h>
#include <TaskScheduler.h>

#if (defined ESP32)
#error "No LittlFS on the ESP32"
#endif

#define BAUDRATE 74880

// Определяем объект и порт сервера для работы с HTTP
ESP8266WebServer HTTP(80);

// tell the FtpServer to use LittleFS
FTPServer ftpSrv(LittleFS);

byte RELAY = 4;

extern Scheduler runner;
extern Task t1;

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

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  HTTP.begin();                   // Инициализируем Web-сервер
  ftpSrv.begin("relay", "relay"); // Инициализируем FTP-сервер

  runner.startNow(); // set point-in-time for scheduling start
}

void loop()
{
  HTTP.handleClient(); // Обработчик HTTP-событий (отлавливает HTTP-запросы к устройству и обрабатывает их в соответствии с выше описанным алгоритмом)
  ftpSrv.handleFTP();  // Обработчик FTP-соединений

  MDNS.update();
  runner.execute();
}