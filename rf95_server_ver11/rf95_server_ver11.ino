#include <SPI.h>
#include <RH_RF95.h>
#include <Queue.h>

#define RFM95_CS 10
#define RFM95_INT 2

#define ST_START 1
#define ST_WAIT_ID 2
#define ST_RESP_ID 3
#define ST_ADD_QUEUE 4
#define ST_COUNT_ID 5
#define ST_REQ_ID_DATA 6
#define ST_WAIT_ID_DATA 7
#define ST_FINISH 8

int Status = -1;
uint8_t testqueue[256] = "";
boolean checkStatus = false;
Queue<byte> queue = Queue<byte>(); 
uint8_t regisID[6] = "";
uint8_t lenregisID = sizeof(regisID);
uint8_t newbuf = 0;
  
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 439.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 13

void setup() 
{
  Status = ST_START;
  pinMode(LED, OUTPUT);    

  while (!Serial);
  Serial.begin(115200);
  delay(100);

  while (!rf95.init()) {
    while (1);
  }
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    while (1);
  }
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

void loop()
{
  boolean check_ST  = false;
  switch (Status){
    case ST_START:
        START();
        Status = ST_WAIT_ID;
        break;
        
    case ST_WAIT_ID:
        check_ST = WAIT_ID();
        if (check_ST == true) Status = ST_RESP_ID;
        else Status = ST_COUNT_ID;
        break;
        
    case ST_RESP_ID:
        RESP_ID();
        Status = ST_ADD_QUEUE;
        break;

    case ST_ADD_QUEUE:
        ADD_QUEUE();
        Status = ST_WAIT_ID;
        break;
      
    case ST_COUNT_ID:
        check_ST = COUNT_ID();
        if (check_ST == true) Status = ST_REQ_ID_DATA;
        else Status = ST_FINISH;
        break;

    case ST_REQ_ID_DATA:
        REQ_ID_DATA();
        Status = ST_WAIT_ID_DATA;
        break;

    case ST_WAIT_ID_DATA:
        WAIT_ID_DATA();
        Status = ST_COUNT_ID;
        break;

    case ST_FINISH:
        FINISH();
        Status = ST_START;
        break;
    
    default:
        Status = ST_START;
        break;
  }
}

void START(){
  for (int i = 0; i < 256; i++) testqueue[i] = 0;
  checkStatus = false;
  queue.clear();
  newbuf = 0;
  for (int i = 0; i < 5; i++) regisID[i] = 0; 
  lenregisID = sizeof(regisID);
}

boolean WAIT_ID(){
  newbuf = 0;
  for (int i = 0; i < 5; i++) regisID[i] = 0; 
  lenregisID = sizeof(regisID);
  if (rf95.waitAvailableTimeout(3000))     //Nếu trong vòng 10s mà không còn client nào đăng ký thì sẽ ngắt quá trính đăng ký
  {  
    if (rf95.recv(regisID, &lenregisID))      //Khi có 1 client gửi thông tin lên
    {
      int z = 0;
      int x = 0;
      bool checkbuf = true;
      digitalWrite(LED, HIGH);
          //Bắt đầu quá trình kiểm tra...
      while(regisID[x] != 0 && checkbuf ==true){        //Kiểm tra các ký tự có phải là số hay không? => VD: 2
        if(regisID[x] >= 48 && regisID[x] <= 57) x++;
        else checkbuf = false; 
      }
      if ( lenregisID < 6 && regisID[0]!= 'w' && checkbuf == true){  
        while(regisID[z] != 0){         //Đưa số của ASCII về số bình thường. VD ASCII '2' = 50 => byte = 2 = ASCII '2' (50) - 48  
          newbuf = newbuf*10 + regisID[z] - 48;
          z++;
        }
      }
      return true;
    }
  } else return false;
}

void RESP_ID(){
  uint8_t data[] = "w     ";                //With range (10,255): datat[] = "w   ";
  uint8_t lendata = sizeof(data);
  for (int i = 0; i < lenregisID; i++) data[i+1] = regisID[i];
  data[lenregisID+2] = 0;
  Serial.println((char *) data);
  rf95.send(data, sizeof(data));
  rf95.waitPacketSent();
  delay(10);
}

void ADD_QUEUE(){
  if (testqueue[newbuf] == 0 & newbuf != 0){
    queue.push(newbuf);
    testqueue[newbuf] = 1;
  }
}

