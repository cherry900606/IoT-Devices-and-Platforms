#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 

#define I2C_SDA 17
#define I2C_SCL 18

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// all the words
byte I_B[8]= {
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111 };

  byte I_W[8]= {
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000 };

  byte love_B[8] = {
    B00000,
    B01010,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000};

  byte love_W[8] = {
    B11111,
    B10101,
    B00000,
    B00000,
    B10001,
    B11011,
    B11111,
    B11111};

  byte N_B[8] = {
    B10001,
    B11001,
    B10101,
    B10011,
    B10001,
    B00000,
    B00000,
    B00000};
  
  byte N_W[8] = {
    B01110,
    B00110,
    B01010,
    B01100,
    B01110,
    B11111,
    B11111,
    B11111};

  byte Y_B[8] = {
    B10001,
    B01010,
    B00100,
    B00100,
    B00100,
    B00000,
    B00000,
    B00000};

  byte Y_W[8] = {
    B01110,
    B10101,
    B11011,
    B11011,
    B11011,
    B11111,
    B11111,
    B11111};

  byte C_B[8] = {
    B11111,
    B10000,
    B10000,
    B10000,
    B11111,
    B00000,
    B00000,
    B00000};

  byte C_W[8] = {
    B00000,
    B01111,
    B01111,
    B01111,
    B00000,
    B11111,
    B11111,
    B11111};

  byte U_B[8] = {
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B00000,
    B00000,
    B00000};

  byte U_W[8] = {
    B01110,
    B01110,
    B01110,
    B01110,
    B00000,
    B11111,
    B11111,
    B11111};

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init(); // initialize the lcd 
  lcd.backlight(); // turn on backlight. 
  
  
}

void loop() {

  // show the black word first
  lcd.createChar(0, I_B);
  lcd.createChar(1, love_B);
  lcd.createChar(2, N_B);
  lcd.createChar(3, Y_B);
  lcd.createChar(4, C_B);
  lcd.createChar(5, U_B);

  lcd.clear(); // Clears the LCD screen
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);

  delay(1000);

  // then show the white word
  lcd.createChar(0, I_W);
  lcd.createChar(1, love_W);
  lcd.createChar(2, N_W);
  lcd.createChar(3, Y_W);
  lcd.createChar(4, C_W);
  lcd.createChar(5, U_W);

  lcd.clear(); // Clears the LCD screen
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);

  delay(1000);
}
