#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 
#include <SimpleDHT.h>


#define I2C_SDA 17
#define I2C_SCL 18

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int pinDHT11 = 9;
SimpleDHT11 dht11(pinDHT11);

int buttonPin1 = 13; // set hour, minutes, seconds
int buttonPin2 = 12; // increase value


int status = 0; // 0 = hour, 1 = minute, 2 = second
int hour = 0, minute = 0, second = 0;

void setup() {
  // setting I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init(); // initialize the lcd 
  lcd.backlight(); // turn on backlight. 
  // Note. lcd.noBacklight() means turn off.

  // setting button pin
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  digitalWrite(buttonPin1, HIGH);
  digitalWrite(buttonPin2, HIGH);
}

void loop() {
  // First part: temperature & humidity
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(err);
    delay(1000);
    return;
  }


  // Second part: reading button input
  if(digitalRead(buttonPin1) == LOW) 
  {
    status += 1;
    Serial.print("buttonPin1(status) = ");
    Serial.println(status);
    if(status == 3)
      status = 0;
    delay(500);
  }
  if(digitalRead(buttonPin2) == LOW) 
  {
    if(status == 1)
    {
      hour += 1;
      Serial.print("buttonPin1(hour) = ");
      Serial.println(hour);
    }
    else if(status == 2)
    {
      minute += 1;
      Serial.print("buttonPin1(minute) = ");
      Serial.println(minute);
    }
    else if(status == 0)
    {
      second += 1;
      Serial.print("buttonPin1(second) = ");
      Serial.println(second);
    }
    delay(500);
  }
  second += 1;
  if(second == 60)
  {
    second = 0;
    minute += 1;
  }
  if(minute == 60)
  {
    minute = 0;
    hour += 1;
  }


  // Third part: Show info on the sreen
  lcd.clear(); // Clears the LCD screen
  lcd.setCursor(0,0);
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  lcd.setCursor(3,1);
  lcd.print((int) temperature);
  lcd.write(B11011111);
  lcd.print("c");
  delay(800);
}

