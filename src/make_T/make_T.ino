/* This example shows how to get single-shot range
 measurements from the VL53L0X. The sensor can optionally be
 configured with different ranging profiles, as described in
 the VL53L0X API user manual, to get better performance for
 a certain application. This code is based on the four
 "SingleRanging" examples in the VL53L0X API.

 The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
int AVERAGE_COUNT = 3;    // 평균을 내기 위해 모으는 값
int summm = 0, t = 1, k = 0;
unsigned long T = 0;

  int average[500] = {};        // 평균값 집합 ( 1000 -> 500 )
  int min_mm[500] = {};         // 평균값 중 조건에 맞는 값 집합 ( 1000 -> 500 )
  unsigned long timeval[30];                 // min_mm의 시간
  int i = 1, j = 1;    // l, i, j, t는 숫자 카운팅 변수
    

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY


void setup()
{

  sensor.init();
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

void loop()
{
  
  int aver;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
  
  Serial.print("거리는 : ");
  Serial.print(sensor.readRangeSingleMillimeters());
    if (sensor.timeoutOccurred()) {
      Serial.print(" TIMEOUT");
    }
    Serial.println();
    summm += sensor.readRangeSingleMillimeters();
    Serial.print("summm의 값은 : ");
    Serial.println(summm);
    Serial.print("k의 값은 : ");
    Serial.println(k);
    k++;
    aver = 0;
    if( k == AVERAGE_COUNT )
    {
      aver = summm/AVERAGE_COUNT;
      summm = 0;
      k = 0;

      average[i] = aver;
      Serial.print(" 평균 값은 : ");
      Serial.println(aver);

      Serial.print("x값 : ");
      for(int x = 1 ; x<= 10 ; x++)
      {
        Serial.print(average[x]);
      }
      Serial.println();
   
      i++;
    }
      j=i;
    if(average[1] != 0 && average[2] != 0 && average[3] != 0)
    {
      if(average[j] <=average[j+1]  && average[j] <= average[j-1])
      {
        timeval[t] = millis();
        t++;
      }
    }

    Serial.print("y값 : ");
    for(int y = 1 ; y<=10 ; y++)
    {
      Serial.print(timeval[y]);
    }
    Serial.println();

    if(timeval[10] != 0)
    {
      T = timeval[10] - timeval[8];     // 혹시 모를 잘못된 값이 timeval 함수에 들어갈 것을 대비해서 주기 설정
    }
    if(T != 0)
    {
      Serial.print("주기는 : ");
        Serial.println(T);
    }
}
