#define BLYNK_PRINT Serial    
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "e5f4e326ea9c4958a73a9c4e2e961f7e";    //AuthToken vừa copy bên Blynk Project
char ssid[] = "BKIT_105_C6";  //Tên wifi muốn truy cập
char pass[] = "bkitc5c6";     //Mật khẩu(Nếu wifi không có mật khẩu thì để "" )

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
