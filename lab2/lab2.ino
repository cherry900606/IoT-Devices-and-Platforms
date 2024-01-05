// pin setup & variable declare
int const trigPin = 12; 
int const echoPin = 11; 
int ledPin = 14;

int Duration; 
int Distance; 

// set up output & input
void setup() { 
  Serial.begin(9600); 
  pinMode(trigPin,OUTPUT); 
  pinMode(echoPin,INPUT);  
  pinMode(ledPin, OUTPUT);

} 
void loop() { 
  // 計算距離
  digitalWrite(trigPin,LOW); 
  delay(2); 
  digitalWrite(trigPin,HIGH); //發射超音波 
  delay(10); 
  digitalWrite(trigPin,LOW); 
  Duration = pulseIn(echoPin,HIGH); //超音波發射到接收的時間 
  Distance = Duration*0.034/2; //計算距離(cm)
  delay(50); //delay 50ms
  Serial.println(Distance); 


  // 10 ~ 20 公分時，低速閃爍LED 
  if(Distance > 10 && Distance < 20)
  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
  // 低於10 公分時，高速閃爍LED 
  else if(Distance < 10)
  {
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
    delay(50);
  }
}
