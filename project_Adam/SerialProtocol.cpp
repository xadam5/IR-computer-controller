#include "SerialProtocol.h"

/* ===== Internal state ===== */
static unsigned long lastAliveCheck = 0;

/* ===== Command mapping ===== */
static const char* commandToString(uint8_t cmd) {
    switch (cmd) {
        case 0x12:               return "ALT_TAB";
        case 0x10:               return "ALT_SHIFT_TAB";
        case 0x65:               return "LEFT";
        case 0x62:               return "RIGHT";
        case 0x60:               return "UP";
        case 0x61:               return "DOWN";
        case 0x68:               return "ENTER";
        case 0x47:               return "PLAY";
        case 0x4A:               return "PAUSE";
        case 0x48:               return "NEXT";
        case 0x45:               return "PREVIOUS";
        case 0x7:                return "VOLUME_UP";
        case 0xB:                return "VOLUME_DOWN";
        case 0xF:                return "MUTE";
        case 0x58:               return "RETURN";
        case 0x2D:               return "ESCAPE";
        default:                 return "UNKNOWN";
    }
}

/* ===== Initialization ===== */
void serialProtocolInit() {
    Serial.begin(SERIAL_BAUDRATE);
    while (!Serial) {
        ; // wait for serial (important for Leonardo, harmless for UNO)
    }
}

/* ===== Low-level PONG wait ===== */
bool serialWaitForPong(unsigned long timeoutMs) {
    unsigned long start = millis();
    String line;

    while (millis() - start < timeoutMs) {
        while (Serial.available()) {
            char c = Serial.read();
            if (c == '\n') {
                line.trim();
                if (line == "PONG") {
                    return true;
                }
                line = "";
            } else {
                line += c;
            }
        }
    }
    return false;
}

/* ===== 1. Initialization handshake ===== */
bool serialInitHandshake() {
    unsigned long start = millis();

    while (millis() - start < INIT_TIMEOUT_MS) {

        Serial.println("PING");

        if (serialWaitForPong(PONG_TIMEOUT_MS)) {
            return true; // success
        }

        delay(PING_INTERVAL_MS);
    }

    // Timeout reached
    return false;
}

/* ===== 2. Periodic alive check ===== */
bool serialAliveCheck() {
    unsigned long now = millis();

    if (now - lastAliveCheck < ALIVE_INTERVAL_MS) {
        return true;
    }

    lastAliveCheck = now;
    Serial.println("PING");

    if (!serialWaitForPong(PONG_TIMEOUT_MS)) {
        // optional: handle lost connection
        return false;
    }
    return true;
}

/* ===== 3. Send command ===== */
void serialSendCommand(uint8_t cmd) {
    if (commandToString(cmd) != "UNKNOWN") {
      Serial.println(commandToString(cmd));
    }
}
