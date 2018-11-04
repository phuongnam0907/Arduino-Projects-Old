#include <SimpleDHT.h>
#include <ESP8266WiFi.h>

#define pinDHT11 4

SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;

const char* ssid = "MyPhone: 01644414324";
const char* password = "123456789";


void setup() {
  Serial.begin(115200);
}

void loop() {
  
  if ((dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed!");
    delay(1000);
    return;
  }
  
  Serial.println((int)temperature);
  
  // DHT11 sampling rate is 1HZ.
  delay(1500);
}
