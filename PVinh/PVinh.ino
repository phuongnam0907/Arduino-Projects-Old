#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);

void setup()
{
  //lcd.init();                      // initialize the lcd 
 lcd.begin(16,2);
  // Print a message to the LCD.
  //lcd.backlight();
  lcd.print("Hello, world!");
}

void loop()
{
}
