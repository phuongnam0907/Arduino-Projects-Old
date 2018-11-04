#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "e5f4e326ea9c4958a73a9c4e2e961f7e";   
const char* ssid = WiFi.SSID().c_str();  
const char* pass = WiFi.psk().c_str();    

void setup()
{
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  delay(500);

  WiFi.beginSmartConfig();
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.println(WiFi.smartConfigDone());
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(ssid);
  Serial.println(pass);
  
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);    
}

void loop()
{
  Blynk.run();
}

