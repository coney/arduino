#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#define LED_PIN 13

void setup()
{
    for (int i = 2; i <= 13; ++i)
    {
        pinMode(i, OUTPUT);
    }
}

void loop()
{
    for (int i = 2; i <= 13; ++i)
    {
        digitalWrite(i, HIGH);
    }
    delay(100);

    for (int i = 2; i <= 13; ++i)
    {
        digitalWrite(i, LOW);
    }
    delay(2000);
}
