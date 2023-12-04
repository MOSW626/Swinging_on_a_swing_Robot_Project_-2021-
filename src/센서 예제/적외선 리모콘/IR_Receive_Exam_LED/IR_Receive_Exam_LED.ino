
/*
Project. 센서다루기 - 적외선 리모컨 // 댓글, 추천 부탁 드립니다. - Devicemart 
1. 리모콘 라이브러리 다운로드
Arduino-IRremote  -   https://github.com/z3t0/Arduino-IRremote
2. 디바이스마트 블로그 참조
http://blog.naver.com/no1_devicemart/
by David Gwak - myksj1105@hanmail.net
*/


//#1. 라이브러리 추가
#include <IRremote.h>

//#2. 적외선 수광부 핀 설정
int IR_Receive_Pin = A0; 

//#3. 적외선 리모컨 초기 설정
IRrecv irrecv(IR_Receive_Pin);
decode_results results;

//#4. Setup문
void setup(){
  Serial.begin(9600);  // 신호율 9600 - 컴퓨터를 통해 데이터 확인하기 위한 설정
  irrecv.enableIRIn();  // 적외선 리모컨 사용하기 위한 초기설정
  Serial.println("IR Receive Test OK!"); 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  for (int i=2; i <= 6; i++){
  digitalWrite(i, LOW);
}
}

void LED_Contoller(int number);

//#5. Loop문
void loop() {
  
  if (irrecv.decode(&results)) {          // IR 리모콘에 데이터가 들어 왔을 시에
    Serial.println(results.value, HEX);  // 시리얼 모니터에 출력
    if(results.value == 0xFF30CF){  LED_Contoller(1);}
    if(results.value == 0xFF18E7){  LED_Contoller(2);}
    if(results.value == 0xFF7A85){  LED_Contoller(3);}
    if(results.value == 0xFF10EF){  LED_Contoller(4);}
    if(results.value == 0xFF38C7){  LED_Contoller(5);}
    
    irrecv.resume(); // Receive the next value    
  }
  
}

void LED_Contoller(int number){  
//for (initialization; condition; increment) {
for (int i=2; i <= 6; i++){
  digitalWrite(i, LOW);
}
for (int i=2; i <= number+1; i++){
  digitalWrite(i, HIGH);
}
}


