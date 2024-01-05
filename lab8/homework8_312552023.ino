/*
  學號：312552023
  姓名：蔣秉錕
  題目：
    當 ultrasonic sensor 在預設的感測範圍內感測到有物體時，會將感測到的資料（距離）通過ThingSpeak傳送給另一個ESP32並使用LED警示（閃爍）
    ，同時通過Line傳送給使用者。感測距離設定為 15 cm。本程式實作資料接收方。
*/
#include <WiFi.h>
#include <ArduinoJson.h>
WiFiServer server(80); 

// 紀錄上一筆資料的 entry id
String prev_entry_id = "0";

// 設定資料讀取網站
char   host[] = "api.thingspeak.com"; // ThingSpeak address
const int httpPort = 80;

// 設定頻道跟 READ API KEY
String CHANNELID = "2346744";  // Thingspeak CHANNEL ID
String READAPIKEY = "39FVKKNZTROU7I0K";  // READ APIKEY for the CHANNEL_ID
 
// 設定 WiFi 帳號密碼
const char *ssid     = "AndroidAPC113"; 
const char *password = "psei1964";

WiFiClient client; 
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server

int ledPin = 2;

void setup(){
  pinMode(ledPin, OUTPUT); // 設定 led 腳位模式
  Serial.begin(115200); // 設定傳輸速度
  WiFi.mode(WIFI_STA); // 設定 esp32 WiFi 模式

  // 執行連線
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connect...");
}

void loop(){
  RetrieveTSChannelData();
  delay(15000); //Wait before we request again
}

void RetrieveTSChannelData() {  // Receive data from Thingspeak
  static char responseBuffer[3*1024]; // Buffer for received data
  client = server.available(); 
  if (!client.connect(host, httpPort)) { 
    Serial.println("connection failed"); 
    return; 
  } 
  String url = "/channels/" + CHANNELID; // Start building API request string
  url += "/fields/1.json?api_key=" + READAPIKEY + "&results=1";  // 5 is the results request number, so 5 are returned, 1 would return the last result received
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  while (!skipResponseHeaders());                      // Wait until there is some data and skip headers
  while (client.available()) {                         // Now receive the data
    String line = client.readStringUntil('\n');
    if ( line.indexOf('{',0) >= 0 ) {                  // Ignore data that is not likely to be JSON formatted, so must contain a '{'
      Serial.println(line);                            // Show the text received
      line.toCharArray(responseBuffer, line.length()); // Convert to char array for the JSON decoder
      decodeJSON(responseBuffer);                      // Decode the JSON text
    }
  }
  client.stop();
}

bool skipResponseHeaders() { 
  char endOfHeaders[] = "\r\n\r\n"; // HTTP headers end with an empty line 
  client.setTimeout(HTTP_TIMEOUT); 
  bool ok = client.find(endOfHeaders); 
  if (!ok) { Serial.println("No response or invalid response!"); } 
  return ok; 
} 

// 檢測到異常時，讓 Led 閃爍
void blinkLED(int distance) {
  if(distance < 15) {
    Serial.println("Abnormal Detect!!!");
    for(int i = 0; i < 10; i++) {
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
    }
  }
}

bool decodeJSON(char *json) {
  StaticJsonBuffer <3*1024> jsonBuffer;
  char *jsonstart = strchr(json, '{'); // Skip characters until first '{' found and ignore length, if present
  if (jsonstart == NULL) {
    Serial.println("JSON data missing");
    return false;
  }
  json = jsonstart;
  JsonObject& root = jsonBuffer.parseObject(json); // Parse JSON
  if (!root.success()) {
    Serial.println(F("jsonBuffer.parseObject() failed"));
    return false;
  }
  JsonObject& root_data = root["channel"]; // Begins and ends within first set of { }
  String id   = root_data["id"];
  String name = root_data["name"];
  String field1_name = root_data["field1"];
  String datetime    = root_data["updated_at"];
  Serial.println("\n\n Channel id: "+id+" Name: "+ name);
  Serial.println(" Readings last updated at: "+datetime);
  
  // 只讀取最後一筆資料
  for (int result = 0; result < 1; result++){
    JsonObject& channel = root["feeds"][result]; // Now we can read 'feeds' values and so-on
    String entry_id     = channel["entry_id"];
    String field1value  = channel["field1"];
    Serial.print(" Field1 entry number ["+entry_id+"] had a value of: ");
    Serial.println(field1value);
    // 將字串型態的距離轉換為整數
    int distance = field1value.toInt();
    // 如果是同一筆資料，就不要進行閃爍
    if(entry_id != prev_entry_id) {
      blinkLED(distance);
      // 更新 id 紀錄
      prev_entry_id = entry_id;
    }
  }
  return true; // 使 board 不要重新啟動
}
