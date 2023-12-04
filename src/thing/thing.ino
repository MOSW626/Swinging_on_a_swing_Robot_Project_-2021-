#define led 12
#define bt  8

bool b = false;


void setup()
{
  pinMode(led, OUTPUT);
  pinMode(bt, INPUT);
}

void loop()
{
 int value = digitalRead(bt);

  if( value == HIGH)
  {
    b = true;
  }
  else 
  {
    if(b == true)
    {
      if(digitalRead(led) == HIGH)
      {
        digitalWrite(led, LOW);
      }
      else
      {
        digitalWrite(led, HIGH);
      }
      b = false;
    }  
  }
}
