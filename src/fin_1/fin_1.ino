#include <Servo.h>

Servo servserv;

#define bt  8
int state = 0;

bool b = false;

void setup()
{
  servserv.attach(6);
  pinMode(bt, INPUT);
}

void loop()
{

  int angle;
  servserv.write(45);
  int value = digitalRead(bt);

  if( value == HIGH)
  {
    b = true;
  }
  else 
  {
    if(b == true)
    {
      while(1)
      {
        
        for (angle = 45; angle > 0; angle--)
        {
          servserv.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(0.01);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
        }
        delay(426);                 // 0.001초 단위
        for (angle = 0; angle < 45; angle++)
        {
          servserv.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(0.01);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
        }
        delay(426);                 // 0.001초 단위
      }

      b = false;
    }
  }

  delay(100);
}
