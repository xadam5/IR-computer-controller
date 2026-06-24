#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <Arduino.h>

/* ===== Configuration ===== */
#define SERIAL_BAUDRATE        9600
#define PING_INTERVAL_MS       1000UL     // how often to send PING
#define INIT_TIMEOUT_MS        36000UL   // 3 minutes
#define ALIVE_INTERVAL_MS     20000UL     // periodic alive check
#define PONG_TIMEOUT_MS       5000UL       // wait for PONG

/* ===== API ===== */
void serialProtocolInit();
bool serialWaitForPong(unsigned long timeoutMs);
bool serialInitHandshake();
bool serialAliveCheck();
void serialSendCommand(uint8_t cmd);

#endif
