#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#define bt  8

VL53L0X sensor;
Servo servserv;

int state = 0;
bool b = false;
int min_mm[1000] = {0, };
int a= 0;
int summm = 0;
int average;
int i= 1;
int average[1000] = {0, };
int timeval[


extern volatile unsigned long timer0_millis; //타이머변수

unsigned long timeVal=0;  //현재시간값 저장변수
unsigned long previousVal=0; //이전시간값 저장변수

void setup()
{
  servserv.attach(5);
  pinMode(bt, INPUT);
  
  
  Serial.begin(9600);
  Wire.begin();

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif

}
}

void T()
{
  while(1)
  {
    Serial.print(sensor.readRangeSingleMillimeters());
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  
    Serial.println();
    summm += sensor.readRangeSingleMillimeters();
    a++;
    if(a == 10)
    {
      average = summm/10;
      Serial.print("average : ");
      Serial.println(average);

      summm = 0;
      a = 0;

      average[i] = average;
      i++;
    }

    if(average[j] <=average[j+1]  && average[j] <= average[j-1])
    {
      timer0_millis=0;
      previousVal=0;
    }
  }
  
  
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
