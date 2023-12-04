#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <IRremote.h>

int remote = 7;
int moter = 5;
int LENGTH = 150;     // 원하는 높이
int MAXA = 60;        // 원하는 최대 각도
int AVERAGE_COUNT = 3;    // 평균을 내기 위해 모으는 값
void make_average();
void moving(int first, int second, int third);

VL53L0X sensor;
Servo servo;
IRrecv irrecv(remote);
decode_results results;


void setup()
{
  servo.attach(moter);
  irrecv.enableIRIn();

  pinMode(moter, OUTPUT);
  pinMode(remote, INPUT);

  sensor.init();
  Serial.begin(9600);
  Wire.begin();
  
  // -------------------- lazer sensor -------------------------
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
  // -------------------- 변수 ----------------------------------

int angle = 0, al = 0, T = 0;      // angle은 모터 각도, al은 average length, T는 주기
int k = 0, i= 1, j = 1, t = 1;    // l, i, j, t는 숫자 카운팅 변수
int aver, summm = 0;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
int average[1000] = {0, };        // 평균값 집합
int min_mm[1000] = {0, };         // 평균값 중 조건에 맞는 값 집합
int timeval[100];                 // min_mm의 시간
  
extern volatile unsigned long timer0_millis; //타이머변수

unsigned long tim = 0;            // 타이머 변수 2
unsigned long pre = 0;            // 타이머 변수3
  // ------------------------------------------------------------
  
  make_average();  // 주기를 만드는 함수
  
  
  servo.write(45);
  Serial.print(sensor.readRangeSingleMillimeters());
  if (sensor.timeoutOccurred()) {
    Serial.print(" TIMEOUT");
  }
  Serial.println();
  if( al < LENGTH )   // al은 바닥까지의 average length
  {
     moving(0.25, 0.25, 0.25);      // moving이라는 함수 실행
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
  }
}

  // -------------- 그네를 형식에 맞춰서 움직이는 함수 ---------------
void moving(int first, int second, int third)
{

    // -------------------- 변수 ----------------------------------

int angle = 0, al = 0, T = 0;      // angle은 모터 각도, al은 average length, T는 주기
int k = 0, i= 1, j = 1, t = 1;    // l, i, j, t는 숫자 카운팅 변수
int aver, summm = 0;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
int average[1000] = {0, };        // 평균값 집합
int min_mm[1000] = {0, };         // 평균값 중 조건에 맞는 값 집합
int timeval[100];                 // min_mm의 시간
  
extern volatile unsigned long timer0_millis; //타이머변수

unsigned long tim = 0;            // 타이머 변수 2
unsigned long pre = 0;            // 타이머 변수3
  // ------------------------------------------------------------
  
  int q = T * first;
  int w = T * second;
  int e = T * third;
  int r = T - (first + second + third);
  
  for (angle = MAXA; angle > 0; angle--)
  {
    servo.write(angle);    //angle(각도)값으로 서보모터 제어
    delay(q * 0.1);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
  }
  delay(w * 0.001);
  for (angle = 0; angle < MAXA; angle++)
  {
    servo.write(angle);    //angle(각도)값으로 서보모터 제어
    delay(e * 0.1);                    //delay로 각도의 변화 속도를 조절 (0.1초 단위)
  }
  delay(r * 0.001);
}
  // ------------------------------------------------------------


  // ------------------- 주기를 만드는 함수 ----------------------
void make_average()
{

    // -------------------- 변수 ----------------------------------

int angle = 0, al = 0, T = 0;      // angle은 모터 각도, al은 average length, T는 주기
int k = 0, i= 1, j = 1, t = 1;    // l, i, j, t는 숫자 카운팅 변수
int aver, summm = 0;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
int average[1000] = {0, };        // 평균값 집합
int min_mm[1000] = {0, };         // 평균값 중 조건에 맞는 값 집합
int timeval[100];                 // min_mm의 시간
  
extern volatile unsigned long timer0_millis; //타이머변수

unsigned long tim = 0;            // 타이머 변수 2
unsigned long pre = 0;            // 타이머 변수3
  // ------------------------------------------------------------
  while(T != 0)
  {
    summm = 0;
    k = 1;
    summm += sensor.readRangeSingleMillimeters();
    k++;
    aver = 0;
    if( k == AVERAGE_COUNT )
    {
      aver = summm/AVERAGE_COUNT;
      summm = 0;
      k = 0;

      average[i] = aver;
   
      i++;
    }

    if(average[j] <=average[j+1]  && average[j] <= average[j-1] && average[j-1] != 0 && average[j] != 0 && average[j+1] != 0)
    {
      if(t==1)
      {
        timer0_millis=0;
        pre=0;
      }
      timeval[t] = timer0_millis;
      t++;
    }
    T = timeval[10] - timeval[8];     // 혹시 모를 잘못된 값이 timeval 함수에 들어갈 것을 대비해서 주기 설정
  }
}
  // ------------------------------------------------------------
