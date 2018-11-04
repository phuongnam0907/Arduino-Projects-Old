/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include <SimpleDHT.h>

#define pinDHT11 4

SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;

// Replace with your network details
const char* ssid = "MyPhone: 01644414324";
const char* password = "123456789";

// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
            if ((dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
              Serial.print("Read DHT11 failed!");
              delay(1000);
              return;
            }
              Serial.print("Temperature: ");
              Serial.print((int)temperature);
              Serial.print(" *C ");
           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature and humidity
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>ESP8266 - Temperature and Humidity</h1><h3>Temperature in Celsius: ");
            client.println((int)temperature);
            client.println("*C</h3><h3>");  
    }  
    // closing the client connection
    delay(1000);
    client.stop();
    Serial.println("Client disconnected.");
}   
