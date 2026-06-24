#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>


enum LedMode {
    LED_OFF,
    LED_ON,
    LED_PULSE,
    LED_BLINK
};

struct Led {
    uint8_t pin;
    LedMode mode;
    bool state;
    unsigned long interval;
    unsigned long lastChange;
};

/* Initialization */
void ledInit(Led &led, uint8_t pin);

/* Update (call every loop) */
void ledUpdate(Led &led);

/* Control API */
void ledOn(Led &led);
void ledOff(Led &led);
void ledPulse(Led &led, unsigned long durationMs);
void ledBlink(Led &led, unsigned long periodMs);

#endif // LED_CONTROLLER_H
