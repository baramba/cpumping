
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <pumper.h>

void httpInit()
{
    // Обработка HTTP-запросов
    HTTP.on("/relay_switch", []()
            { HTTP.send(200, "text/plain", relay_switch()); });

    HTTP.on("/relay_status", []()
            { HTTP.send(200, "text/plain", relay_status()); });

    HTTP.on("/switch_params", []()
            { HTTP.send(200, "text/plain", relay_status()); });

    HTTP.on("/list", []()
            {
                String str = "";
                Dir dir = LittleFS.openDir("/");
                while (dir.next())
                {
                    str += dir.fileName();
                    str += " / ";
                    str += dir.fileSize();
                    str += "\r\n";
                }
                Serial.print("List of file:\n" + str);
                HTTP.send(200, "text/json", "List of file:" + str);
            });

    HTTP.onNotFound([]()
                    {
                        if (!handleFileRead(HTTP.uri()))
                            HTTP.send(404, "text/plain", "Not found - " + HTTP.uri());
                    });
}

String getContentType(String filename){                                 // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
  if (filename.endsWith(".html")) return "text/html";                   // Если файл заканчивается на ".html", то возвращаем заголовок "text/html" и завершаем выполнение функции
  else if (filename.endsWith(".css")) return "text/css";                // Если файл заканчивается на ".css", то возвращаем заголовок "text/css" и завершаем выполнение функции
  else if (filename.endsWith(".js")) return "application/javascript";   // Если файл заканчивается на ".js", то возвращаем заголовок "application/javascript" и завершаем выполнение функции
  else if (filename.endsWith(".png")) return "image/png";               // Если файл заканчивается на ".png", то возвращаем заголовок "image/png" и завершаем выполнение функции
  else if (filename.endsWith(".jpg")) return "image/jpeg";              // Если файл заканчивается на ".jpg", то возвращаем заголовок "image/jpg" и завершаем выполнение функции
  else if (filename.endsWith(".gif")) return "image/gif";               // Если файл заканчивается на ".gif", то возвращаем заголовок "image/gif" и завершаем выполнение функции
  else if (filename.endsWith(".ico")) return "image/x-icon";            // Если файл заканчивается на ".ico", то возвращаем заголовок "image/x-icon" и завершаем выполнение функции
  return "text/plain";                                                  // Если ни один из типов файла не совпал, то считаем что содержимое файла текстовое, отдаем соответствующий заголовок и завершаем выполнение функции
}

bool handleFileRead(String path){                                       // Функция работы с файловой системой
  if(path.endsWith("/")) path += "index.html";                          // Если устройство вызывается по корневому адресу, то должен вызываться файл index.html (добавляем его в конец адреса)
  String contentType = getContentType(path);                            // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
  if(LittleFS.exists(path)){                                            // Если в файловой системе существует файл по адресу обращения
    File file = LittleFS.open(path, "r");                               //  Открываем файл для чтения
    size_t sent = HTTP.streamFile(file, contentType);                   //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
    Serial.print("\nSize of file "|| sent ||" ;");
    file.close();                                                       //  Закрываем файл
    return true;                                                        //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
  }
  return false;                                                         // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}