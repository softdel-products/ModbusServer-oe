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
 *	 Modbus_Config.c
 *
 *   AUTHORS
 *	 Kailas Vadak
 *
 *   DESCRIPTION
 *
 *   RELEASE HISTORY
 *
 *
 *************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include "cJSON.h"
#include "Modbus_Config_API.h"



ERROR_T ParseConfigParamJson( char *ModbusCnfgData, char *ptrObjName, char *ptrParmName, void *ConfigValue)
{
    cJSON *root 		= NULL;
    cJSON *item 		= NULL;
    cJSON *jsonValue	= NULL;

    if(NULL == (root = cJSON_Parse(ModbusCnfgData)))
    {
    	printf("Invalid config JSON file \r\n");
    	return ERROR_FAIL;
    }
    else if(NULL == (item = cJSON_GetObjectItem(root, ptrObjName)))
	{
    	printf("Could not find config object in config JSON file \r\n");
    	cJSON_Delete(root);
    	return ERROR_FAIL;
	}
    else
    {
    	if(NULL == (jsonValue = cJSON_GetObjectItem(item, ptrParmName)))
		{
			printf("Could not find object item portname in config JSON file \r\n");
		}
		else
		{
			if(cJSON_IsString(jsonValue)) /*  Judge key Of value Is it string */
			{
				memcpy(ConfigValue, jsonValue->valuestring,strlen(jsonValue->valuestring));
			}else
			{
				*((int*)ConfigValue) = jsonValue->valueint;
			}
		}
    }

    cJSON_Delete(root);
    return ERROR_OK;
}

ERROR_T GetConfigParam(char *ptrFileName, char *ptrObjName, char *ptrParamName,void *ConfigValue)
{
	char *pcBuff 					= NULL;
	FILE *fp 						= NULL;
	unsigned int nFileReadBytes 	= 0;
	struct stat status;

    if(0 != stat(ptrFileName, &status))
    {
        printf("Modbus Configuration file doesn't exist \r\n");
        return ERROR_FAIL;
    }
    else if(NULL == (fp = fopen(ptrFileName, "r")))
    {
    	printf("Could not open Modbus Configuration file \r\n");
    	return ERROR_FAIL;
    }
    else
    {
		if(0 != fseek(fp, 0, SEEK_SET))
		{
			printf("Could not set Modbus Configuration file to read \r\n");
			return ERROR_FAIL;
		}
		else
		{
			/*malloc memory*/
			pcBuff = malloc(status.st_size+1);
			if(NULL == pcBuff)
			{
				printf("Failed to malloc for Modbus config file \r\n");
				return ERROR_FAIL;
			}
			memset(pcBuff, 0x0, status.st_size+1);

			nFileReadBytes = fread(pcBuff, 1, status.st_size, fp);
			if(0 == nFileReadBytes)
			{
				printf("Did not read Modbus config file \r\n");
				return ERROR_FAIL;
			}
		}
		fclose(fp);
    }

    ParseConfigParamJson(pcBuff, ptrObjName, ptrParamName, ConfigValue);

    free(pcBuff);
    return ERROR_OK;
}

ERROR_T Set_ParseConfigParamJson(char *ptrFileName, char *ptrObjName, char *ModbusCnfgData, char *ptrParmName, void *ConfigValue)
{
	cJSON *root 		= NULL;
	cJSON *item 		= NULL;
	cJSON *jsonValue	= NULL;
	cJSON *replacement = NULL;
	FILE *OutputFile=NULL;
	FILE *fp=NULL;

	if((OutputFile = fopen("temp_file_path.json", "w")) == NULL)
	{
		printf("\n Can not open file");fflush(stdout);
		pclose(fp);
		return -1;
	}
	if(NULL == (root = cJSON_Parse(ModbusCnfgData)))
	{
		printf("Invalid config JSON file \r\n");
		return ERROR_FAIL;
	}
	else if(NULL == (item = cJSON_GetObjectItem(root, ptrObjName)))
	{
		printf("Could not find config object in config JSON file \r\n");
		cJSON_Delete(root);
		return ERROR_FAIL;
	}
	else
	{
		if(NULL == (jsonValue = cJSON_GetObjectItem(item, ptrParmName)))
		{
			printf("Could not find object item portname in config JSON file \r\n");
		}
		else
		{
			if(cJSON_IsString(jsonValue))                   /*  Judge key Of value Is it string */
			{
				replacement =cJSON_CreateString((char *)ConfigValue);
			}else
			{
				replacement =cJSON_CreateNumber(*((int*)ConfigValue));
			}

			cJSON_ReplaceItemInObject(item, ptrParmName, replacement);

			char *p2 = cJSON_Print(root);  /*  Output... In format json  cJSON_PrintUnformatted Indicates that the formatted output is a long string  */
			if(p2 != NULL)
			{
				fprintf(OutputFile, "%s", p2);
				free(p2);
				p2 = NULL;
			}
		}
	}
	fclose(OutputFile);
	rename("temp_file_path.json", ptrFileName);

	cJSON_Delete(root);
	return ERROR_OK;
}