boolean COUNT_ID(){
  if (queue.count() > 0) return true;
  else return false;
}

void REQ_ID_DATA(){
  uint8_t reqdata[] = "ok     ";      //With range (10,255): reqdata[] = "ok   ";
  uint8_t lenreqdata = sizeof(reqdata);
  byte temp = queue.pop();
  int q = 0;
  uint8_t bufsend[5];          //With range (10,255): bufsend[4];
  uint8_t tempbuf[5];
  //tempbuf[0] = 0;
  if (temp == 0){
    tempbuf[0] = 48;
    q++;
  } else while(temp != 0){
    tempbuf[q] = temp%10 + 48;
    temp = temp / 10;
    q++;
  }
      //Tại thời điểm x, nếu tới lượt ClientID 2 gửi dữ liệu, Node Server sẽ gửi gói tin với nội dung 'ok+ID'. VD 'ok2'
  for (int g = 0; g < q; g++) bufsend[g] = tempbuf[q-g-1];
  bufsend[q] = 0;
  for (int i = 0; i < q; i++) reqdata[i+2] = bufsend[i];
  reqdata[q+2] = 0;
  delay(10);
  rf95.send(reqdata, sizeof(reqdata));
  rf95.waitPacketSent();
}

void WAIT_ID_DATA(){
  if (rf95.waitAvailableTimeout(1000))
  {
    uint8_t datadht[40];
    uint8_t lendht = sizeof(datadht);
    if (rf95.recv(datadht, &lendht))
    {
      int countstar = 0;
      int v = 1;
      uint8_t dataID[4];
      uint8_t dataTem[6];
      uint8_t dataHum[6];
      int cID = 0;
      int cTem = 0;
      int cHum = 0;

      if(datadht[0] == 42){
        countstar = 1;
      } else countstar = 0;
      //Serial.print("Count Star: "); Serial.println(countstar);
      while(countstar > 0 && countstar <4 && datadht[v] != 0){
        if (datadht[v] == 42) countstar++;
        else {
          if (countstar == 1) { dataID[cID] = datadht[v]; cID++;}
          if (countstar == 2) { dataTem[cTem] = datadht[v]; cTem++;}
          if (countstar == 3) { dataHum[cHum] = datadht[v]; cHum++;}
        }
        v++;
      }
      if (countstar == 4){
        uint8_t ID = 0;
        float tem = 0.0;
        float hum = 0.0;
        byte pHum = 0;
        byte pTem = 0;
        dataID[cID] = 0;
        dataHum[cHum] = 0;
        dataTem[cTem] = 0;
        
        for (int j = 0; j < cID; j++) ID = ID*10 + dataID[j] - 48;
        if(dataHum[0] == 45) hum = -1.0;
        else {
          for (int j = 0; j < cHum; j++){ 
            if (dataHum[j] == 46) pHum = j;
            else hum = hum*10 + dataHum[j] - 48;
          }
        hum = hum / pow(10,cHum - pHum - 1);
        }
        if(dataTem[0] == 45) tem = -1.0;
        else {
          for (int j = 0; j < cTem; j++){ 
            if (dataTem[j] == 46) pTem = j;
            else tem = tem*10 + dataTem[j] - 48;
          }
          tem = tem / pow(10,cTem - pTem - 1);
        }
        if (tem == -1.00 || hum == -1.00){} //Serial.println("NO DATA");
        else {
          //Push data on server (TD,Temp,Humi)
          PUSH_DATA(dataID, cID, dataTem, cTem, dataHum, cHum);
        }          
      }
    }
  }
  delay(250);
}

void FINISH(){
  delay(10);
}

void PUSH_DATA(uint8_t* dataID, int cID, uint8_t* dataTem, int cTem, uint8_t* dataHum, int cHum ){
  char urlUp[] = "#                          ";
  for (int aa = 0; aa < cID; aa++) urlUp[1 + aa] = dataID[aa];
  urlUp[1 + cID] = ',';
  for (int bb = 0; bb < cTem; bb++) urlUp[2 + cID + bb] = dataTem[bb];
  urlUp[2 + cID + cTem] = ',';
  for (int cc = 0; cc < cHum; cc++) urlUp[3 + cID + cTem + cc] = dataHum[cc];
  urlUp[3 + cID + cTem + cHum] = '*';
  urlUp[4 + cID + cTem + cHum] = 0;
  Serial.println(urlUp);
}
