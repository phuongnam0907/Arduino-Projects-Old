// inslude the SPI library:
#include <SPI.h>

#define RH_SPI_WRITE_MASK                                  0x80
#define RH_RF95_REG_00_FIFO                                0x00
#define RH_RF95_REG_01_OP_MODE                             0x01
#define RH_RF95_REG_06_FRF_MSB                             0x06
#define RH_RF95_REG_07_FRF_MID                             0x07
#define RH_RF95_REG_08_FRF_LSB                             0x08
#define RH_RF95_REG_09_PA_CONFIG                           0x09
#define RH_RF95_REG_0A_PA_RAMP                             0x0a
#define RH_RF95_REG_0B_OCP                                 0x0b
#define RH_RF95_REG_0C_LNA                                 0x0c
#define RH_RF95_REG_0D_FIFO_ADDR_PTR                       0x0d
#define RH_RF95_REG_0E_FIFO_TX_BASE_ADDR                   0x0e
#define RH_RF95_REG_0F_FIFO_RX_BASE_ADDR                   0x0f
#define RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR                0x10
#define RH_RF95_REG_11_IRQ_FLAGS_MASK                      0x11
#define RH_RF95_REG_12_IRQ_FLAGS                           0x12
#define RH_RF95_REG_13_RX_NB_BYTES                         0x13
#define RH_RF95_REG_14_RX_HEADER_CNT_VALUE_MSB             0x14
#define RH_RF95_REG_15_RX_HEADER_CNT_VALUE_LSB             0x15
#define RH_RF95_REG_16_RX_PACKET_CNT_VALUE_MSB             0x16
#define RH_RF95_REG_17_RX_PACKET_CNT_VALUE_LSB             0x17
#define RH_RF95_REG_18_MODEM_STAT                          0x18
#define RH_RF95_REG_19_PKT_SNR_VALUE                       0x19
#define RH_RF95_REG_1A_PKT_RSSI_VALUE                      0x1a
#define RH_RF95_REG_1B_RSSI_VALUE                          0x1b
#define RH_RF95_REG_1C_HOP_CHANNEL                         0x1c
#define RH_RF95_REG_1D_MODEM_CONFIG1                       0x1d
#define RH_RF95_REG_1E_MODEM_CONFIG2                       0x1e
#define RH_RF95_REG_1F_SYMB_TIMEOUT_LSB                    0x1f
#define RH_RF95_REG_20_PREAMBLE_MSB                        0x20
#define RH_RF95_REG_21_PREAMBLE_LSB                        0x21
#define RH_RF95_REG_22_PAYLOAD_LENGTH                      0x22
#define RH_RF95_REG_23_MAX_PAYLOAD_LENGTH                  0x23
#define RH_RF95_REG_24_HOP_PERIOD                          0x24
#define RH_RF95_REG_25_FIFO_RX_BYTE_ADDR                   0x25
#define RH_RF95_REG_26_MODEM_CONFIG3                       0x26

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

void setup() {
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
  Serial.begin(9600);
  Serial.println(spiWrite(RH_RF95_REG_01_OP_MODE, 0x80), HEX);
}

void loop(){
  //byte data = 0x85;
  //Serial.print("Status: ");
  //Serial.println(spiWrite(RH_RF95_REG_01_OP_MODE, data), HEX);
  //delay(5000);
  if (spiRead(RH_RF95_REG_0D_FIFO_ADDR_PTR) == 10) spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0x00);
  listRegister(10000);
  //SPI.end();
}

