#include "./joybus.h"
#include <hardware/pio.h>

joybus_port_t _port;

void setup() {
    set_sys_clock_khz(130000, true);
    Serial.begin(115200);
    joybus_port_init(&_port, D4, pio0, -1, -1);
}

void loop() {
    uint16_t command[10] = {0};
    uint8_t send[9];
    uint8_t recv = joybus_receive_bytes(&_port, command, 10, 50, false);
    if (recv == 10 && (command[0] >> 1) == 0x40 && (command[1] >> 2) == 0x03) {
        send[0] = (uint8_t)(((command[2] & 3) << 6) | ((command[3] & 0x1f8) >> 3));
        send[1] = (uint8_t)(((command[3] & 0x7) << 5) | ((command[4] & 0x1f0) >> 4));
        send[2] = (uint8_t)(((command[4] & 0xf) << 4) | ((command[5] & 0x1e0) >> 5));
        send[3] = (uint8_t)(((command[5] & 0x1f) << 3) | ((command[6] & 0x1c0) >> 6));
        send[4] = (uint8_t)(((command[6] & 0x3f) << 2) | ((command[7] & 0x180) >> 7));
        send[5] = (uint8_t)(((command[7] & 0x7f) << 1) | ((command[8] & 0x100) >> 8));
        send[6] = (uint8_t)(command[8] & 0xff);
        send[7] = (uint8_t)(command[9] >> 1);
        send[8] = 0x0A;
        Serial.write(send, 9);
        //delay(2);
    } else {
        joybus_port_reset(&_port);
    }
}
