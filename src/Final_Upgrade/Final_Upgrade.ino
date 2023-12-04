#include <Servo.h>

Servo myservo;  // create servo object to control a servo

//extern volatile unsigned long timer0_millis; //타이머 리셋 변수

unsigned long timeVal=0;  //현재시간값 저장변수
unsigned long TimeRight=0;  //근근이가 오른쪽으로 중앙을 지나는 시간
unsigned long TimeLeft=0;  //근근이가 왼쪽으로 중앙을 지나는 시간 
unsigned long HalfPeriod=0;  //반주기



int delaytime = 100; //모터 작동 후 딜레이 시간
int Initialangle = 20; //근근이가 누웠을 때 각도
int Laterangle = 90;  //근근이가 앉았을 때 각도

int gatesensorRightPin = 5; //앞쪽 센서 핀
int gatesensorLeftPin = 6;  //뒷쪽 센서 핀
int gateRightClosed =0; //앞쪽 센서에 인식 되었는지 여부
int gateLeftClosed =0;  //뒷쪽 센서에 인식 되었는지 여부
int right =1; //오른쪽으로 가고 있는지 여부

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); //시리얼 통신 속도
  myservo.write(Initialangle); //전원을 켰을 때 근근이가 눕도록 세팅
}

void loop() {
  timeVal = millis(); //현재 시각을 나타내는 변수

  gateRightClosed = digitalRead(gatesensorRightPin);  //앞쪽 센서로부터 값을 읽음
 
  if (gateRightClosed==1) {  //앞쪽 센서가 인식 되면
    
    if (right == 1) {  // 오른쪽으로 가고 있다면
      TimeRight=timeVal;  //현재 시각을 TimeRight에 기록
      Serial.print("TimeRight=");  //시리얼 모니터에 현재 시각 출력
      Serial.println(TimeRight);
      delay(HalfPeriod*30/100); //원하는 시간만큼 기다림
      myservo.write(Laterangle);  //근근이 앉음
      HalfPeriod=TimeRight-TimeLeft; //반주기 계산
      Serial.print("HalfPeriod=");  //시리얼 모니터에 반주기 출력
      Serial.println(HalfPeriod);
      right=0;   //이제 왼쪽으로 갈거라고 표시
      delay(delaytime);  // 근근이 앉을 때까지 기다림
      
    } else{}
  }

  gateLeftClosed = digitalRead(gatesensorLeftPin);  //뒷쪽 센서로부터 값을 읽음

  if (gateLeftClosed==1) { //뒷쪽 센서가 인식 되면
    
    if (right == 0) { // 왼쪽으로 가고 있다면
      TimeLeft=timeVal; //현재 시각을 TimeLeft에 기록

      Serial.print("TimeLeft=");  //시리얼 모니터에 현재 시각 출력
      Serial.println(TimeLeft);
      delay(HalfPeriod*15/100); //원하는 시간만큼 기다림
      myservo.write(Initialangle);  //근근이 누움
      HalfPeriod=TimeLeft-TimeRight; //반주기 계산
      Serial.print("HalfPeriod=");  //시리얼 모니터에 반주기 출력
      Serial.println(HalfPeriod);
      right=1;   //이제 오른쪽으로 갈거라고 표시
      delay(delaytime);  // 근근이 누울 때까지 기다림
    } else {}
  }

}
