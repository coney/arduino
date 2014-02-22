#include <Arduino.h>
void setup();
void loop();
#line 1 "src/sketch.ino"

#include "lcd1602.h"
static LCD1602 *lcd = NULL;

#define HR202_D0    2

void setup()
{
    lcd = LCD1602::CreateDefaultInstance();
    pinMode(HR202_D0, INPUT_PULLUP);
}


void loop()
{
    while (1)
    {
        lcd->printAt(0, "%d", digitalRead(HR202_D0));
        delay(100);
    }
    
}
