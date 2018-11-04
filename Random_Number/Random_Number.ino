// include the library code:

#include <LiquidCrystal_I2C.h>

int run;
int buttonPin;
int newnum;
unsigned long NumberRand;

// initialize the library with the numbers of the interface pins

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

void setup() {

  // set up the LCD's number of columns and rows:

  lcd.begin(16, 2);

  // Print a message to the LCD.

  lcd.print(" Give A Number!");

  run = 0; //starts stopped
  buttonPin = 7; //whatever pin your button is plugged into

  pinMode(buttonPin, INPUT_PULLUP);

}

void loop() {

// set the cursor to column 0, line 1

// (note: line 1 is the second row, since counting begins with 0):
  if(digitalRead(buttonPin) == LOW) //funcitons based off of button pulling input pin LOW
    {
       if(run == 0)
       {
        run = 255;
        lcd.setCursor(5, 1);
        // print the number of seconds since reset:
        //lcd.print("Second: ");
        //lcd.print(millis()/1000);
        NumRand();
      }
      else
      {
        run = 0;
    }
  }
}
void NumRand()
{
  newnum = random(1,100);
  NumberRand = ((newnum*16+41)*9 + 92);
  lcd.print(NumberRand);
  delay(3000);
  
}

