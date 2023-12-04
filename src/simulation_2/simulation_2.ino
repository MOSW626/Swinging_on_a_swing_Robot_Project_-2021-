#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
Servo sv;


unsigned long tim = 0;
unsigned long pre = 0;

void setup()
{
  sv.attach(5);

  Serial.begin(9600);
  Wire.begin();


  //------------------ sensor  --------------------
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

  // -------------------------------------------------
}

void loop()
{
  int angle;
  sv.write(45);
  Serial.print(sensor.readRangeSingleMillimeters());
  if (sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }

  Serial.println();

  int d=0;
  

    d = sensor.readRangeSingleMillimeters();

 /*   if (d <= 250)
    {*/
      while(1)
      {
        
        for (angle = 45; angle > 0; angle--)
        {
          sv.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(50);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
        }
        delay(10);
        for (angle = 0; angle < 45; angle++)
        {
          sv.write(angle);    //angle(각도)값으로 서보모터 제어
          delay(50);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
        }
        delay(10);
      }
     /*   Serial.println(sensor.readRangeSingleMillimeters());
        if (sensor.timeoutOccurred()) {
          Serial.println(" TIMEOUT");
        }

       int l = 0;
       l = sensor.readRangeSingleMillimeters();
       if(l > 250)
       {
        break;
       }

      }
      }
      else
      {
        sv.write(45);
      }
      d = 0;
*/
    }
