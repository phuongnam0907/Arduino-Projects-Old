#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "SoftwareSerial.h"
SoftwareSerial sim808(2,3);    

String SDT="0981662961";
String textOK = "Noise is OK  | (dB): ";
String textAr = "Noise is ARLET  | (dB): ";
String textDa = "Noise is DANGEROUS  | (dB): ";
int run;
int buttonPin,pdb;
int db;
const int sampleWindow = 100;       
unsigned int sample;
int sensorValue = 0;
int chkNet = 12;
int isSend = 0;
//Use GPRS of sim800a to crate a wireless connection

// initialize the library with the numbers of the interface pins

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(4, OUTPUT);
  pinMode(chkNet, INPUT);
  sim808.begin(9600);
  
  lcd.begin(16, 2);
  
  introduce();
  checkWifi();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NOISE  |  STATUS");


  run = 0;
  buttonPin = 7;
  pinMode(buttonPin, INPUT_PULLUP);
  delay(100);
}

void loop() 
{
  while(digitalRead(buttonPin)==HIGH);
  CodeChinh();
}



//**********************//
//   Chuong trinh con   //
//**********************//

void CodeChinh()
{
    sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  unsigned long startMillis= millis();                   // Start of sample window
  float peakToPeak = 0;                                  // peak-to-peak level
  unsigned int signalMax = 0;                            //minimum value
  unsigned int signalMin = 1024;                         //maximum value
                                                          // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
     sample = analogRead(0);                             //get reading from microphone
     if (sample < 1024)                                  // toss out spurious readings
     {
        if (sample > signalMax)
        {
           signalMax = sample;                           // save just the max levels
        }
        else if (sample < signalMin)
        {
           signalMin = sample;                           // save just the min levels
        }
     }
  }
  peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
  db = map(peakToPeak,10,650,500,799); 
  pdb = db %10;
  lcd.setCursor(0, 1);
  lcd.print(db/10);
  lcd.print(".");
  lcd.print(pdb);
  lcd.setCursor(5,1);
  lcd.print("dB|");  
  lcd.setCursor(10,1);
  Stt();
  if (digitalRead(5) == HIGH)
  {
    sendMess();
  }
  delay(200);
};

void Stt()
{
  lcd.print("    ");
  if (db < 650)
  {
    lcd.setCursor(14,1);
    lcd.print("OK");
    isSend = 0;
  }
  else
  {
    if (db >= 650 && db < 790)
    {
      lcd.setCursor(11,1);
      lcd.print("ARLET");
      isSend = 0;
    }
    else
    {
      if (db >= 790)
      {
        lcd.setCursor(10,1);
        lcd.print("DANGER");
        isSend++;
        if (isSend > 3) sendMess();
      }
    }
  }
}


void introduce()
{
  lcd.print("Ho va Ten:");
  lcd.setCursor(0,1);
  lcd.print("   LE PHUONG NAM");
  delay(2000);
  lcd.clear();
  
  lcd.print("MSSV     1512067");
  lcd.setCursor(0,1);
  lcd.print("Phone 0902740087");
  delay(2000);
  lcd.clear();

  lcd.print("Project:");
  lcd.setCursor(0,1);
  lcd.print(" DO CUONG DO AM");
  delay(2000);
  lcd.clear();
}

void sendMess()
{
  lcd.setCursor(9,1);
  lcd.print("Sending");
  delay(1000);
  lcd.setCursor(8,1);
  lcd.print("        ");

  //Use SIM800A to send a message
  at("AT",10);
  at("AT+CMGF=1",10);
  at("AT+CSCS=\"GSM\"",10);
  at("AT+CMGS=\"" + SDT+"\"",20);
  
  if (db < 650)
  {
    float bdb = (float)db / 10.0;
    at(textOK + bdb,10);
  }
  else
  {
    if (db >= 650 && db < 790)
    {
      float bdb = (float)db / 10.0;
      at(textAr + bdb,10);
    }
    else
    {
      if (db >= 790)
      {
        float bdb = (float)db / 10.0;
        at(textDa + bdb,10);
      }
    }
  }

  sim808.write(26); 
  at("AT+CMGDA=\"DEL ALL\"",10);
};

void checkWifi(){
  int sec_cnt = 0;
  while ((digitalRead(chkNet) == HIGH) & (sec_cnt < 100))
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connect");
    lcd.setCursor(7,0);
    lcd.print("    ");
    delay(500);
    lcd.setCursor(7,0);
    for (int i = 0; i < 4; i++){
      lcd.print(".");
      sec_cnt++;
      delay(500);
    }
  }
  if (sec_cnt < 100)
  {
    lcd.setCursor(7,1);
    lcd.print("SUCCESSED");
  }
  else
  {
    lcd.setCursor(10,1);
    lcd.print("FAILED");
  }
  
  delay(500);
}

void at(String _atcm,unsigned long _dl){
  sim808.print(_atcm+"\r\n");
  delay(_dl);
}
