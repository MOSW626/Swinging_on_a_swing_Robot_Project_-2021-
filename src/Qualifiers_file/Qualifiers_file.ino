#include <Servo.h>

Servo servserv;

void setup()
{
  servserv.attach(5);
}

void loop()
{
  int angle;
  servserv.write(0);
      while(1)
      {
        for (angle = -100; angle < 75; angle++)
        {
          servserv.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(10);                    //delay로 각도의 변화 속도를 조절
        }
        delay(100);
        for (angle = 75; angle > -100; angle--)
        {
          servserv.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(10);                    //delay로 각도의 변화 속도를 조절
        }
        delay(50);
      }


 // delay(100);
}
