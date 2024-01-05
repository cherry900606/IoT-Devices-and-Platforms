// 姓名：林琛琛
// 學號：312552024
// 題目：紅燈5秒，綠燈5秒，黃燈2秒，其中黃燈閃爍速率為每0.1秒交替發光與熄滅


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
void loop() {
  // 紅燈亮五秒鐘後熄滅一秒鐘
  digitalWrite(red_led, HIGH);
  delay(5000);
  digitalWrite(red_led, LOW);
  delay(1000);

  // 綠燈亮五秒鐘後熄滅一秒鐘
  digitalWrite(green_led, HIGH);
  delay(5000); 
  digitalWrite(green_led, LOW);
  delay(1000);

  // 讓黃燈不斷發光熄滅
  digitalWrite(yellow_led, LOW);
  for (int i = 0; i < 10; i++) // 重複以下行為 10 次
  {
    digitalWrite(yellow_led, !digitalRead(yellow_led)); // 如果原本亮，就熄滅；否則就發亮
    delay(100); // 停 0.1 秒鐘
  }
}
