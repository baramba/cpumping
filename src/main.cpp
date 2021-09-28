#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <pumper.h>
#include <TaskScheduler.h>
#include <Relay.h>

#if (defined ESP32)
#error "No LittlFS on the ESP32"
#endif

#define BAUDRATE 74880

// Необходимо задать номер управляющего пина (GPIO)
#define RELAY_PIN1 4
//#define RELAY_PIN2 4

//Логин и пароль от wifi
#define SSID "sosna-2"
#define PASS "+79119652485MM"


// Определяем объект и порт сервера для работы с HTTP
ESP8266WebServer HTTP(80);

Scheduler runner;

Relay relay1 = Relay(RELAY_PIN1);

void setup()
{
  Serial.begin(BAUDRATE);

  // if   (WiFi.SSID() == staConfig.ssid || WiFi.psk() == staConfig.password) WiFi.begin();  //start station with saved credentials
  // else WiFi.begin(staConfig.ssid.c_str(), staConfig.password.c_str()); //start station with new credentials

  /* Connect WiFi */
  // WiFi.mode(WIFI_STA);
  // WiFi.begin("ssid", "password");
  // if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //     Serial.printf("WiFi Failed!\n");
  //     return;
  // }
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.localIP());

  WiFi.begin(SSID, PASS);

  bool fsok = LittleFS.begin();
  Serial.printf_P(PSTR("FS init: %s\n"), fsok ? PSTR("ok") : PSTR("fail!"));

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.printf_P(PSTR("."));
  }
  Serial.printf_P(PSTR("\nConnected to %s, IP address is %s\n"), SSID, WiFi.localIP().toString().c_str());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  HTTP.begin();                   // Инициализируем Web-сервер
  httpInit();
  runner.startNow(); // set point-in-time for scheduling start
}

void loop()
{
  HTTP.handleClient(); // Обработчик HTTP-событий (отлавливает HTTP-запросы к устройству и обрабатывает их в соответствии с выше описанным алгоритмом)
  MDNS.update();
  runner.execute();
}