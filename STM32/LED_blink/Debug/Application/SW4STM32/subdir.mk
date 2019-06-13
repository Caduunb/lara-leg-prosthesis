################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
C:/Users/Cadu/Documents/GitHub/lara-leg-prosthesis/GitHub/lara-leg-prosthesis/STM32/startup_stm32f103xb.s 

C_SRCS += \
C:/Users/Cadu/Documents/GitHub/lara-leg-prosthesis/GitHub/lara-leg-prosthesis/STM32/syscalls.c 

OBJS += \
./Application/SW4STM32/startup_stm32f103xb.o \
./Application/SW4STM32/syscalls.o 

C_DEPS += \
./Application/SW4STM32/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32f103xb.o: C:/Users/Cadu/Documents/GitHub/lara-leg-prosthesis/GitHub/lara-leg-prosthesis/STM32/startup_stm32f103xb.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/SW4STM32/syscalls.o: C:/Users/Cadu/Documents/GitHub/lara-leg-prosthesis/GitHub/lara-leg-prosthesis/STM32/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I../../../Inc -I../../../Drivers/STM32F1xx_HAL_Driver/Inc -I../../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../../../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../../../Drivers/CMSIS/Include -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


