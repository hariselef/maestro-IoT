################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Middlewares/ST/STM32_GNSS/LibNMEA/Src/NMEA_parser.c 

OBJS += \
./Middlewares/STM32_GNSS/LibNMEA/NMEA_parser.o 

C_DEPS += \
./Middlewares/STM32_GNSS/LibNMEA/NMEA_parser.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_GNSS/LibNMEA/NMEA_parser.o: /home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Middlewares/ST/STM32_GNSS/LibNMEA/Src/NMEA_parser.c Middlewares/STM32_GNSS/LibNMEA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=c99 -g -DREGION_EU868 -DLORA_SMART_TRACKER -DSTM32L072xx -DUSE_HAL_DRIVER -DIOT_TRACKER_APP_VER=0x00002200 -DUSE_HAL_PCD_REGISTER_CALLBACKS=1 -c -I../../../Core/inc -I../../../LoRaWAN/App/inc -I../../../../../../../../Drivers/BSP/CMWX1ZZABZ-0xx -I../../../../../../../../Drivers/STM32L0xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Phy -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lis2dw12 -I../../../../../../../../Drivers/BSP/Components/teseoIII -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/m95m02_dr -I../../../../../../../../Drivers/BSP/Components/sx1276 -I../../../../../../../../Drivers/BSP/Components/w752535 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A1 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A2 -I../../../../../../../../Drivers/BSP/X-NUCLEO-GNSS1A1 -I../../../../../../../../Drivers/BSP/LoRaTracker -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibGNSS/Inc -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibNMEA/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -O2 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-STM32_GNSS-2f-LibNMEA

clean-Middlewares-2f-STM32_GNSS-2f-LibNMEA:
	-$(RM) ./Middlewares/STM32_GNSS/LibNMEA/NMEA_parser.d ./Middlewares/STM32_GNSS/LibNMEA/NMEA_parser.o ./Middlewares/STM32_GNSS/LibNMEA/NMEA_parser.su

.PHONY: clean-Middlewares-2f-STM32_GNSS-2f-LibNMEA

