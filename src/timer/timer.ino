unsigned long timer[100];
unsigned long T = 0;
int t = 1;
 
void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("T : ");
  timer[t] = millis();
  t++;
  if(timer[10] != 0)
  {
    T = timer[10] - timer[8];
  }
  Serial.println(T); //prints time since program started
  delay(1000);          // wait a second so as not to send massive amounts of data
}
