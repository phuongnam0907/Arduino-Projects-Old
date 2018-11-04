#include <EEPROM.h>  
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial  

char auth[] = "e5f4e326ea9c4958a73a9c4e2e961f7e";   
const char* ssid = WiFi.SSID().c_str();  
const char* pass = WiFi.psk().c_str();   

char ssidChar[31];
char passChar[31];
int length_ssid;
int length_pass;
int count_ssid = 0;
int count_pass = 0;
bool check_ssid = false;
bool check_pass = false;
int eeAddr = 0;
int recAddr = 0;

void eepRead();
void eepWrite();
void takeLength();
void SmartConfig();
void NormalConfig();
void Connect();
  
void setup() {
  Serial.begin(9600);
  EEPROM.begin(64);
  Connect();
}

void loop() {
  //Blynk.run();
  while(1){
    Blynk.run();
  }
}

void takeLength()
{
  count_ssid = 0;
  check_ssid = false;
  do {
    if (ssid[count_ssid] == '\0') check_ssid = true;
    else count_ssid++;
  }
  while (check_ssid == false);
  length_ssid = count_ssid;
  
  count_pass = 0;
  check_pass = false;
  do {
    if (ssid[count_pass] == '\0') check_ssid = true;
    else count_pass++;
  }
  while (check_pass == false);
  length_pass = count_pass;
}

void SmartConfig(){
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
 
  Blynk.begin(auth, ssid, pass);
  
}

void NormalConfig(){
  int count = 0;
  Serial.begin(115200);
  Serial.print("Connected ");
  WiFi.begin(ssidChar, passChar);
  Serial.println("");

 for (count = 0; count < 20; count++){
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Connect Successed");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Blynk.begin(auth, ssidChar, passChar);
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

void EEPROMclear(){
  for (int k = 0 ; k < 128 ; k++) {
    EEPROM.write(k, 0);
  }
}

void Connect(){
  //Read SSID and PASS 'old' from EEPROM
  //Normal connect in 5 seconds
  //If connect faied, change to SmartConfig
  //After access the Internet
  //Store new SSID and PASS to EEPROM
  eepRead();
  NormalConfig();
  if (WiFi.status() != WL_CONNECTED){
    SmartConfig();
    eepWrite();
  }
}

void eepRead(){
  length_ssid = EEPROM.read(0);
  length_pass = EEPROM.read(32);
  for (int i = 1; i < length_ssid + 1; i++){
    ssidChar[i] = EEPROM.read(i);
  }
  for (int i = 33; i < length_pass + 1; i++){
    passChar[i] = EEPROM.read(i);
  }
  //Serial.println(ssidChar);
  //Serial.println(passChar);
}

void eepWrite(){
  takeLength();
  EEPROM.write(0, count_ssid);
  for (int i = 1; i < count_ssid + 1; i++){
    eeAddr = i;
    EEPROM.write(eeAddr,ssid[i]);
  }
  eeAddr++;
  
  EEPROM.write(32, count_pass);
  for (int i = 33; i < count_pass + 1; i++){
    eeAddr = i;
    EEPROM.write(eeAddr,pass[i]);
  }
  eeAddr++;
  EEPROM.end();
}

