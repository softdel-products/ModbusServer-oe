/*
 * modbus-rtu-private.h
 *
 *  Created on: 05-Sep-2022
 *      Author: ubuntu
 */

#ifndef MODBUS_RTU_PRIVATE_H
#define MODBUS_RTU_PRIVATE_H

#include <stdint.h>
#include <termios.h>

#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2

typedef struct _modbus_rtu {
    /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*" on Mac OS X. */
    char *device;
    /* Bauds: 9600, 19200, 57600, 115200, etc */
    int baud;
    /* Data bit */
    uint8_t data_bit;
    /* Stop bit */
    uint8_t stop_bit;
    /* Parity: 'N', 'O', 'E' */
    char parity;

    /* Save old termios settings */
    struct termios old_tios;

    /* To handle many slaves on the same link */
    int confirmation_to_ignore;
} modbus_rtu_t;

#endif /* MODBUS_RTU_PRIVATE_H */
