#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <SPI.h>

TaskHandle_t Task1;
TaskHandle_t Task2;
SemaphoreHandle_t baton;

char inByte[41], data[41];
String sri[41], kode01, kode02, kode03;
int i, a;

char ssid[] = "03aims";
char pass[] = "12345678Ab";
const char *host = "aktualisasi-bbspjppi.com";

void codeForTask2(void *pvParameters)
{
  for (;;)
  {
    xSemaphoreTake(baton, portMAX_DELAY);
    while (Serial2.available() > 0)
  {
    for (i = 1; i < 42; i++)
    {
      inByte[i] = Serial2.read();
      data[i] = inByte[i];
      if (isSpace(data[i]))
      {
        data[i] = '\0';
      }
      sri[i] = String(data[i]);
      Serial.print(data[i]);
    }
    kode01 = sri[4] + sri[5] + sri[6] + sri[7] + sri[8] + sri[9] + sri[10] + sri[11] + sri[12];
    kode02 = sri[14] + sri[15] + sri[16] + sri[17] + sri[18] + sri[19] + sri[20] + sri[21] + sri[22] + sri[23] + sri[24];
    kode03 = sri[27] + sri[28] + sri[29] + sri[30] + sri[31] + sri[32] + sri[33] + sri[34] + sri[35] + sri[36] + sri[37] + sri[38];
    Serial.println();
    if (kode02 != NULL)
    {
      kode02 = kode02;
    }
    else
    {
      kode02 = String(a);
    }
    if (kode03 != NULL)
    {
      kode03 = kode03;
    }
    else
    {
      kode03 = String(a);
    }
    Serial.print("Kode 01: ");
    Serial.println(kode01);
    Serial.print("Kode 02: ");
    Serial.println(kode02);
    Serial.print("Kode 03: ");
    Serial.println(kode03);
  }
    xSemaphoreGive(baton);
    delay(50);
  }
}

void codeForTask1(void *pvParameters)
{
 for (;;)
  {
    xSemaphoreTake(baton, portMAX_DELAY);
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort))
    {
        Serial.println("Tidak terkoneksi ke server");
        return;
    }
    client.print(String("GET /flow.php?")); 
    client.print("&kode01=");
    client.print(kode01);
    client.print("&kode02=");
    client.print(kode02);
    client.print("&kode03=");
    client.print(kode03);
    client.print(" HTTP/1.1\r\nHost: host\r\nConnection: keep-alive\r\nAccept: */*\r\n\r\n");
    delay(8000);
    client.stop();
    xSemaphoreGive(baton);
  }
}
void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8E2);
    a = 0;

      WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Tersambung ke jaringan");

    baton = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(codeForTask1, "Task1", 10000, NULL, 1, NULL, 0);
    delay(100);
    xTaskCreatePinnedToCore(codeForTask2, "Task2", 10000, NULL, 1, NULL, 1);
    delay(100);
    vTaskDelete(NULL);
  }

void loop()
{
  //not used
}