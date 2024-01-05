// Author: 312552024 林琛琛
// 加分題： 一個 channel 記錄兩個以上的 ESP32 的溫溼度感測資料，並且能夠分辨是哪塊板子上傳的資料
#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>

//網路連接
char ssid[] = "AndroidAPC113";
char password[] = "psei1964";

//thingspeak連接
String url = "http://api.thingspeak.com/update?api_key=X0Z57N57HKPW5Z4Q";
int pinDHT11 = 17;//DHT11接在腳位pin 17
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
  // 一次傳送溫溼度訊息
  //http client取得網頁內容
  String url1 = url + "&field3=" + (int)temperature+"&field4=" + (int)humidity;
  http.begin(url1);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    //讀取網頁內容到payload
    String payload = http.getString();
    //將內容顯示出來
    Serial.print("更新網頁內容(溫度)=");
    Serial.println(payload);
    if(payload == "0") // 如果封包沒發送成功
    {
      for(int i=0; i<10; i++) // 連續發送10次封包
      {
        http.begin(url1);
        Serial.println("重新傳送");
      }
    }
  } else {
    //讀取失敗
    Serial.println("網路傳送失敗");
  }
  http.end();
  
  delay(15000);//休息15秒

}