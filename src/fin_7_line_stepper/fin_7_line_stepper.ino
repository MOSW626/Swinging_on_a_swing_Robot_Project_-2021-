#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <IRremote.h>
#include <I2Cdev.h>
#include <MPU9250.h>
#include <AccelStepper.h>

int MAXA = 90;        // 원하는 최대 각도

MPU9250 accelgyro;
I2Cdev   I2C_M;
Servo servo;
AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

  // -------------------- 변수 ----------------------------------

int angle = 0;      // angle은 모터 각도, al은 average length, T는 주기
int k = 0, i= 1, j = 2;    // l, i, j, t는 숫자 카운팅 변수
float summm = 0;              // summ은 지금의 높이의 합, aver는 지금의 3개의 값의 평균
float average[1000];        // 평균값 집합 ( 1000 -> 500 )

unsigned long timer = 0;      // timer, 편의성을 위해 설정

int16_t ax, ay, az;         //함수의 이용을 위해 변수 설정
int16_t gx, gy, gz;
int16_t   mx, my, mz;

float Axyz[3];          // z축 인식 변수
  // ------------------------------------------------------------

void setup()
{
  // -----------setting------------
  Serial.begin(9600);
  Wire.begin();    
  // ------------------------------
  
  // ------------stepper-------------
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  // ------------------------------

  // ------------sensor------------
  pinMode(A0, INPUT);
  // ------------------------------

  // ------------accel-------------
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");
  
  delay(1000);
  Serial.println("     ");
  
  Serial.println("CLEARDATA");
  Serial.println("LABEL, TIME, Z, CONDITION");
  // ------------------------------
}

void loop()
{
  int SENSOR = analogRead(A0);
  
  servo.write(0);

  getAccel_Data();
  
  timer = millis();

  Serial.print("DATA, ");
  Serial.print(timer);
  Serial.print(",");
  Serial.print(Axyz[2]);
  Serial.print(",");
  if(SENSOR < 500)
  {
    Serial.println("WHITE");
  }
  else if(SENSOR > 500)
  {
    Serial.println("DARK");
  }
  

  // -------------------------moter---------------------------
  if(SENSOR > 500)
  {
    stepper.runToNewPosition(0);
    //stepper.runToNewPosition(500);
    delay(300);
    stepper.runToNewPosition(50);
    delay(300);
  }
  // -----------------------------------------------------------
}

void getAccel_Data(void)
{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Axyz[2] = (double) az / 16384;
}
