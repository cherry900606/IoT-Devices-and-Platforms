int ledPin = 13;       
int inputPin = 14;  
int brightness = 0;

void setup() {
  pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);     
}

void loop() {
  brightness = map(analogRead(inputPin), 0, 4095, 0, 255); // map sensor value from 0-4095 to 0-255
  Serial.println(analogRead(inputPin));
  Serial.println(brightness);
 
  analogWrite(ledPin, 255-brightness);  
}
