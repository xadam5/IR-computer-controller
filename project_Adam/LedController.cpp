#include "LedController.h"

void ledInit(Led &led, uint8_t pin) {
    led.pin = pin;
    led.mode = LED_OFF;
    led.state = LOW;
    led.interval = 0;
    led.lastChange = 0;

    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void ledUpdate(Led &led) {
    unsigned long now = millis();

    switch (led.mode) {

        case LED_OFF:
            digitalWrite(led.pin, LOW);
            led.state = LOW;
            break;

        case LED_ON:
            digitalWrite(led.pin, HIGH);
            led.state = HIGH;
            break;

        case LED_PULSE:
            if (!led.state) {
                digitalWrite(led.pin, HIGH);
                led.state = HIGH;
                led.lastChange = now;
            }
            else if (now - led.lastChange >= led.interval) {
                digitalWrite(led.pin, LOW);
                led.state = LOW;
                led.mode = LED_OFF;
            }
            break;

        case LED_BLINK:
            if (now - led.lastChange >= led.interval) {
                led.state = !led.state;
                digitalWrite(led.pin, led.state);
                led.lastChange = now;
            }
            break;
    }
}

void ledOn(Led &led) {
    led.mode = LED_ON;
    ledUpdate(led);
}

void ledOff(Led &led) {
    led.mode = LED_OFF;
    ledUpdate(led);
}

void ledPulse(Led &led, unsigned long durationMs) {
    led.mode = LED_PULSE;
    led.interval = durationMs;
    led.state = LOW;
}

void ledBlink(Led &led, unsigned long periodMs) {
    led.mode = LED_BLINK;
    led.interval = periodMs;
    led.lastChange = millis();
}
