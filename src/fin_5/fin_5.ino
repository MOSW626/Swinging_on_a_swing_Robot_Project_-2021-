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
 // if(
//  {
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

      q = T * a/10;
      w = T * b;
      e = T * c /40 ;
      r = ( T * (1 - (a + c + b)) );
      while(1)
      {
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
   /*   if (irrecv.decode(&results)){  // 적외선 리모컨의 신호를 받을 때
         switch (results.value) {
           case 'FFC23D' :  // 시작 버튼을 눌렀을 때
             a = 1;
             b = 1;
             c = 1;
             
             q = (T * a) / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF30CF' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF18E7' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF7A85' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF10EF' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF38C7' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF5AA5' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF42BD' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF4AB5' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
             
           case 'FF52AD' :
             a = 1;
             b = 1;
             c = 1;
             
             q = T * a / 4;
             w = T * b * 10 / 4;
             e = T * c / 4;
             r = ( T * (1 - (a + c + b)) ) * 10 / 4;
             
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
             
             break;
         }*/
       }
    }
  //}
}
