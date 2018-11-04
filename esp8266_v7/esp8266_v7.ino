#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "e5f4e326ea9c4958a73a9c4e2e961f7e";   
const char* ssid = WiFi.SSID().c_str();  
const char* pass = WiFi.psk().c_str();   
void Connect();
void NormalConfig();
void SmartConfig();
  
void setup() {
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);
  Serial.begin(115200);
  Connect();
  Serial.begin(9600);
  //Blynk.begin(auth, ssid, pass);
}

void loop() {
  while(1){
    Blynk.run();
  }
}


void SmartConfig(){
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
 
  Blynk.begin(auth, ssid, pass);
  
}

void NormalConfig(){
  int count = 0;
  WiFi.begin(ssid, pass);
  Serial.print("Connected ");
  Serial.println("");

 for (count = 0; count < 50; count++){
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Connect Successed");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Blynk.begin(auth, ssid, pass);
  }
  else
  {
    delay(50);
    Serial.println("");
    Serial.println("Connect Failed");
    Serial.println("Smart Config...");
    delay(50);
  }
}

void Connect(){
  NormalConfig();
  while (WiFi.status() != WL_CONNECTED){
    SmartConfig();
  }
  if (WiFi.status() == WL_CONNECTED){
    digitalWrite(14, LOW);
  }
  else
  {
    digitalWrite(14,HIGH);
  }
}

