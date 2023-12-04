#include <IRremote.h>

IRrecv irrecv(7);
decode_results re;

void setup()
{
  irrecv.enableIRIn();

  Serial.begin(9600);
}

void loop()
{
  if(irrecv.decode(&re))
  {
    Serial.println(re.value, HEX);

    if(re.value == 0xFD30CF)
    {
      Serial.println("zero");
    }
    else {
      Serial.println("what the ...");
    }

    delay(30);
    irrecv.resume();
  }
}
