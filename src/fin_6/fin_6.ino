#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <IRremote.h>

int MAXA = 90;        // 원하는 최대 각도

VL53L0X sensor;
Servo servo;
decode_results results;
  // -------------------- 변수 ----------------------------------

int angle = 0;      // angle은 모터 각도, al은 average length, T는 주기
int k = 0, i= 1, j = 2;    // l, i, j, t는 숫자 카운팅 변수
int summm = 0;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
int average[1000];        // 평균값 집합 ( 1000 -> 500 )
  // ------------------------------------------------------------

void setup()
{
  servo.attach(5, 544, 2400);

  pinMode(5, OUTPUT);

  Serial.begin(9600);
  Wire.begin();  
  sensor.init();

  // -------------------- sensor -------------------------
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
  // ------------------------------------------------------------
}

void loop()
{
  Serial.print(sensor.readRangeSingleMillimeters());
  if (sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }
  
  servo.write(0);
  
  int aver;              // aver는 지금의 3개의 값의 평균
  
  Serial.println();
  summm += sensor.readRangeSingleMillimeters();
  k++;
  aver = 0;
  if( k == 2 )
  {
    aver = summm/2;
    summm = 0;
    k = 0;
    average[i] = aver;
    Serial.print(" 평균 값은 : ");
    Serial.println(aver);
  }
  if(average[1] != 0 && average[2] != 0 && average[3] != 0)
  {
    if(average[j] <=average[j+1]  && average[j] <= average[j-1])
    {
      if(angle == 0)
      {
        for(angle = 0 ; angle < MAXA ; angle ++)
        {
          servo.write(angle);
          delay(2);
        }
     }
      else if(angle == MAXA)
      {
        for(angle = MAXA ; angle > 0 ; angle--)
        {
          servo.write(angle);
          delay(2);
        }
      }
    }
    j++;
  }
}
