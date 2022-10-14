 /***************************************************************************************
 *
 *                   Copyright (c) by SoftDEL Systems Ltd.
 *
 *   This software is copyrighted by and is the sole property of SoftDEL
 *   Systems Ltd. All rights, title, ownership, or other interests in the
 *   software remain the property of  SoftDEL Systems Ltd. This software
 *   may only be used in accordance with the corresponding license
 *   agreement. Any unauthorized use, duplication, transmission,
 *   distribution, or disclosure of this software is expressly forbidden.
 *
 *   This Copyright notice may not be removed or modified without prior
 *   written consent of SoftDEL Systems Ltd.
 *
 *   SoftDEL Systems Ltd. reserves the right to modify this software
 *   without notice.
 *
 *   SoftDEL Systems Ltd.						india@softdel.com
 *   3rd Floor, Pentagon P4,					http://www.softdel.com
 *	 Magarpatta City, Hadapsar
 *	 Pune - 411 028
 *
 *
 *   FILE
 *	 modbus-data.c
 *
 *   AUTHORS
 *
 *
 *   DESCRIPTION
 *
 *   RELEASE HISTORY
 *
 *
 *************************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include "ModbusExporttedAPI.h"
#include <safe_lib.h>

#if defined(__GNUC__)
#  define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__ * 10)
#  if GCC_VERSION >= 430
// Since GCC >= 4.30, GCC provides __builtin_bswapXX() alternatives so we switch to them
#    undef bswap_32
#    define bswap_32 __builtin_bswap32
#  endif
#  if GCC_VERSION >= 480
#    undef bswap_16
#    define bswap_16 __builtin_bswap16
#  endif
#endif

/* Sets many bits from a single byte value (all 8 bits of the byte value are
   set) */
void modbus_set_bits_from_byte(uint8_t *dest, int idx, const uint8_t value)
{
    int i;

    for (i=0; i < 8; i++) {
        dest[idx+i] = (value & (1 << i)) ? 1 : 0;
    }
}

/* Sets many bits from a table of bytes (only the bits between idx and
   idx + nb_bits are set) */
void modbus_set_bits_from_bytes(uint8_t *dest, int idx, unsigned int nb_bits,
                                const uint8_t *tab_byte)
{
    unsigned int i;
    int shift = 0;

    for (i = idx; i < idx + nb_bits; i++) {
        dest[i] = tab_byte[(i - idx) / 8] & (1 << shift) ? 1 : 0;
        /* gcc doesn't like: shift = (++shift) % 8; */
        shift++;
        shift %= 8;
    }
}

/* Gets the byte value from many bits.
   To obtain a full byte, set nb_bits to 8. */
uint8_t modbus_get_byte_from_bits(const uint8_t *src, int idx,
                                  unsigned int nb_bits)
{
    unsigned int i;
    uint8_t value = 0;

    if (nb_bits > 8) {
        /* Assert is ignored if NDEBUG is set */
        assert(nb_bits < 8);
        nb_bits = 8;
    }

    for (i=0; i < nb_bits; i++) {
        value |= (src[idx+i] << i);
    }

    return value;
}

/* Get a float from 4 bytes (Modbus) without any conversion (ABCD) */
float modbus_get_float_abcd(const uint16_t *src)
{
    float f;
    uint32_t i;
    uint8_t a, b, c, d;

    a = (src[0] >> 8) & 0xFF;
    b = (src[0] >> 0) & 0xFF;
    c = (src[1] >> 8) & 0xFF;
    d = (src[1] >> 0) & 0xFF;

    i = (a << 24) |
        (b << 16) |
        (c << 8) |
        (d << 0);
    memcpy_s((void*)&f,(rsize_t) sizeof(float),(void*)&i,(rsize_t) sizeof(float));

    return f;
}

/* Get a float from 4 bytes (Modbus) in inversed format (DCBA) */
float modbus_get_float_dcba(const uint16_t *src)
{
    float f;
    uint32_t i;
    uint8_t a, b, c, d;

    a = (src[0] >> 8) & 0xFF;
    b = (src[0] >> 0) & 0xFF;
    c = (src[1] >> 8) & 0xFF;
    d = (src[1] >> 0) & 0xFF;

    i = (d << 24) |
        (c << 16) |
        (b << 8) |
        (a << 0);
    memcpy_s((void*)&f,(rsize_t) sizeof(float),(void*)&i,(rsize_t) sizeof(float));

    return f;
}

