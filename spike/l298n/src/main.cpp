#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

int inpin[] = {2, 3, 4, 5};
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void setup()
{
    for (int i = 0; i < ARRAY_SIZE(inpin); ++i)
    {
        pinMode(inpin[i], OUTPUT);
    }
    analogWrite(inpin[0], 128);
    analogWrite(inpin[1], 0);
    analogWrite(inpin[2], 255);
    analogWrite(inpin[3], 0);
}

void loop()
{
}
