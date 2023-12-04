#include <Servo.h>


int MAXA = 60;        // 원하는 최대 각도
unsigned long T = 157;
int angle = 0;

Servo servo;


void setup()
{
  servo.attach(5);
}

void loop()
{
  servo.write(0);
  float a = 0.19;
  float b = 0.31;
  float c = 0.19;

  Serial.println("now move");
  int q = T * a;
  int w = T * b * 10;
  int e = T * c;
  int r = ( T - (a + c + b) ) * 10;
  
  for (angle = MAXA; angle > 0; angle--)
  {
    servo.write(angle);    //angle(각도)값으로 서보모터 제어
    delay(q);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
  }
  delay(w);
  for (angle = 0; angle < MAXA; angle++)
  {
    servo.write(angle);    //angle(각도)값으로 서보모터 제어
    delay(e);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
  }
  delay(r);
}
