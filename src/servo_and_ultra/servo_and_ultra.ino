#include <Servo.h>    // 서보모터 라이브러리
#include <IRremote.h>  // 적외선 리모컨 라이브러리

int remote = A0;    // 적외선 리모컨 수광 다이오드 신호선
int motor = 7;      // 서보모터
int led_blue = 8;   // 파란색 led
int led_red = 9;    // 빨간색 led

Servo servo;          // servo 라이브러리를 사용해주기 위해 servo객체 생성
IRrecv irrecv(remote);    // IRremote를 사용해주기 위해 irrecv객체 생성
decode_results results;   // 디코드한 결과값
  
int angle = 0;    // 서보모터 각도 값

void setup() {
  servo.attach(motor);    // 모터 시작
  irrecv.enableIRIn();    // 수광 다이오드 시작
  
  pinMode(motor,OUTPUT);  // 모터 출력
  pinMode(remote, INPUT); // 수광 다이오드 입력
  pinMode(led_blue, OUTPUT);  // led 출력
  pinMode(led_red, OUTPUT);   // led 출력
}

void loop() {
  if (irrecv.decode(&results)){  // 적외선 리모컨의 신호를 받을 때
    switch (results.value) {
      case 0xFFA857:  // +  버튼을 눌렀을 때
        angle += 15;  // 각도값 15 추가
        if (angle > 180)  // 각도가 180이 넘어갈 때
          angle = 180;    // 각도값 최대를 180으로
        servo.write(angle); // 서보모터 각도 이동
        break;
        
      case 0xFFE01F:  // - 버튼을 눌렀을 때
        angle -= 15;  // 각도값 15 감소
        if(angle < 0) // 각도값이 0도보다 적을 때
          angle = 0;  // 각도값 최소값을 0으로
        servo.write(angle);  // 서보모터 각도 이동 
        break;
    }
    irrecv.resume(); // 수광 다이오드 다음 값 받기
  }

   if(angle >= 0 && angle <= 60) // 서보모터의 각도가 0 ~ 60 사이일 때 
      digitalWrite(led_blue, HIGH); // 파란색 led 출력
   else if (angle >= 120 && angle <= 180 )  // 서보모터의 각도가 120 ~ 180 사이일 때
      digitalWrite(led_red, HIGH);  // 빨간색 led 출력
   else {   // 그 외의 값일 때 파란색, 빨간색 led 둘 다 OFF
       digitalWrite(led_blue, LOW);
       digitalWrite(led_red, LOW);
   }
}
