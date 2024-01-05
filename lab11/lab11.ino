// Author: 312552024 林琛琛
// 題目： 用超音波測距離，並定期上傳到 thingspeak。如果距離過短，則發出警示(LED發亮)。資料傳送完後休眠20秒。

// 連線用
#include <WiFi.h>
#include <HTTPClient.h>
char ssid[] = "CC";
char password[] = "wonf3444";
String url = "http://api.thingspeak.com/update?api_key=9JIQ21L8G89HHTDW";


// 測距離用
int const trigPin = 14; 
int const echoPin = 13; 
int Duration; 
int Distance; 

// LED 設定
gpio_num_t ledpin = GPIO_NUM_0; // 定義LED接腳

// sleep
#include <driver/rtc_io.h>
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  20        /* Time ESP32 will go to sleep (in seconds) */

void setup() { 
  Serial.begin(115200); 
  // pin 設定
  pinMode(trigPin,OUTPUT); 
  pinMode(echoPin,INPUT);  

  // 連線設定
  Serial.print("...");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connect...");

  // 睡眠時不斷電設定
  rtc_gpio_init(ledpin);
  rtc_gpio_set_direction(ledpin, RTC_GPIO_MODE_OUTPUT_ONLY);
  rtc_gpio_set_level(ledpin, LOW);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);


} 
void loop() { 
  // 測量距離
  digitalWrite(trigPin,LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin,HIGH); //發射超音波 
  delayMicroseconds(10); 
  digitalWrite(trigPin,LOW); 
  Duration = pulseIn(echoPin,HIGH); //超音波發射到接收的時間 
  Distance = Duration*0.034/2; //計算距離(cm)
  delay(50); //delay 50ms
  Serial.println(Distance); 

  // LED 發亮或暗
  if(Distance <=15)
    rtc_gpio_set_level(ledpin, HIGH);
  else
    rtc_gpio_set_level(ledpin, LOW);

  // 連線傳資料
  Serial.println("啟動網頁連線");
  HTTPClient http;
  //將距離以http get參數方式補入網址後方
  //http client取得網頁內容
  String url1 = url + "&field1=" + (int)Distance;
  http.begin(url1);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    //讀取網頁內容到payload
    String payload = http.getString();
    //將內容顯示出來
    Serial.print("更新網頁內容(距離)=");
    Serial.println(payload);
    if(payload == "0") // 如果封包沒發送成功
    {
      for(int i=0; i<5; i++) // 連續發送5次封包
      {
        http.begin(url1);
        Serial.println("重新傳送");
        delay(1000);
      }
    }
  } else {
    //讀取失敗
    Serial.println("網路傳送失敗");
  }
  http.end();

  // 睡覺設定
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
}
