#include <Servo.h>

int moter = 5;

Servo servo;

int angle = 0;

void setup()
{
  servo.attach(moter);

  pinMode(moter, OUTPUT);
}

void loop()
{
  Serial.println(angle);

  if(angle == 45)
  {
    angle = 45;
  }
  else if(angle == 0)
  {
    angle = 45;
  }
  servo.write(angle);

  
}
