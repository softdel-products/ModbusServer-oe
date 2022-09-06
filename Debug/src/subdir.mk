################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Data_Link_Layer.c \
../src/modbus-data.c \
../src/ModbusExporttedAPI.c 

OBJS += \
./src/Data_Link_Layer.o \
./src/modbus-data.o \
./src/ModbusExporttedAPI.o 

C_DEPS += \
./src/Data_Link_Layer.d \
./src/modbus-data.d \
./src/ModbusExporttedAPI.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	aarch64-tdx-linux-gcc -DMODBUS_STACK_TCPIP_ENABLED -fstack-protector-strong  -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security --sysroot=/opt/tdx-xwayland/5.7.0/sysroots/aarch64-tdx-linux -fPIC -I"/home/ubuntu/Desktop/sample/Modbus_Linux_29Aug2022/Modbus/Modbus_Server_Stack/inc" -O0 -g3 -Wall -O2 -pipe -g -feliminate-unused-debug-types  -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


