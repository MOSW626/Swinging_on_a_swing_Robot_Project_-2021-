#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <IRremote.h>

int remote = 7;
int LENGTH = 180;     // 원하는 높이
int MAXA = 60;        // 원하는 최대 각도
int AVERAGE_COUNT = 3;    // 평균을 내기 위해 모으는 값

VL53L0X sensor;
Servo servo;
IRrecv irrecv(remote);
decode_results results;
  // -------------------- 변수 ----------------------------------

int angle = 0, al = 0;      // angle은 모터 각도, al은 average length, T는 주기
int k = 0, i= 1, j = 1, t = 1;    // l, i, j, t는 숫자 카운팅 변수
int summm = 0;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
int average[500];        // 평균값 집합 ( 1000 -> 500 )
int min_mm[500];         // 평균값 중 조건에 맞는 값 집합 ( 1000 -> 500 )
unsigned long timeval[30];                 // min_mm의 시간
unsigned long T=0;                 // 주기
float a=0, b=0, c=0;
int q=0, w=0, e=0, r=0;

  // ------------------------------------------------------------

void setup()
{
  servo.attach(5);
  irrecv.enableIRIn();

  pinMode(5, OUTPUT);
  pinMode(remote, INPUT);

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

  // ------------------- 주기를 만드는 함수 ----------------------
  if(T == 0)
  {
    int aver;              // aver는 지금의 3개의 값의 평균
    
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
     
        i++;
      }
      j = i;
  
      if(average[j] <=average[j+1]  && average[j] <= average[j-1])
      {
        timeval[t] = millis();
        t++;
      }
  
      if(timeval[10] != 0)
      {
        T = timeval[10] - timeval[8];     // 혹시 모를 잘못된 값이 timeval 함수에 들어갈 것을 대비해서 주기 설정
      }
      if(T != 0)
      {
        Serial.print("주기는 : ");
          Serial.println(T);
      }
    // ------------------------------------------------------------
  }
  else
  {
    servo.write(45);
    Serial.println(sensor.readRangeSingleMillimeters());
    al = sensor.readRangeSingleMillimeters();
    if( al < LENGTH )   // al은 바닥까지의 average length
    {
      a = 0.25;
      b = 0.25;
      c = 0.25;

      q = T * a;
      w = T * b * 10;
      e = T * c /4 ;
      r = ( T * (1 - (a + c + b)) ) * 10;
      
    /*   if (irrecv.decode(&results)){  // 적외선 리모컨의 신호를 받을 때
         switch (results.value) {
           case FFC23D:  // 시작 버튼을 눌렀을 때
             a = ;
             b = ;
             c = ;
             moving(a, b, c);
             break;
         }
       }*/

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
  }
}