/* Get a float from 4 bytes (Modbus) with swapped bytes (BADC) */
float modbus_get_float_badc(const uint16_t *src)
{
    float f;
    uint32_t i;
    uint8_t a, b, c, d;

    a = (src[0] >> 8) & 0xFF;
    b = (src[0] >> 0) & 0xFF;
    c = (src[1] >> 8) & 0xFF;
    d = (src[1] >> 0) & 0xFF;

    i = (b << 24) |
        (a << 16) |
        (d << 8) |
        (c << 0);
    memcpy_s((void*)&f,(rsize_t) sizeof(float),(void*)&i,(rsize_t) sizeof(float));

    return f;
}

/* Get a float from 4 bytes (Modbus) with swapped words (CDAB) */
float modbus_get_float_cdab(const uint16_t *src)
{
    float f;
    uint32_t i;
    uint8_t a, b, c, d;

    a = (src[0] >> 8) & 0xFF;
    b = (src[0] >> 0) & 0xFF;
    c = (src[1] >> 8) & 0xFF;
    d = (src[1] >> 0) & 0xFF;

    i = (c << 24) |
        (d << 16) |
        (a << 8) |
        (b << 0);
    memcpy_s((void*)&f,(rsize_t) sizeof(float),(void*)&i,(rsize_t) sizeof(float));

    return f;
}

/* DEPRECATED - Get a float from 4 bytes in sort of Modbus format */
float modbus_get_float(const uint16_t *src)
{
    float f;
    uint32_t i;

    i = (((uint32_t)src[1]) << 16) + src[0];
    memcpy_s((void*)&f,(rsize_t) sizeof(float),(void*)&i,(rsize_t) sizeof(float));

    return f;

}

/* Set a float to 4 bytes for Modbus w/o any conversion (ABCD) */
void modbus_set_float_abcd(float f, uint16_t *dest)
{
    uint32_t i;
    uint8_t *out = (uint8_t*) dest;
    uint8_t a, b, c, d;

    memcpy_s((void*)&i,(rsize_t) sizeof(uint32_t),(void*)&f,(rsize_t) sizeof(uint32_t));
    a = (i >> 24) & 0xFF;
    b = (i >> 16) & 0xFF;
    c = (i >> 8) & 0xFF;
    d = (i >> 0) & 0xFF;

    out[0] = a;
    out[1] = b;
    out[2] = c;
    out[3] = d;
}

/* Set a float to 4 bytes for Modbus with byte and word swap conversion (DCBA) */
void modbus_set_float_dcba(float f, uint16_t *dest)
{
    uint32_t i;
    uint8_t *out = (uint8_t*) dest;
    uint8_t a, b, c, d;

    memcpy_s((void*)&i,(rsize_t) sizeof(uint32_t),(void*)&f,(rsize_t) sizeof(uint32_t));
    a = (i >> 24) & 0xFF;
    b = (i >> 16) & 0xFF;
    c = (i >> 8) & 0xFF;
    d = (i >> 0) & 0xFF;

    out[0] = d;
    out[1] = c;
    out[2] = b;
    out[3] = a;

}

/* Set a float to 4 bytes for Modbus with byte swap conversion (BADC) */
void modbus_set_float_badc(float f, uint16_t *dest)
{
    uint32_t i;
    uint8_t *out = (uint8_t*) dest;
    uint8_t a, b, c, d;

    memcpy_s((void*)&i,(rsize_t) sizeof(uint32_t),(void*)&f,(rsize_t) sizeof(uint32_t));
    a = (i >> 24) & 0xFF;
    b = (i >> 16) & 0xFF;
    c = (i >> 8) & 0xFF;
    d = (i >> 0) & 0xFF;

    out[0] = b;
    out[1] = a;
    out[2] = d;
    out[3] = c;
}

/* Set a float to 4 bytes for Modbus with word swap conversion (CDAB) */
void modbus_set_float_cdab(float f, uint16_t *dest)
{
    uint32_t i;
    uint8_t *out = (uint8_t*) dest;
    uint8_t a, b, c, d;

    memcpy_s((void*)&i,(rsize_t) sizeof(uint32_t),(void*)&f,(rsize_t) sizeof(uint32_t));
    a = (i >> 24) & 0xFF;
    b = (i >> 16) & 0xFF;
    c = (i >> 8) & 0xFF;
    d = (i >> 0) & 0xFF;

    out[0] = c;
    out[1] = d;
    out[2] = a;
    out[3] = b;
}

/* DEPRECATED - Set a float to 4 bytes in a sort of Modbus format! */
void modbus_set_float(float f, uint16_t *dest)
{
    uint32_t i;

    memcpy_s((void*)&i,(rsize_t) sizeof(uint32_t),(void*)&f,(rsize_t) sizeof(uint32_t));
    dest[0] = (uint16_t)i;
    dest[1] = (uint16_t)(i >> 16);
}
