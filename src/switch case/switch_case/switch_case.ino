int led_pin = 12;
int a, b, c = 0;
char input;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)
  {
    input = Serial.read();
  }


  switch(input) {
    case 1 : digitalWrite(led_pin, HIGH);
         break;
    case 2 : digitalWrite(led_pin, LOW);
         break;
    default : digitalWrite(led_pin, HIGH);
          delay(1000);
          digitalWrite(led_pin, LOW);
          delay(1000);  
              break;
  }
}
