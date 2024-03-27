################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/comm.c \
../Core/Src/current.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/mccCommand.c \
../Core/Src/pwmCommand.c \
../Core/Src/speed.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tickTimer.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/util.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/comm.o \
./Core/Src/current.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/mccCommand.o \
./Core/Src/pwmCommand.o \
./Core/Src/speed.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tickTimer.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/util.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/comm.d \
./Core/Src/current.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/mccCommand.d \
./Core/Src/pwmCommand.d \
./Core/Src/speed.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tickTimer.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/util.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/comm.cyclo ./Core/Src/comm.d ./Core/Src/comm.o ./Core/Src/comm.su ./Core/Src/current.cyclo ./Core/Src/current.d ./Core/Src/current.o ./Core/Src/current.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mccCommand.cyclo ./Core/Src/mccCommand.d ./Core/Src/mccCommand.o ./Core/Src/mccCommand.su ./Core/Src/pwmCommand.cyclo ./Core/Src/pwmCommand.d ./Core/Src/pwmCommand.o ./Core/Src/pwmCommand.su ./Core/Src/speed.cyclo ./Core/Src/speed.d ./Core/Src/speed.o ./Core/Src/speed.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tickTimer.cyclo ./Core/Src/tickTimer.d ./Core/Src/tickTimer.o ./Core/Src/tickTimer.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/util.cyclo ./Core/Src/util.d ./Core/Src/util.o ./Core/Src/util.su

.PHONY: clean-Core-2f-Src

