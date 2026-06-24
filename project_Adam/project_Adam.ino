#include <Servo.h>
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
#include "LedController.h"
#include "SerialProtocol.h"

#define DECODE_SAMSUNG

Servo myservo;
const int servoPin = 36;
const int greenLedPin = 47;
const int yellowLedPin = 45;
const int redLedPin = 43;

Led greenLed;
Led yellowLed;
Led redLed;

enum SystemState {
    OFF,
    PAIRING,
    CONNECTED,
    ERROR
};

SystemState state = PAIRING;

void setup() {
    ledInit(greenLed, 47);
    ledInit(yellowLed, 45);
    ledInit(redLed, 43);

    myservo.attach(servoPin);

    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
    ledUpdate(greenLed);
    ledUpdate(yellowLed);
    ledUpdate(redLed);

    switch (state) {
        case PAIRING:
            ledOn(yellowLed);
            if (serialInitHandshake()) {
                state = CONNECTED;
            } else {
                state = OFF;
            }
            ledOff(yellowLed);
            break;

        case OFF:
            if (IrReceiver.decode()) {
                if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
                    uint8_t cmd = IrReceiver.decodedIRData.command;
                    if (cmd == 0x02) {
                        servo_push();
                        state = PAIRING;
                    }
                }
                IrReceiver.resume();
            }
            break;

        case CONNECTED:
            ledOn(greenLed);
            if (IrReceiver.decode()) {
                if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
                    uint8_t cmd = IrReceiver.decodedIRData.command;
                    if (cmd == 0x02) {
                        servo_push();
                        ledPulse(redLed, 300);
                        ledOff(greenLed);
                        state = OFF;
                    } else {
                        serialSendCommand(cmd);
                    }
                }
                IrReceiver.resume();
            }
            if (!serialAliveCheck()) {
                state = ERROR;
                ledOff(greenLed);
            }
            break;

        case ERROR:
            ledPulse(redLed, 3600);
            state = OFF;
            break;

        default:
            state = OFF;
    }
}

void servo_push(void) {
    myservo.write(90);
    delay(150);
    myservo.write(120);
    delay(150);
    myservo.write(90);
}
