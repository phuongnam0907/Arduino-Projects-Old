
int run;
int buttonPin;

void setup()
{
  run = 0; //starts stopped
   buttonPin = 7; //whatever pin your button is plugged into

   pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
  //code you always run here; you can leave this section blank if you want the entire program to stop and start, or add code here if you want it to always run

  //check button press here and if it is pressed then toggle run variable between 0 and 255; REQUIRED!
  if(digitalRead(buttonPin) == LOW) //funcitons based off of button pulling input pin LOW
  {
     if(run == 0)
     {
         run = 255;
     }
     else
     {
         run = 0;
     }
  }

  if(run > 0)
  {
     //code you only run if button was pressed, stops running when button pressed again, so forth...
  }
}
