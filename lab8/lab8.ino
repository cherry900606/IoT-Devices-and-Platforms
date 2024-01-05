// Author: 312552024 林琛琛
// 題目：超音波測距(特定範圍內) 與 Line 訊息傳送
// Note: 兩人合作，另一份 code 為另一同學實作從 thingspeak 抓資料下來與燈泡閃爍的功能
#include <WiFi.h>
#include <HTTPClient.h>


// 測距相關參數
int const trigPin = 13; 
int const echoPin = 12; 
int Duration; 
int Distance; 

//網路連接
char ssid[] = "AndroidAPC113";
char password[] = "psei1964";
//thingspeak連接
String url = "http://api.thingspeak.com/update?api_key=X0Z57N57HKPW5Z4Q"; // write key = X0Z57N57HKPW5Z4Q



// line 參數
WiFiClientSecure client;//網路連線物件
char host[] = "notify-api.line.me";//LINE Notify API網址
String Linetoken = "3JsRoGPmwmEti39iAKERMgHBNPJglVA0H5773md1UMz";


void setup() {
  Serial.begin(9600); 
  // 測距腳位設定
  pinMode(trigPin,OUTPUT); 
  pinMode(echoPin,INPUT);  
  // 網路連線
  Serial.print("...");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connect...");

  //連線成功，顯示取得的IP
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  client.setInsecure();//ESP32核心 1.0.6以上
}

void loop() {
  // 測距
  digitalWrite(trigPin,LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin,HIGH); //發射超音波 
  delayMicroseconds(10); 
  digitalWrite(trigPin,LOW); 
  Duration = pulseIn(echoPin,HIGH); //超音波發射到接收的時間 
  Distance = Duration*0.034/2; //計算距離(cm)
  delay(50); //delay 50ms
  Serial.println(Distance); 

  // 如果感測距離內有物體，上傳到 thingspeak
  if(Distance >= 5 && Distance <= 15) // 感測範圍：介於 5 ~ 15 之間
  {
    // 傳line給自己
    String message = "檢測環境發生異常：";
    message += "\n距離=" + String(((int)Distance));
    if (client.connect(host, 443)) {
      int LEN = message.length();
      //傳遞POST表頭
      String url = "/api/notify";
      client.println("POST " + url + " HTTP/1.1");
      client.print("Host: "); client.println(host);
      //權杖
      client.print("Authorization: Bearer "); 
      client.println(Linetoken);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: "); 
      client.println( String((LEN + 8)) );
      client.println();      
      client.print("message="); 
      client.println(message);
      client.println();
      //等候回應
      delay(2000);
      String response = client.readString();
      //顯示傳遞結果
      Serial.println(response);
      client.stop(); //斷線，否則只能傳5次
    }
    else {
      //傳送失敗
      Serial.println("connected fail");
    }


    //上傳到thingspeak
    String url1 = url + "&field1=" + (int)Distance;
    Serial.print("啟動網頁連線，距離=");
    Serial.println(Distance);
    HTTPClient http;
    http.begin(url1);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)      {
      //讀取網頁內容到payload
      String payload = http.getString();
      //將內容顯示出來
      Serial.print("更新網頁內容(距離)payload=");
      Serial.println(payload);
      if(payload == "0") // 沒傳成功的話，重新傳10次
      {
        for(int i=0; i<10; i++) // 連續發送10次封包
        {
          http.begin(url1);
          int httpCode = http.GET();
          String payload = http.getString();
          Serial.print("重新傳送，更新網頁內容(距離)payload=");
          Serial.println(payload);
          if(payload != "0") // 有成功就離開迴圈
            break;
          delay(1000); // delay 1s
        }
      }
    } else {
      //讀取失敗
      Serial.println("網路傳送失敗");
    }
    http.end();
    
    delay(20000);//休息20秒

  }

}
