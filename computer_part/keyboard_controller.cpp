#include <windows.h>
#include <iostream>
#include <string>

// ---------- SERIAL HELPERS ----------

HANDLE openSerial(const char* portName) {
    HANDLE hSerial = CreateFileA(
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open serial port\n";
        return INVALID_HANDLE_VALUE;
    }

    DCB dcb{};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb);

    dcb.BaudRate = CBR_9600;     // Must match Arduino
    dcb.ByteSize = 8;
    dcb.Parity   = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    SetCommState(hSerial, &dcb);

    COMMTIMEOUTS timeouts{};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    return hSerial;
}

void writeLine(HANDLE hSerial, const std::string& s) {
    DWORD written;
    WriteFile(hSerial, s.c_str(), s.size(), &written, nullptr);
}

// ---------- KEYBOARD HELPERS ----------

void keyPress(WORD vk) {
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    SendInput(1, &input, sizeof(INPUT));
}

void keyRelease(WORD vk) {
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void keyTap(WORD vk) {
    keyPress(vk);
    Sleep(20);
    keyRelease(vk);
}

void altTab() {
    keyPress(VK_MENU);   // ALT
    keyTap(VK_TAB);
    keyRelease(VK_MENU);
}

// ---------- COMMAND HANDLER ----------

void handleCommand(const std::string& cmd) {
    if (cmd == "UP") {
        keyTap(VK_UP);
    }
    else if (cmd == "DOWN") {
        keyTap(VK_DOWN);
    }
    else if (cmd == "LEFT") {
        keyTap(VK_LEFT);
    }
    else if (cmd == "RIGHT") {
        keyTap(VK_RIGHT);
    }
    else if (cmd == "ALT_TAB") {
        altTab();
    }
    else if (cmd == "ALT_SHIFT_TAB") {
        keyPress(VK_MENU);   // ALT
        keyPress(VK_SHIFT);  // SHIFT
        keyTap(VK_TAB);      // TAB
        keyRelease(VK_SHIFT);
        keyRelease(VK_MENU);
	}
    else if (cmd == "ENTER") {
        keyTap(VK_RETURN);
	}
    else if (cmd == "ESCAPE") {
        keyTap(VK_ESCAPE);
	}
    else if (cmd == "PLAY" || cmd == "PAUSE") {
        keyTap(VK_MEDIA_PLAY_PAUSE);
	}
    else if (cmd == "VOLUME_UP") {
		keyTap(VK_VOLUME_UP);
    }
	else if (cmd == "VOLUME_DOWN") {
        keyTap(VK_VOLUME_DOWN);
	}
    else if (cmd == "MUTE") {
        keyTap(VK_VOLUME_MUTE);
	}
    else if (cmd == "RETURN") {
		keyPress(VK_MENU);   // ALT
		keyTap(VK_LEFT);    // LEFT
		keyRelease(VK_MENU);
    }
	else if (cmd == "PREVIOUS") {
		keyTap(VK_MEDIA_PREV_TRACK);
	}
	else if (cmd == "NEXT") {
		keyTap(VK_MEDIA_NEXT_TRACK);
	}
    else {
        std::cout << "Unknown command: " << cmd << "\n";
    }
}

// ---------- MAIN LOOP ----------

int main() {
    HANDLE serial = openSerial("COM3"); // <-- CHANGE THIS
    if (serial == INVALID_HANDLE_VALUE) return 1;

    std::string buffer;
    char ch;
    DWORD read;

    std::cout << "Listening...\n";

    while (true) {
        if (ReadFile(serial, &ch, 1, &read, nullptr) && read == 1) {
            if (ch == '\n') {
                // Remove CR if present
                if (!buffer.empty() && buffer.back() == '\r')
                    buffer.pop_back();

                std::cout << "RX: " << buffer << "\n";

                if (buffer == "PING") {
                    writeLine(serial, "PONG\n");
                } else {
                    handleCommand(buffer);
                }

                buffer.clear();
            } else {
                buffer += ch;
            }
        }
    }

    CloseHandle(serial);
    return 0;
}
