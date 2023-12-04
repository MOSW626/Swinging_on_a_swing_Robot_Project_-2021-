#include <IRremote.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

IRrecv irrecv(8);
LiquidCrystal_I2C lcd(0x27, 16, 2);
decode_results re;
int count = 0;
String restring;
int cas;
String res;

void setup()
{
  lcd.init();
  lcd.backlight();
  irrecv.enableIRIn();
  Serial.begin(9600);
}


int table()
{
  
    if(res == "FFC23D")
    {
      cas = 0;
    }
    else if(res == "FF30CF")
    {
      cas = 1;
    }
    else if(res == "FF18E7")
    {
      cas = 2;
    }
    else if(res == "FF7A85")
    {
      cas = 3;
    }
    else if(res == "FF10EF")
    {
      cas = 4;
    }
    else if(res == "FF38C7")
    {
      cas = 5;
    }
    else if(res == "FF5AA5")
    {
      cas = 6;
    }
    else if(res == "FF42BD")
    {
      cas = 7;
    }
    else if(res == "FF4AB5")
    {
      cas = 8;
    }
    else if(res == "FF52AD")
    {
      cas = 9;
    }
  
}

void loop()
{
  if(irrecv.decode(&re))
  {
    Serial.println(re.value, HEX);
    
    restring = re.value;
    res = restring;
    table();

    
    if(cas == 0)
    {       
      lcd.init();
      if( count == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print("now set up");
        lcd.setCursor(0,1);
        lcd.print("Setting mod");
        count++;
      }
      else if(count == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("pause");
        lcd.setCursor(0, 1);
        lcd.print("now, stoping");
        count++;
      }
      else if(count == 2)
      {
        lcd.setCursor(0, 0);
        lcd.print("I'am reset.");
        delay(1000);
        lcd.init();
        lcd.setCursor(0, 0);
        lcd.print("Setting mod");

      }
    }    
 else if(cas == 1) {
             
             lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 1");
               lcd.setCursor(0,1);
               lcd.print("S1's best swing");
    
   }
   else if(cas == 2) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 2");
               lcd.setCursor(0,1);
               lcd.print("S2's best swing");
    
   }
   else if(cas == 3) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 3");
               lcd.setCursor(0,1);
               lcd.print("S3's best swing");
   }
    
   else if(cas == 4) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 4");
               lcd.setCursor(0,1);
               lcd.print("S1's worst swing");
   }
    
   else if(cas == 5) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 5");
               lcd.setCursor(0,1);
               lcd.print("S2's worst swing");
   }
    
   else if(cas == 6) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 6");
               lcd.setCursor(0,1);
               lcd.print("S3's worst swing");
   }
    
   else if(cas == 7) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 7");
               lcd.setCursor(0,1);
               lcd.print("The best swing");
   }
    
   else if(cas == 8) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 8");
               lcd.setCursor(0,1);
               lcd.print("The worst swing");
   }
    
   else if(cas == 9) {
               lcd.init();
               lcd.setCursor(0,0);
               lcd.print("case 9");
               lcd.setCursor(0,1);
               lcd.print("??");
   }
    
    }
    

    delay(30);
    irrecv.resume();
  }
