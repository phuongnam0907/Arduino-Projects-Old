#include "DHT.h"
#include <SPI.h>
#include <RH_RF95.h>

#define DHTPIN 3
#define DHTTYPE DHT11  
#define ST_START 1
#define ST_SEND_ID 2
#define ST_CONFIRM_ID 3
#define ST_WAIT_REQUEST 4
#define ST_SEND_DATA 5
#define ST_FINISH 6

RH_RF95 rf95;
DHT dht(DHTPIN, DHTTYPE);
int Status = -1;
int countREG = 150;         //Thời gian đợi của WAIT_REQ = countREG *100ms = 15s

void setup() 
{
  Status = ST_START;
  dht.begin();
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  rf95.init();
  Serial.begin(115200);
  //while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init()) Serial.println("init failed");
  rf95.setFrequency(434.0);
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  //rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128); 
  //rf95.setTxPower(7,false);
}

void SEND_ID(){
  delay(60);        //Thời gian delay trước khi gửi gói tin đi = 50 + ID. ID = 7 => 57ms
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  uint8_t data[] = "2";
  data[2] = 0;
  Serial.println((char*) data); //Serial.println(sizeof(data));
  rf95.send(data, sizeof(data));
  digitalWrite(8,HIGH);
  rf95.waitPacketSent();
}

boolean CONFIRM_ID(){
  if(rf95.waitAvailableTimeout(100)){
    uint8_t waitbuf[10] = "";
    uint8_t waitlen = sizeof(waitbuf);
    if (rf95.recv(waitbuf, &waitlen) & (waitbuf[0] == 'w' & waitbuf[1] == '2' & waitbuf[2] == 0)) return true;
    else return false;
  }
  else return false;
}

boolean WAIT_REQUEST(){
  if(rf95.waitAvailableTimeout(100)){
    uint8_t newbuf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t newlen = sizeof(newbuf); 
    if(rf95.recv(newbuf, &newlen) & (newbuf[0] == 'o' & newbuf[1] == 'k' & newbuf[2] == '2' & newbuf[3] == 0)) return true;
    else return false;
  }
  else return false;
}

void SEND_DATA(){
  uint8_t poiTem = 0;
  uint8_t poiHum = 0;
  uint8_t bufTem[8];
  uint8_t bufHum[8];
  char charTem[8];
  char charHum[8];
  float hum = dht.readHumidity();
  float tep = dht.readTemperature();
  if (isnan(hum) || isnan(tep)) 
  {
    hum = -1.0;
    tep = -1.0;
    Serial.println("Failed to read from DHT sensor!");
  }
  if (tep < 0.0) dtostrf(tep, 4,2, charTem);
  else if (tep < 10.0 && tep >= 0.0) dtostrf(tep, 3,2, charTem);
  else if (tep < 100.0 && tep >= 10.0) dtostrf(tep, 4,2, charTem);
  else if (tep >= 100.0) dtostrf(tep, 5,2, charTem);

  if (hum < 0.0) dtostrf(hum, 4,2, charHum);
  else if (hum < 10.0 && hum >= 0.0 ) dtostrf(hum, 3,2, charHum);
  else if (hum < 100.0) dtostrf(hum, 4,2, charHum);
  else dtostrf(hum, 5,2, charHum);
  
  int y = 0;
  bool ck = true;
  while (ck == true){ 
    bufTem[y] = charTem[y];
    if(bufTem[y] == 0) {poiTem = y; ck = false;}
    y++;
  }
  
  int yy = 0;
  bool ckk = true;
  while (ckk == true){ 
    bufHum[yy] = charHum[yy];
    if(bufHum[yy] == 0) {poiHum = yy; ckk = false;}
    yy++;
  }
  
  Serial.print("Temperature: ");
  Serial.println((char*)bufTem);
  Serial.print("Humidity: ");
  Serial.println((char*)bufHum);

  uint8_t sData[] = "#2,                ";
  for (int w = 0; w < poiTem; w++) sData[w+3] = bufTem[w];
  sData[poiTem + 3] = ',';
  for (int w = 0; w < poiHum; w++) sData[w+4+poiTem] = bufHum[w];
  sData[poiTem + poiHum + 4] = '*';
  uint8_t lensData = poiTem + poiHum + 6;
  sData[lensData-1] = 0;
  Serial.println((char*)sData);
  //Serial.println(lensData);
  rf95.send(sData, lensData);
  rf95.waitPacketSent();
  delay(100);
  Serial.println("send!!!!!!"); 
}

void COMPLETE(){        //code cũ 5000 => khoảng 1 phút thì gửi 1 lần
  rf95.waitAvailableTimeout(55000);        //Thời gian delay = 500ms * (số_ID_dự_đoán_trước + 1)
}                     //Theo thực nghiệm thì tốn khoảng 450~550ms (với code hiện tại) cho 1 quá trính nhận REQUEST và SEND_DATA của 1 client

void loop()
{
  boolean check_ST  = false;
  switch (Status){
    case ST_START:
        Status = ST_SEND_ID;
        break;
        
    case ST_SEND_ID:
        countREG = 250;
        SEND_ID();
        Status = ST_CONFIRM_ID;
        break;
        
    case ST_CONFIRM_ID:
        check_ST = CONFIRM_ID();
        if (check_ST == true) Status = ST_WAIT_REQUEST;
        else Status = ST_SEND_ID;
        break;
        
    case ST_WAIT_REQUEST:
        if (countREG == 0) Status = ST_SEND_ID;
        else {
          countREG--;
          check_ST = WAIT_REQUEST();
          if (check_ST == true) Status = ST_SEND_DATA;
        }
        break;
        
    case ST_SEND_DATA:
        SEND_DATA();
        Status = ST_FINISH;
        break;
      
    case ST_FINISH:
        COMPLETE();
        Status = ST_SEND_ID;
        break;
        
    default:
        Status = ST_START;
        break;
  }
}
