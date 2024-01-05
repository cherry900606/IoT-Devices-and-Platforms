#include <WiFi.h>
#include <LiquidCrystal_I2C.h> 
#include "time.h"


#define I2C_SDA 17
#define I2C_SCL 18

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// set your wifi’s ssid and password const char* ssid = "YOUR_SSID";
const char* ssid = "AndroidAPC113";
const char* password = "psei1964";

// the address of the  NTP Server 
const char* ntpServer = "time.stdtime.gov.tw";
const long gmtOffset_sec = 8*60*60; // Adjust the offset for your timezone 
const int daylightOffset_sec = 0; // Change the Daylight offset (in seconds)

// other pins & variables
int buttonPin1 = 19;
int buttonStatus = 0;

int const trigPin = 12; 
int const echoPin = 11; 
int Duration; 
int Distance; 

void printLocalTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() {
  Serial.begin(115200);

  // WiFi
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");


  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);


  // setting I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init(); // initialize the lcd 
  lcd.backlight(); // turn on backlight. 
  // Note. lcd.noBacklight() means turn off.

  // setting pin
  pinMode(buttonPin1, INPUT);
  pinMode(trigPin,OUTPUT); 
  pinMode(echoPin,INPUT);  

}

void loop() {
  delay(1000);
  printLocalTime();

  // button status
  if(digitalRead(buttonPin1) == HIGH) 
  {
    
    Serial.print("status:");
    Serial.println(buttonStatus);
    if(buttonStatus == 0)
      buttonStatus = 1;
    else
      buttonStatus = 0;
  }

  // distance
  digitalWrite(trigPin,LOW); 
  delay(2); 
  digitalWrite(trigPin,HIGH); //發射超音波 
  delay(10); 
  digitalWrite(trigPin,LOW); 
  Duration = pulseIn(echoPin,HIGH); //超音波發射到接收的時間 
  Distance = Duration*0.034/2; //計算距離(cm)
  delay(50); //delay 50ms
  Serial.println(Distance); 

  // backlight
  if(Distance < 10)
    lcd.backlight();
  else
    lcd.noBacklight();


  // time info and lcd
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  int hour = timeinfo.tm_hour;
  int min = timeinfo.tm_min;
  int sec = timeinfo.tm_sec;
  if(buttonStatus == 0)
  {
    lcd.clear(); // Clears the LCD screen
    lcd.setCursor(0,0);
    lcd.print(hour);
    lcd.print(":");
    lcd.print(min);
    lcd.print(":");
    lcd.print(sec);
    lcd.setCursor(3,1);
    lcd.print("GMT");
  }
  else
  {
    lcd.clear(); // Clears the LCD screen
    lcd.setCursor(0,0);
    lcd.print(hour+1);
    lcd.print(":");
    lcd.print(min);
    lcd.print(":");
    lcd.print(sec);
    lcd.setCursor(3,1);
    lcd.print("JST");
  }
}
