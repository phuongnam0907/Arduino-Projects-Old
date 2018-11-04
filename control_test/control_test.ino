int run;
int buttonPin;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  run = 0; //starts stopped
  buttonPin = 7; //whatever pin your button is plugged into

   pinMode(buttonPin, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
    if(digitalRead(buttonPin) == LOW) //funcitons based off of button pulling input pin LOW
  {
     if(run == 0)
     {
    run = 255;
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
     }
     else
     {
         run = 0;
     }
  }

}
