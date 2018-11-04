void setup() {
//here is an input for sound sensor
pinMode(A0, INPUT);
//here we are setting up all pins as an outputs for LEDs
for(int z = 0; z < 10; z++){
pinMode(z, OUTPUT);
}
}
void loop() {
//here we are storing the volume value
int volume = analogRead(A0);
//max value for analog read is 1023 but it must be very very loud to reach this value
//so I lower it down in map function to 700
//mapping volume value to make it easier to turn LEDs on
volume = map(volume, 0, 700, 0, 10);
//for loop to turn on or off all LEDs
//thanks to this loop code for this project is very short
//we are going through all pins where we have LEDs and checking if the volume is
//bigger then pin number (that's why we are maping the volume)
for(int a = 0; a < 10; a++){ if(volume >= a){
//if it is bigger we can turn on the LED
digitalWrite(a, HIGH);
}else{
//if it is smaller we can turn the LED off
digitalWrite(a, LOW);
}
}
}
