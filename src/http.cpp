#include <Arduino.h>
#include <pumper.h>

void httpInit()
{
        // Обработка HTTP-запросов
        HTTP.on("/relay_switch", []()
                { HTTP.send(200, "text/plain", relay_switch()); });

        HTTP.on("/relay_status", []()
                { HTTP.send(200, "text/plain", relay_status()); });

        HTTP.on("/relay_start", []()
                {
                        int time = HTTP.arg("time").toInt();
                        int delay = HTTP.arg("delay").toInt();
                        relay_start(time, delay);
                        HTTP.send(200, "text/plain", HTTP.arg("time") + "--" + HTTP.arg("delay") + "--" + HTTP.args() );
        });
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

String getContentType(String filename)
{ // Функция, возвращающая необходимый заголовок типа содержимого в зависимости от расширения файла
        if (filename.endsWith(".html"))
                return "text/html";
        else if (filename.endsWith(".css"))
                return "text/css";
        else if (filename.endsWith(".js"))
                return "application/javascript";
        else if (filename.endsWith(".png"))
                return "image/png";
        else if (filename.endsWith(".jpg"))
                return "image/jpeg";
        else if (filename.endsWith(".gif"))
                return "image/gif";
        else if (filename.endsWith(".ico"))
                return "image/x-icon";
        return "text/plain";
}

bool handleFileRead(String path)
{ // Функция работы с файловой системой
        if (path.endsWith("/"))
                path += "index.html";
        String contentType = getContentType(path); // С помощью функции getContentType (описана ниже) определяем по типу файла (в адресе обращения) какой заголовок необходимо возвращать по его вызову
        if (LittleFS.exists(path))
        {                                                         // Если в файловой системе существует файл по адресу обращения
                File file = LittleFS.open(path, "r");             //  Открываем файл для чтения
                size_t sent = HTTP.streamFile(file, contentType); //  Выводим содержимое файла по HTTP, указывая заголовок типа содержимого contentType
                file.close(); //  Закрываем файл
                return true;  //  Завершаем выполнение функции, возвращая результатом ее исполнения true (истина)
        }
        return false; // Завершаем выполнение функции, возвращая результатом ее исполнения false (если не обработалось предыдущее условие)
}