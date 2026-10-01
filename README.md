# IR Remote Control for PC

An Arduino-based infrared remote control system for a Windows computer. It translates commands from a Samsung TV remote into keyboard actions and uses a micro servo to mechanically press the computer’s power button.

The project is a working prototype combining Arduino firmware, a Windows application, and a servo-driven power-button mechanism.

## Features

- Physical power-button control using an SG90 micro servo.
- Multimedia controls: volume up/down, play/pause, and next/previous track.
- Navigation controls: arrow keys, Enter, and Escape.
- Window switching using Alt+Tab.
- USB serial communication with a `PING`/`PONG` handshake and periodic keep-alive checks.
- LED indicators for controller states: off, pairing, connected, and error.
- Filtering of IR signals so that only predefined command codes are processed.

## Hardware

| Component | Purpose |
| --- | --- |
| Arduino-compatible board with ATmega2560 | Decodes IR signals and controls the hardware |
| HX1838 IR receiver module | Receives commands from the remote |
| Samsung TV IR remote | Provides wireless input |
| SG90 micro servo | Mechanically presses the computer’s power button |
| Status LEDs | Indicate the controller state |
| Breadboard and connecting wires | Connect the prototype components |
| Laptop running Windows | Runs the companion application |
| 9 V battery | Power source tested during prototyping; unsuitable for long-term use |

## Controller Sketch

![Sketch of the IR remote controller](/controller-sketch.png)

## How It Works

1. The HX1838 receiver detects an IR signal from the remote.
2. The Arduino decodes the signal and checks it against the predefined command codes. Unrecognized signals are ignored.
3. When the designated power-on command is received, the Arduino moves the servo to press the computer’s power button.
4. The Arduino periodically sends `PING` over the USB virtual serial (COM) port and waits for a `PONG` response from the Windows application.
5. Once connected, valid remote commands are sent to the application as text-based instructions.
6. The application translates these instructions into virtual keyboard events using the Windows API. Periodic keep-alive messages verify that communication remains active.

Power on/off operation uses the physical power button. The effect of pressing it depends on the computer’s power-button configuration.

## Software

### Arduino Firmware

The firmware handles IR decoding, command filtering, servo actuation, LED status indication, and serial communication.

Arduino libraries used:

- [Arduino-IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote)
- [Servo](https://docs.arduino.cc/libraries/servo/)

### Windows Application

The companion application monitors the COM port, responds to handshake and keep-alive messages, and simulates keyboard input through the Windows API.

The current implementation requires the companion application to be running for keyboard control.

## Prototype

![Photo of the assembled IR remote controller](controller-photo.jpg)

## Known Limitations

- **Reset when opening the serial port:** The Arduino resets when the computer application opens the COM port, even when powered separately. Communication initialization must account for this behavior.
- **Servo power:** During testing, the SG90 did not operate reliably when the Arduino was powered solely through the computer’s USB port. A power supply capable of supporting the servo’s current demand is needed for reliable operation.
- **Battery operation:** A standard 9 V battery proved unsuitable for long-term use because of limited capacity and voltage stability.
- **Software dependency:** Keyboard control depends on the Windows application and an established serial connection.
- **Mechanical alignment:** Reliable power-button operation depends on the servo mounting and alignment.

## Future Improvements

- Use a board with USB HID support so the controller can act directly as a keyboard, including before user login.
- Add a power-saving mode while the computer is off, keeping IR reception active.
- Improve the power-button mechanism with a higher-quality servo or a solenoid.

## References

- [VS1838B IR receiver datasheet](https://www.laskakit.cz/user/related_files/vs1838b.pdf)
- [SG90 servo datasheet](https://www.kjell.com/globalassets/mediaassets/701916_87897_datasheet_en.pdf)
- [Arduino Servo library documentation](https://docs.arduino.cc/libraries/servo/)
- [Arduino-IRremote library](https://github.com/Arduino-IRremote/Arduino-IRremote)
