#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#include "lcd1602.h"

LCD1602 *lcd = NULL;

#define SR04_TRIGGER                21
#define SR04_ECHO                   20
// for mega2560, interrupt of pin20 is 3
#define SR04_ECHO_INT               3

#define SR04_EFFECTIVE_RANGE        10
#define SONIC_SPEED_MPS             340
#define SR04_MAX_TIME_COST_IN_US    (SR04_EFFECTIVE_RANGE * 2000000 / SONIC_SPEED_MPS)

void init_sr04() {
    pinMode(SR04_TRIGGER, OUTPUT);
    pinMode(SR04_ECHO, INPUT);
    attachInterrupt(SR04_ECHO_INT, echo_callback, CHANGE);
}

unsigned long time_to_distance(unsigned long timecost_us) {
    // timecost / 1000000 * sonic-speed * 1000 / 2 
    // return in millimetres
    return timecost_us * SONIC_SPEED_MPS / 2000;
}

void echo_callback(){
    static unsigned long last_timestamp = 0;
    unsigned long cost = micros() - last_timestamp;
    last_timestamp += cost;

    if (cost <= SR04_MAX_TIME_COST_IN_US)
    {
        lcd->printAt(0, "time %8lu", cost);
        lcd->printAt(1, "dist %4lu", time_to_distance(cost));
    }
}

void setup()
{
    init_sr04();
    lcd = LCD1602::CreateDefaultInstance();

}

void loop()
{
    while (1)
    {
        digitalWrite(SR04_TRIGGER, HIGH);
        digitalWrite(SR04_TRIGGER, LOW);
        delay(100);
    }
}
