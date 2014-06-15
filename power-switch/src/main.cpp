#include <Arduino.h>

void setup();
void loop();
#line 1 "src/sketch.ino"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define SWITCH_INTERRUPT_INTERVAL_MS       1000


static int power_ctl_pins[] = {4};

static int led_pin = 13;
static int switch_pin = 2;
static int switch_intno = 0;

#define ON          true
#define OFF         false

static void change_power_to(bool state) {
    if (state == ON) {
        for (unsigned int i = 0; i < ARRAY_SIZE(power_ctl_pins); ++i) {
            pinMode(power_ctl_pins[i], OUTPUT);
            digitalWrite(power_ctl_pins[i], LOW);
        }
    }
    else {
        for (unsigned int i = 0; i < ARRAY_SIZE(power_ctl_pins); ++i) {
            pinMode(power_ctl_pins[i], INPUT);
        }
    }
}

static void setup_led() {
    pinMode(led_pin, OUTPUT);
    for (unsigned int i = 0; i < 3; ++i) {
        digitalWrite(led_pin, HIGH);
        delay(30);
        digitalWrite(led_pin, LOW);
        delay(100);
    }
    
}

static void change_led_to(bool state) {
    digitalWrite(led_pin, state == ON ? HIGH : LOW);
}

void on_switch() {
    static bool state = OFF;
    static unsigned long last_intr_tick = millis();
    if (millis() - last_intr_tick >= SWITCH_INTERRUPT_INTERVAL_MS) {
        last_intr_tick = millis();
        state = !state;
        change_led_to(state);
        change_power_to(state);
    }
}

static void setup_switch() {
    pinMode(switch_pin, INPUT_PULLUP);
    attachInterrupt(switch_intno, on_switch, FALLING);
}

void setup()
{
    change_power_to(OFF);
    setup_led();
    setup_switch();
}

void loop()
{
  //change_led_to(digitalRead(switch_pin) == LOW);
}