ERROR_T SetConfigParam(char *ptrFileName, char *ptrObjName, char *ptrParamName,void *ConfigValue)
{
	char *pcBuff 					= NULL;
	FILE *fp 						= NULL;
	unsigned int nFileReadBytes 	= 0;
	struct stat status;
	printf("\nptrFileName= %s ptrObjName= %s ptrParamName= %s",ptrFileName,ptrObjName,ptrParamName);
  	if(0 != stat(ptrFileName, &status))
    {
        printf("Modbus Server Configuration file doesn't exist \r\n");
        return ERROR_FAIL;
    }
    else if(NULL == (fp = fopen(ptrFileName, "r")))
    {
    	printf("Could not open Modbus Server Configuration file \r\n");
    	return ERROR_FAIL;
    }
    else
    {
		if(0 != fseek(fp, 0, SEEK_SET))
		{
			printf("Could not set Modbus Server Configuration file to read \r\n");
			return ERROR_FAIL;
		}
		else
		{

			/*malloc memory*/
			pcBuff = malloc(status.st_size+1);
			if(NULL == pcBuff)
			{
				printf("Failed to malloc for Modbus Server configuration file \r\n");
				return ERROR_FAIL;
			}
			memset(pcBuff, 0x00, status.st_size+1);

			nFileReadBytes = fread(pcBuff, 1, status.st_size, fp);
			if(0 == nFileReadBytes)
			{
				printf("Did not read Modbus Server configuration file \r\n");
				return ERROR_FAIL;
			}
		}
		fclose(fp);
    }

    Set_ParseConfigParamJson(ptrFileName, ptrObjName, pcBuff, ptrParamName, ConfigValue);
    free(pcBuff);

    return ERROR_OK;
}


ERROR_T get_PortName(char *PortName)
{
	ERROR_T retval=ERROR_OK;

	retval=GetConfigParam("/etc/default/ModbusRTUServer.json", "config", "PortName", PortName);
	return retval;
}

ERROR_T get_ParityValue(char *Parity)
{
	ERROR_T retval=ERROR_OK;

	retval=GetConfigParam("/etc/default/ModbusRTUServer.json", "config", "Parity", Parity);
	return retval;
}

ERROR_T get_BaudrateValue(uint32_t *Baudrate)
{
	ERROR_T retval=ERROR_OK;

	retval=retval=GetConfigParam("/etc/default/ModbusRTUServer.json", "config", "Baudrate", Baudrate);
	return retval;
}

ERROR_T get_IP_Address(uint8_t *IP_Address)
{
	ERROR_T retval=ERROR_OK;

	retval=GetConfigParam("/etc/default/ModbusTCPServer.json", "config", "IP_Address", (uint8_t *)IP_Address);
	return retval;
}

ERROR_T get_PortNumber(uint16_t *PortNumber)
{
	ERROR_T retval=ERROR_OK;

	retval=GetConfigParam("/etc/default/ModbusTCPServer.json", "config", "PortNumber", (uint16_t *)PortNumber);
	return retval;
}

ERROR_T get_Slave_ID(uint8_t *Slave_ID)
{
	ERROR_T retval=ERROR_OK;

#ifdef MODBUS_STACK_TCPIP_ENABLED
	retval=GetConfigParam("/etc/default/ModbusTCPServer.json", "config", "Slave_ID", (uint8_t *)Slave_ID);
#else
	retval=GetConfigParam("/etc/default/ModbusRTUServer.json", "config", "Slave_ID", (uint8_t *)Slave_ID);
#endif
	return retval;
}


ERROR_T set_PortName(char *PortName)
{
	ERROR_T retval=ERROR_OK;

	retval=SetConfigParam("/etc/default/ModbusRTUServer.json", "config", "PortName", PortName);
	return retval;
}

ERROR_T set_ParityValue(char *Parity)
{
	ERROR_T retval=ERROR_OK;

	retval=SetConfigParam("/etc/default/ModbusRTUServer.json", "config", "Parity", Parity);
	return retval;
}

ERROR_T set_BaudrateValue(int Baudrate)
{
	ERROR_T retval=ERROR_OK;

	retval=SetConfigParam("/etc/default/ModbusRTUServer.json", "config", "Baudrate", (void *)&Baudrate);
	return retval;
}

ERROR_T set_IP_Address(uint8_t *IP_Address)
{
	ERROR_T retval=ERROR_OK;

	retval=SetConfigParam("/etc/default/ModbusTCPServer.json", "config", "IP_Address", IP_Address);
	return retval;
}

ERROR_T set_PortNumber(int PortNumber)
{
	ERROR_T retval=ERROR_OK;

	retval=SetConfigParam("/etc/default/ModbusTCPServer.json", "config", "PortNumber", (void *)&PortNumber);
	return retval;
}

ERROR_T set_Slave_ID(int Slave_ID)
{
	ERROR_T retval=ERROR_OK;

#ifdef MODBUS_STACK_TCPIP_ENABLED
	retval=SetConfigParam("/etc/default/ModbusTCPServer.json", "config", "Slave_ID", (void *)&Slave_ID);
#else
	retval=SetConfigParam("/etc/default/ModbusRTUServer.json", "config", "Slave_ID", (void *)&Slave_ID);
#endif

	return retval;
}
