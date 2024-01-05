// Author: 312552024 林琛琛
// 題目： 光敏電阻每20秒感測一次；溫溼度每40秒感測一次。相同數值則不上傳，持續最多十次。

#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>

//網路連接
char ssid[] = "AndroidAPC113";
char password[] = "psei1964";

//thingspeak連接
String url = "http://api.thingspeak.com/update?api_key=9JIQ21L8G89HHTDW";
int pinDHT11 = 17; //DHT11接在腳位pin 17
int pinPhoto = 14; //光敏電阻腳位
//---------------------------------------------------------
SimpleDHT11 dht11(pinDHT11);//宣告SimpleDHT11物件

// 紀錄上一次的數值，以及維持相同的次數
int lastTemp = 0;
int lastHumidity = 0;
int lastPhoto = 0;
int countTemp = 0;
int countHumidity = 0;
int countPhoto = 0;

void setup() {
  //網路連接
  Serial.begin(115200);
  Serial.print("...");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connect...");
}

void loop() {
  Serial.print("使用核心編號：");
  Serial.println(xPortGetCoreID());
  //嘗試讀取溫濕度內容
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("溫度計讀取失敗，錯誤碼="); Serial.println(err); delay(1000);
    return;
  }
  //讀取成功，將溫濕度顯示在序列視窗
  Serial.print("溫度計讀取成功: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");
  //開始傳送到thingspeak
  Serial.println("啟動網頁連線");
  HTTPClient http;
  //將溫度及濕度以http get參數方式補入網址後方

  // 溫度讀取寫入
  if((int)temperature == lastTemp && countTemp < 10) // 跟上次一樣，不上傳
  {
    countTemp += 1; // counter++
    Serial.print("網頁內容(溫度)不更新");
    Serial.println(countTemp);
  }
  else // 上傳
  {
    countTemp = 0;
    lastTemp = (int)temperature;
    //http client取得網頁內容
    String url1 = url + "&field1=" + (int)temperature;
    http.begin(url1);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)      {
      //讀取網頁內容到payload
      String payload = http.getString();
      //將內容顯示出來
      Serial.print("更新網頁內容(溫度)=");
      Serial.println(payload);
    } else {
      //讀取失敗
      Serial.println("網路傳送失敗");
    }
    http.end();
  }

  // 濕度讀取寫入
  if((int)humidity == lastHumidity && countHumidity < 10)
  {
    countHumidity += 1;
    Serial.print("網頁內容(濕度)不更新");
    Serial.println(countHumidity);
  }
  else
  {
    countHumidity = 0;
    lastHumidity = (int)humidity;
    //http client取得網頁內容
    String url1 = url + "&field2=" + (int)humidity;
    http.begin(url1);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)      {
      //讀取網頁內容到payload
      String payload = http.getString();
      //將內容顯示出來
      Serial.print("更新網頁內容(濕度)=");
      Serial.println(payload);
    } else {
      //讀取失敗
      Serial.println("網路傳送失敗");
    }
    http.end();
  }

  // 光敏電阻
  if((int)analogRead(pinPhoto) == lastPhoto && countPhoto < 10)
  {
    countPhoto += 1;
    Serial.print("網頁內容(光敏)不更新");
    Serial.println(countPhoto);
  }
  else
  {
    countPhoto = 0;
    lastPhoto = (int)analogRead(pinPhoto);
    //http client取得網頁內容
    String url1 = url + "&field3=" + (int)analogRead(pinPhoto);
    http.begin(url1);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)      {
      //讀取網頁內容到payload
      String payload = http.getString();
      //將內容顯示出來
      Serial.print("更新網頁內容(光敏)=");
      Serial.println(payload);
    } else {
      //讀取失敗
      Serial.println("網路傳送失敗");
    }
    http.end();
  }

  delay(20000);//休息20秒

  // 光敏電阻每20秒感測一次
  if((int)analogRead(pinPhoto) == lastPhoto && countPhoto < 10)
  {
    countPhoto += 1;
    Serial.print("網頁內容(光敏)不更新");
    Serial.println(countPhoto);
  }
  else
  {
    countPhoto = 0;
    lastPhoto = (int)analogRead(pinPhoto);
    //http client取得網頁內容
    String url1 = url + "&field3=" + (int)analogRead(pinPhoto);
    http.begin(url1);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)      {
      //讀取網頁內容到payload
      String payload = http.getString();
      //將內容顯示出來
      Serial.print("更新網頁內容(光敏)=");
      Serial.println(payload);
    } else {
      //讀取失敗
      Serial.println("網路傳送失敗");
    }
    http.end();
  }

  // 再次 delay 20 秒，達成溫濕度40秒一次、光敏電阻20秒一次
  delay(20000);//休息20秒
}