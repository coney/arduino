#include <Arduino.h>

void setup();
void loop();
#line 6 "src/sketch.ino"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define SWITCH_INTERRUPT_INTERVAL_MS        1000
#define SHUTTING_DOWN_TIMEOUT_S             10


static int power_ctl_pins[] = {4};

static int led_pin = 13;
static int switch_pin = 2;
static int switch_intno = 0;
//static int power_pin = 3;
//static int power_intno = 1;

#define ON          true
#define OFF         false

enum PowerState {
    STOPPED,
    BOOTING,
    RUNNING,
    STOPPING,
};

static struct {
    PowerState          state;
    unsigned long long  timestamp;
} power_state;



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

static void update_power_state(PowerState state){
    if (state == STOPPED) {
        change_power_to(OFF);
    }
    else if (state == BOOTING) {
        change_power_to(ON);
    }
    
    
    power_state.state = state;
    power_state.timestamp = millis();
}

static void init_power_state(){
    update_power_state(STOPPED);
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

static bool led_state = OFF;
static void change_led_to(bool state) {
    led_state = state;
    digitalWrite(led_pin, state == ON ? HIGH : LOW);
}

static void inverse_led_state() {
    change_led_to(!led_state);
}


void on_switch() {
    static unsigned long last_intr_tick = millis();
    if (millis() - last_intr_tick >= SWITCH_INTERRUPT_INTERVAL_MS) {
        last_intr_tick = millis();

        if (power_state.state == STOPPED) {
            Serial.write("start");
            update_power_state(BOOTING);
        } 
        else if (power_state.state == BOOTING || power_state.state == STOPPING) {
            Serial.write("force stop");
            update_power_state(STOPPED);
        }
        else {
            Serial.write("stop");
            // RUNNING, send off signal
            update_power_state(STOPPING);
        }
        
    }
}

//void on_power() {
//    if (millis() - last_intr_tick >= SWITCH_INTERRUPT_INTERVAL_MS) {
//        last_intr_tick = millis();
//        //change_power_to(OFF);
//        change_led_to(ON);
//    }
//}

static void setup_switch() {
    pinMode(switch_pin, INPUT_PULLUP);
    attachInterrupt(switch_intno, on_switch, FALLING);
}

//static void setup_power() {
//    // set to pull down mode manually
//    pinMode(power_pin, INPUT);
//    attachInterrupt(power_intno, on_power, RISING);
//}

#define on_timer(interval, callback) do                 \
{                                                       \
    static unsigned long long __last_tick = millis();   \
    if (millis() > __last_tick + (interval)) {          \
        __last_tick = millis();                         \
        callback;                                       \
    }                                                   \
} while (0)



void setup()
{
    Serial.begin(9600);
    init_power_state();
    setup_led();
    setup_switch();
}

void update_led(PowerState state, unsigned int ticks) {
    switch (state)
    {
    case STOPPED:
        change_led_to(OFF);
        break;
    case BOOTING:
        if (ticks % 10 == 0) {
            inverse_led_state();
        }
        break;
    case RUNNING:
        change_led_to(ON);
        break;
    case STOPPING:
        inverse_led_state();
        break;
    default:
        break;
    }
}

void loop()
{
    on_timer(1000, {
        if (power_state.state == BOOTING || power_state.state == RUNNING) {
            Serial.print("hello");
        } 
        else if (power_state.state == STOPPING) {
            Serial.print("goodbye");
            if (millis() > power_state.timestamp + SHUTTING_DOWN_TIMEOUT_S * 1000) {
                update_power_state(STOPPED);
            }
            
        }
    });

    on_timer(100, {
        static unsigned int ticks = 0;
        update_led(power_state.state, ticks++);
    });
}

void on_serial_message(const String &message) {
    if (message == "alive" && power_state.state == BOOTING) {
        update_power_state(RUNNING);
    }
    else if (message == "wait" && power_state.state == STOPPING) {
        update_power_state(STOPPING);
    }
    
}

static String serial_input;
void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read(); 
        if (inChar == '\0') {
            on_serial_message(serial_input);
            serial_input = "";
        } 
        else {
            serial_input += inChar;
        }
    }
}
