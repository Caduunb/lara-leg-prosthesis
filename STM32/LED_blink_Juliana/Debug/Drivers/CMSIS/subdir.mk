################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Src/system_stm32f1xx.c 

OBJS += \
./Drivers/CMSIS/system_stm32f1xx.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32f1xx.o: C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Src/system_stm32f1xx.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Inc" -I"C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/lemos/Documents/stm32projects/LED_blink_Juliana/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


