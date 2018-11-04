int pin = 2;

void setup() {

pinMode(pin, OUTPUT);
}
void loop() {
digitalWrite(pin, HIGH);   //bật led
delay(10000);               //dừng 1s
digitalWrite(pin, LOW);    //tắt led
delay(10000);               //dừng 1s
}
