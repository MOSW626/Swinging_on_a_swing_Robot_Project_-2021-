#include <Wire.h>

int Sensor = 9;    // 센서핀은 9번에 연결
int val;
 
void setup () {
  pinMode(Sensor, INPUT);    // 센서값을 입력으로 설정
  Serial.begin(9600);
  Wire.begin();  
}
 
void loop () {
  val = digitalRead(Sensor);  // 센서값 읽어옴
  if (val == HIGH) {          // 장애물 감지가 안되면
    Serial.println("no");
    delay(500);
  } else {                    // 장애물이 감지되면
    Serial.println("YEHHHHH");
    delay(500);
  }
}
