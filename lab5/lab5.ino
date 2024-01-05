/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <SimpleDHT.h>


// Replace with your network credentials
const char* ssid     = "312552024"; //REPLACE_WITH_YOUR_SSID
const char* password = "123456789";  //REPLACE_WITH_YOUR_PASSWORD

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;


// Set your Static IP address 
IPAddress local_IP(192, 168, 4, 1); 

// Set your Gateway IP address 
IPAddress gateway(192, 168, 50, 1); 
IPAddress subnet(255, 255, 255, 0); 


// Variable for lab
int tempMode = 1;
byte temperature = 0;
byte humidity = 0;

int pinDHT11 = 9;
SimpleDHT11 dht11(pinDHT11);

unsigned long previousMillis = 0; // will store last time LED was updated
const long interval = 10000; // interval at which to blink (milliseconds)

 

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAPConfig(local_IP, local_IP, subnet);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  
  int err = SimpleDHTErrSuccess;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    // start working...
    Serial.println("=================================");
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); 
      Serial.println(err);
      delay(1000);
      return;
    }
    Serial.print("Humidity = ");
    Serial.print((int) humidity);
    Serial.print("%, ");
    Serial.print("Temperature = ");
    Serial.print((int) temperature);
    Serial.println("C");
    delay(3000); 
    
  }

  


  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /temp/mode1") >= 0) {
              Serial.println("Temp to 華式");
              tempMode = 1;
            } 
            else if (header.indexOf("GET /temp/mode2") >= 0) {
              Serial.println("Temp to 攝式");
              tempMode = 2;
            } 
            
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            client.print("<p>Temperature:  </p>");
            if(tempMode == 2)
            {
              client.print(temperature);
            client.println("C</p>");
            }
            else
            {
              client.print(temperature * 5 / 9 + 32);
            client.println("F</p>");
            }

            client.print("<p>Humidity:  </p>");
            client.print(humidity);
            client.println("%</p>");

            client.println("<p><a href=\"/temp/mode1\"><button class=\"button\">Fahrenheit</button></a></p>");
            client.println("<p><a href=\"/temp/mode2\"><button class=\"button\">Celsius</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } 
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}