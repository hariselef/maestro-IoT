################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Projects/STEVAL-STRKT01/Applications/LoRa/Asset_Tracker/SW4STM32/startup_stm32l072xx.s 

OBJS += \
./Application/SW4STM32/startup_stm32l072xx.o 

S_DEPS += \
./Application/SW4STM32/startup_stm32l072xx.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32l072xx.o: /home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Projects/STEVAL-STRKT01/Applications/LoRa/Asset_Tracker/SW4STM32/startup_stm32l072xx.s Application/SW4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -c -I../../../Core/inc -I../../../LoRaWAN/App/inc -I../../../../../../../../Drivers/BSP/CMWX1ZZABZ-0xx -I../../../../../../../../Drivers/STM32L0xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Phy -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lis2dw12 -I../../../../../../../../Drivers/BSP/Components/teseoIII -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/m95m02_dr -I../../../../../../../../Drivers/BSP/Components/sx1276 -I../../../../../../../../Drivers/BSP/Components/w752535 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A1 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A2 -I../../../../../../../../Drivers/BSP/X-NUCLEO-GNSS1A1 -I../../../../../../../../Drivers/BSP/LoRaTracker -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibGNSS/Inc -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibNMEA/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Application-2f-SW4STM32

clean-Application-2f-SW4STM32:
	-$(RM) ./Application/SW4STM32/startup_stm32l072xx.d ./Application/SW4STM32/startup_stm32l072xx.o

.PHONY: clean-Application-2f-SW4STM32

