#include "Wire.h"
#include "I2Cdev.h"
#include "MPU9250.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU9250 accelgyro;
I2Cdev   I2C_M;

unsigned long timer = 0;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t   mx, my, mz;

float Axyz[3];

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");

    delay(1000);
    Serial.println("     ");

    Serial.println("CLEARDATA");
    Serial.println("LABEL, TIME, Z");
}



void loop()
{
    getAccel_Data();
    
    timer = millis();
    Serial.print("DATA, ");
    Serial.print(timer);
    Serial.print(",");
    Serial.println(Axyz[2]);
   // delay(1000);

}

void getAccel_Data(void)
{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    Axyz[2] = (double) az / 16384;
}
