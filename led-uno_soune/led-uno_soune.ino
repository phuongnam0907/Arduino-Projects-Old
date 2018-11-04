void setup()
{
  pinMode(A0, INPUT);
  for(int z = 0; z < 12; z++)
  {
    pinMode(z, OUTPUT);
  }
}

void loop()
{
  int volume = analogRead(A0);
  volume = map(volume, 200, 950, 0, 14);
  for(int a = 0; a < 13; a++)
  { 
    if(volume >= a)
    {
      digitalWrite(a, HIGH);
    }
    else
    {
      digitalWrite(a, LOW);
    }
  }
}
