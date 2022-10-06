/*
 * Modbus_Config.h
 *
 *  Created on: 21-Sep-2022
 *      Author: ubuntu
 */

#ifndef MODBUS_CONFIG_H_
#define MODBUS_CONFIG_H_

#include <stdint.h>

#define MODBUS_STACK_EXPORT
typedef enum
{
    ERROR_OK = 0,
	ERROR_FAIL
}ERROR_T;

MODBUS_STACK_EXPORT ERROR_T get_PortName(char *PortName);
MODBUS_STACK_EXPORT ERROR_T get_ParityValue(char *Parity);
MODBUS_STACK_EXPORT ERROR_T get_IP_Address(uint8_t *IP_Address);

MODBUS_STACK_EXPORT ERROR_T get_BaudrateValue(uint32_t *Baudrate);
MODBUS_STACK_EXPORT ERROR_T get_PortNumber(uint16_t *PortNumber);
MODBUS_STACK_EXPORT ERROR_T get_Slave_ID(uint8_t *Slave_ID);

MODBUS_STACK_EXPORT ERROR_T set_PortName(char *PortName);
MODBUS_STACK_EXPORT ERROR_T set_ParityValue(char *Parity);
MODBUS_STACK_EXPORT ERROR_T set_IP_Address(uint8_t *IP_Address);

MODBUS_STACK_EXPORT ERROR_T set_BaudrateValue(int Baudrate);
MODBUS_STACK_EXPORT ERROR_T set_PortNumber(int PortNumber);
MODBUS_STACK_EXPORT ERROR_T set_Slave_ID(int Slave_ID);

#endif /* MODBUS_CONFIG_H_ */
