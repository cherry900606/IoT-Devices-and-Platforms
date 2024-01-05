// 姓名：林琛琛
// 學號：312552024
// 題目：紅綠燈的加分題，將 LED 接線中的 GND 換成 5V


// 設置綠、紅、黃燈位置參數
int green_led = 9;
int red_led = 46;
int yellow_led = 10;

// 將 pin 設為輸出模式
void setup() {
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
}

// 執行程式主體
// 因為原本接地的線改接 5V，程式中所有的 LOW 與 HIGH 將會互換
void loop() {
  // 紅燈亮五秒鐘後熄滅一秒鐘
  digitalWrite(red_led, LOW);
  delay(5000); 
  digitalWrite(red_led, HIGH);
  delay(1000);

  // 綠燈亮五秒鐘後熄滅一秒鐘
  digitalWrite(green_led, LOW);
  delay(5000);
  digitalWrite(green_led, HIGH);
  delay(1000);

  // 讓黃燈不斷發光熄滅
  digitalWrite(yellow_led, HIGH);
  for (int i = 0; i < 10; i++)  // 重複以下行為 10 次
  {
    digitalWrite(yellow_led, !digitalRead(yellow_led)); // 如果原本亮，就熄滅；否則就發亮
    delay(100); // 停 0.1 秒鐘
  }
}
