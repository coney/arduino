#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#include <SoftwareSerial.h>
#include "lcd1602.h"


#define S1TX    14
#define S1RX    15

LCD1602 lcd;
SoftwareSerial s1(S1RX, S1TX);
void setup()
{
    lcd.printAt(0, "ready for serial!");
    s1.begin(9600);
    s1.println("hello");
}

void loop()
{
    String str = s1.readString();
    if (str.length())
    {
        lcd.printAt(0, "data arrive");
        lcd.printAt(1, "%s", str.c_str());
    }
}
