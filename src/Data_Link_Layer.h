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
 *	 Data_Link_Layer.h
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

#ifndef DATA_LINK_LAYER_H_
#define DATA_LINK_LAYER_H_
#include "ModbusExporttedAPI.h"

#ifdef MODBUS_STACK_TCPIP_ENABLED
MODBUS_BEGIN_DECLS
#define MODBUS_TCP_DEFAULT_PORT   502
#define MODBUS_TCP_SLAVE         0xFF

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * TCP MODBUS ADU = 253 bytes + MBAP (7 bytes) = 260 bytes
 */
#define MODBUS_TCP_MAX_ADU_LENGTH  260

MODBUS_STACK_EXPORT modbus_t* modbus_new_tcp(const char *ip_address, int port);
MODBUS_STACK_EXPORT int modbus_tcp_listen(modbus_t *ctx, int nb_connection);
MODBUS_STACK_EXPORT int modbus_tcp_accept(modbus_t *ctx, int *s);


MODBUS_END_DECLS
#else
MODBUS_BEGIN_DECLS

/* Modbus_Application_Protocol_V1_1b.pdf Chapter 4 Section 1 Page 5
 * RS232 / RS485 ADU = 253 bytes + slave (1 byte) + CRC (2 bytes) = 256 bytes
 */
#define MODBUS_RTU_MAX_ADU_LENGTH  256

MODBUS_STACK_EXPORT modbus_t* initSerialPort(const char *device, int baud, char parity,
                                    int data_bit, int stop_bit);



MODBUS_STACK_EXPORT int modbus_rtu_set_serial_mode(modbus_t *ctx, int mode);
MODBUS_STACK_EXPORT int modbus_rtu_get_serial_mode(modbus_t *ctx);

#define MODBUS_RTU_RTS_NONE   0
#define MODBUS_RTU_RTS_UP     1
#define MODBUS_RTU_RTS_DOWN   2

MODBUS_STACK_EXPORT int modbus_rtu_set_rts(modbus_t *ctx, int mode);
MODBUS_STACK_EXPORT int modbus_rtu_get_rts(modbus_t *ctx);

MODBUS_STACK_EXPORT int modbus_rtu_set_custom_rts(modbus_t *ctx, void (*set_rts) (modbus_t *ctx, int on));

MODBUS_STACK_EXPORT int modbus_rtu_set_rts_delay(modbus_t *ctx, int us);
MODBUS_STACK_EXPORT int modbus_rtu_get_rts_delay(modbus_t *ctx);

MODBUS_END_DECLS
#endif

#endif /* DATA_LINK_LAYER_H_ */