void listRegister(int time_) {
  Serial.print("\n\nValue 00: ");
  Serial.println(spiRead(RH_RF95_REG_00_FIFO), HEX);
  Serial.print("Value 01: ");
  Serial.println(spiRead(RH_RF95_REG_01_OP_MODE), HEX);
  Serial.print("Value 06: ");
  Serial.println(spiRead(RH_RF95_REG_06_FRF_MSB), HEX);
  Serial.print("Value 07: ");
  Serial.println(spiRead(RH_RF95_REG_07_FRF_MID), HEX); 
  Serial.print("Value 08: ");
  Serial.println(spiRead(RH_RF95_REG_08_FRF_LSB), HEX);
  Serial.print("Value 09: ");
  Serial.println(spiRead(RH_RF95_REG_09_PA_CONFIG), HEX);
  Serial.print("Value 0A: ");
  Serial.println(spiRead(RH_RF95_REG_0A_PA_RAMP), HEX);
  Serial.print("Value 0B: ");
  Serial.println(spiRead(RH_RF95_REG_0B_OCP), HEX);
  Serial.print("Value 0C: ");
  Serial.println(spiRead(RH_RF95_REG_0C_LNA), HEX);
  Serial.print("Value 0D (increase 1 after read/write FIFO): ");
  Serial.println(spiRead(RH_RF95_REG_0D_FIFO_ADDR_PTR), HEX);
  Serial.print("Value 0E: ");
  Serial.println(spiRead(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR), HEX);
  Serial.print("Value 0F: ");
  Serial.println(spiRead(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR), HEX);
  Serial.print("Value 10: ");
  Serial.println(spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR), HEX);
  Serial.print("Value 11: ");
  Serial.println(spiRead(RH_RF95_REG_11_IRQ_FLAGS_MASK), HEX);
  Serial.print("Value 12: ");
  Serial.println(spiRead(RH_RF95_REG_12_IRQ_FLAGS), HEX);
  Serial.print("Value 13: ");
  Serial.println(spiRead(RH_RF95_REG_13_RX_NB_BYTES), HEX);
  Serial.print("Value 14: ");
  Serial.println(spiRead(RH_RF95_REG_14_RX_HEADER_CNT_VALUE_MSB), HEX);
  Serial.print("Value 15: ");
  Serial.println(spiRead(RH_RF95_REG_15_RX_HEADER_CNT_VALUE_LSB), HEX);
  Serial.print("Value 16: ");
  Serial.println(spiRead(RH_RF95_REG_16_RX_PACKET_CNT_VALUE_MSB), HEX);
  Serial.print("Value 17: ");
  Serial.println(spiRead(RH_RF95_REG_17_RX_PACKET_CNT_VALUE_LSB), HEX);
  Serial.print("Value 18: ");
  Serial.println(spiRead(RH_RF95_REG_18_MODEM_STAT), HEX);
  Serial.print("Value 19: ");
  Serial.println(spiRead(RH_RF95_REG_19_PKT_SNR_VALUE), HEX);
  Serial.print("Value 1A: ");
  Serial.println(spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE), HEX);
  Serial.print("Value 1B: ");
  Serial.println(spiRead(RH_RF95_REG_1B_RSSI_VALUE), HEX);
  Serial.print("Value 1C: ");
  Serial.println(spiRead(RH_RF95_REG_1C_HOP_CHANNEL), HEX);
  Serial.print("Value 1D: ");
  Serial.println(spiRead(RH_RF95_REG_1D_MODEM_CONFIG1), HEX);
  Serial.print("Value 1E: ");
  Serial.println(spiRead(RH_RF95_REG_1E_MODEM_CONFIG2), HEX);
  Serial.print("Value 1F: ");
  Serial.println(spiRead(RH_RF95_REG_1F_SYMB_TIMEOUT_LSB), HEX);
  Serial.print("Value 20: ");
  Serial.println(spiRead(RH_RF95_REG_20_PREAMBLE_MSB), HEX);
  Serial.print("Value 21: ");
  Serial.println(spiRead(RH_RF95_REG_21_PREAMBLE_LSB), HEX);
  Serial.print("Value 22: ");
  Serial.println(spiRead(RH_RF95_REG_22_PAYLOAD_LENGTH), HEX);
  Serial.print("Value 23: ");
  Serial.println(spiRead(RH_RF95_REG_23_MAX_PAYLOAD_LENGTH), HEX);
  Serial.print("Value 24: ");
  Serial.println(spiRead(RH_RF95_REG_24_HOP_PERIOD), HEX);
  Serial.print("Value 25: ");
  Serial.println(spiRead(RH_RF95_REG_25_FIFO_RX_BYTE_ADDR), HEX);
  Serial.print("Value 26: ");
  Serial.println(spiRead(RH_RF95_REG_26_MODEM_CONFIG3), HEX);
  Serial.print("Value 27: ");
  Serial.println(spiRead(0x27), HEX);
  Serial.print("Value 28: ");
  Serial.println(spiRead(0x28), HEX);
  Serial.print("Value 29: ");
  Serial.println(spiRead(0x29), HEX);
  Serial.print("Value 2A: ");
  Serial.println(spiRead(0x2A), HEX);
  Serial.print("Value 2B: ");
  Serial.println(spiRead(0x2B), HEX);
  Serial.print("Value 2C: ");
  Serial.println(spiRead(0x2C), HEX);
  Serial.print("Value 2D: ");
  Serial.println(spiRead(0x2D), HEX);
  Serial.print("Value 2E: ");
  Serial.println(spiRead(0x2E), HEX);
  Serial.print("Value 2F: ");
  Serial.println(spiRead(0x2F), HEX);
  Serial.print("Value 30: ");
  Serial.println(spiRead(0x30), HEX);
  Serial.print("Value 31: ");
  Serial.println(spiRead(0x31), HEX);
  Serial.print("Value 32: ");
  Serial.println(spiRead(0x32), HEX);
  Serial.print("Value 33: ");
  Serial.println(spiRead(0x33), HEX);
  Serial.print("Value 34: ");
  Serial.println(spiRead(0x34), HEX);
  Serial.print("Value 35: ");
  Serial.println(spiRead(0x35), HEX);
  Serial.print("Value 36: ");
  Serial.println(spiRead(0x36), HEX);
  Serial.print("Value 37: ");
  Serial.println(spiRead(0x37), HEX);
  Serial.print("Value 38: ");
  Serial.println(spiRead(0x38), HEX);
  Serial.print("Value 39: ");
  Serial.println(spiRead(0x39), HEX);
  Serial.print("Value 3A: ");
  Serial.println(spiRead(0x3A), HEX);
  Serial.print("Value 3B: ");
  Serial.println(spiRead(0x3B), HEX);
  Serial.print("Value 3C: ");
  Serial.println(spiRead(0x3C), HEX);
  Serial.print("Value 3D: ");
  Serial.println(spiRead(0x3D), HEX);
  Serial.print("Value 3E: ");
  Serial.println(spiRead(0x3E), HEX);
  Serial.print("Value 3F: ");
  Serial.println(spiRead(0x3F), HEX);
  
  delay(time_);
}

byte spiWrite(byte reg, byte val)
{
    uint8_t status = 0;
//    SPI.beginTransaction();
    digitalWrite(slaveSelectPin, LOW);
    status = SPI.transfer(reg | RH_SPI_WRITE_MASK); // Send the address with the write mask on
    SPI.transfer(val); // New value follows
    digitalWrite(slaveSelectPin, HIGH);
//    SPI.endTransaction();
    return status;
}

byte spiRead(byte reg)
{
    byte val;
    digitalWrite(slaveSelectPin, LOW);
    SPI.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the address with the write mask off
    val = SPI.transfer(0); // The written value is ignored, reg value is read
    digitalWrite(slaveSelectPin, HIGH);
    return (int) val;
}
