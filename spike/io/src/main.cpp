#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

void setup()
{
    for (unsigned int i = 2; i <= 13; ++i) {
        pinMode(i, INPUT_PULLUP);
    }

    for (unsigned int i = A0; i <= A7; ++i) {
        pinMode(i, INPUT);
        digitalWrite(i, HIGH);
    }
    
    
    Serial.begin(9600);
}

static int readAsDigital(int pin) {
    if ((pin >= 0 && pin <=13) || (pin >= A0 && pin <= A5)) {
        return digitalRead(pin);
    }
    else {
        return analogRead(pin) > 256 ? HIGH : LOW;
    }
    
}

void loop()
{
    for (unsigned int i = 2; i <= 13; ++i) {
       if (readAsDigital(i) == LOW) {
           Serial.println(i);
           delay(100);
       }
    }
    for (unsigned int i = A0; i <= A7; ++i) {
        if (readAsDigital(i) == LOW) {
            Serial.println(-(int)(i - A0));
            delay(100);
        }
    }

    //Serial.println(millis());
}
