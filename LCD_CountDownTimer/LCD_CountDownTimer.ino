 #include <LiquidCrystal_I2C.h>
 
int hours = 0; // start hours
int minutes = 0; //start min
int seconds = 5; //start seconds

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

void setup() {
}
 
void loop() {
 lcd.begin(16, 2);
 lcd.print("Count Down Timer ");
 
 // lcd.scrollDisplayLeft();
 // wait a bit:
 delay(150);
 
 while (hours > 0 || minutes > 0 || seconds >= 0) {
 
 lcd.setCursor(4, 2);
 
 (hours < 10) ? lcd.print("0") : NULL;
 lcd.print(hours);
 lcd.print(":");
 (minutes < 10) ? lcd.print("0") : NULL;
 lcd.print(minutes);
 lcd.print(":");
 (seconds < 10) ? lcd.print("0") : NULL;
 lcd.print(seconds);
 lcd.display();
 stepDown();
 delay(1000);
 }
}
 
void stepDown() {
 if (seconds > 0) {
 seconds -= 1;
 } else {
 if (minutes > 0) {
 seconds = 59;
 minutes -= 1;
 } else {
 if (hours > 0) {
 seconds = 59;
 minutes = 59;
 hours -= 1;
 } else {
 trigger();
 }
 }
 }
}
 
void trigger() {
 lcd.clear(); // clears the screen and buffer
 lcd.setCursor(5, 1); // set timer position on lcd for end.
 
 lcd.println("END  ");
 delay(1000);
 
 lcd.display();
}
