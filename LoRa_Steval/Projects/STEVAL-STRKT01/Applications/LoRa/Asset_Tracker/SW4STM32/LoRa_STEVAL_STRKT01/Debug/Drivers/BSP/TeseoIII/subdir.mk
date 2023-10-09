################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_i2c.c \
/home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_io.c \
/home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_queue.c \
/home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_uart.c 

OBJS += \
./Drivers/BSP/TeseoIII/teseo_i2c.o \
./Drivers/BSP/TeseoIII/teseo_io.o \
./Drivers/BSP/TeseoIII/teseo_queue.o \
./Drivers/BSP/TeseoIII/teseo_uart.o 

C_DEPS += \
./Drivers/BSP/TeseoIII/teseo_i2c.d \
./Drivers/BSP/TeseoIII/teseo_io.d \
./Drivers/BSP/TeseoIII/teseo_queue.d \
./Drivers/BSP/TeseoIII/teseo_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/TeseoIII/teseo_i2c.o: /home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_i2c.c Drivers/BSP/TeseoIII/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=c99 -g -DREGION_EU868 -DLORA_SMART_TRACKER -DSTM32L072xx -DUSE_HAL_DRIVER -DIOT_TRACKER_APP_VER=0x00002200 -DUSE_HAL_PCD_REGISTER_CALLBACKS=1 -c -I../../../Core/inc -I../../../LoRaWAN/App/inc -I../../../../../../../../Drivers/BSP/CMWX1ZZABZ-0xx -I../../../../../../../../Drivers/STM32L0xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Phy -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lis2dw12 -I../../../../../../../../Drivers/BSP/Components/teseoIII -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/m95m02_dr -I../../../../../../../../Drivers/BSP/Components/sx1276 -I../../../../../../../../Drivers/BSP/Components/w752535 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A1 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A2 -I../../../../../../../../Drivers/BSP/X-NUCLEO-GNSS1A1 -I../../../../../../../../Drivers/BSP/LoRaTracker -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibGNSS/Inc -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibNMEA/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -O2 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/BSP/TeseoIII/teseo_io.o: /home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_io.c Drivers/BSP/TeseoIII/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=c99 -g -DREGION_EU868 -DLORA_SMART_TRACKER -DSTM32L072xx -DUSE_HAL_DRIVER -DIOT_TRACKER_APP_VER=0x00002200 -DUSE_HAL_PCD_REGISTER_CALLBACKS=1 -c -I../../../Core/inc -I../../../LoRaWAN/App/inc -I../../../../../../../../Drivers/BSP/CMWX1ZZABZ-0xx -I../../../../../../../../Drivers/STM32L0xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Phy -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lis2dw12 -I../../../../../../../../Drivers/BSP/Components/teseoIII -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/m95m02_dr -I../../../../../../../../Drivers/BSP/Components/sx1276 -I../../../../../../../../Drivers/BSP/Components/w752535 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A1 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A2 -I../../../../../../../../Drivers/BSP/X-NUCLEO-GNSS1A1 -I../../../../../../../../Drivers/BSP/LoRaTracker -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibGNSS/Inc -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibNMEA/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -O2 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/BSP/TeseoIII/teseo_queue.o: /home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_queue.c Drivers/BSP/TeseoIII/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=c99 -g -DREGION_EU868 -DLORA_SMART_TRACKER -DSTM32L072xx -DUSE_HAL_DRIVER -DIOT_TRACKER_APP_VER=0x00002200 -DUSE_HAL_PCD_REGISTER_CALLBACKS=1 -c -I../../../Core/inc -I../../../LoRaWAN/App/inc -I../../../../../../../../Drivers/BSP/CMWX1ZZABZ-0xx -I../../../../../../../../Drivers/STM32L0xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Phy -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lis2dw12 -I../../../../../../../../Drivers/BSP/Components/teseoIII -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/m95m02_dr -I../../../../../../../../Drivers/BSP/Components/sx1276 -I../../../../../../../../Drivers/BSP/Components/w752535 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A1 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A2 -I../../../../../../../../Drivers/BSP/X-NUCLEO-GNSS1A1 -I../../../../../../../../Drivers/BSP/LoRaTracker -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibGNSS/Inc -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibNMEA/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -O2 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/BSP/TeseoIII/teseo_uart.o: /home/nestorid/Downloads/en.fp-atr-lora1/STM32CubeFunctionPack_LORA1_V2.2.0/Drivers/BSP/Components/teseoIII/teseo_uart.c Drivers/BSP/TeseoIII/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=c99 -g -DREGION_EU868 -DLORA_SMART_TRACKER -DSTM32L072xx -DUSE_HAL_DRIVER -DIOT_TRACKER_APP_VER=0x00002200 -DUSE_HAL_PCD_REGISTER_CALLBACKS=1 -c -I../../../Core/inc -I../../../LoRaWAN/App/inc -I../../../../../../../../Drivers/BSP/CMWX1ZZABZ-0xx -I../../../../../../../../Drivers/STM32L0xx_HAL_Driver/Inc -I../../../../../../../../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Crypto -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Phy -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Utilities -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Patterns/Basic -I../../../../../../../../Drivers/BSP/Components/Common -I../../../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../../../Drivers/BSP/Components/lps22hb -I../../../../../../../../Drivers/BSP/Components/lis2dw12 -I../../../../../../../../Drivers/BSP/Components/teseoIII -I../../../../../../../../Drivers/BSP/Components/lsm6dsl -I../../../../../../../../Drivers/BSP/Components/m95m02_dr -I../../../../../../../../Drivers/BSP/Components/sx1276 -I../../../../../../../../Drivers/BSP/Components/w752535 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A1 -I../../../../../../../../Drivers/BSP/X_NUCLEO_IKS01A2 -I../../../../../../../../Drivers/BSP/X-NUCLEO-GNSS1A1 -I../../../../../../../../Drivers/BSP/LoRaTracker -I../../../../../../../../Middlewares/Third_Party/LoRaWAN/Mac/region -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibGNSS/Inc -I../../../../../../../../Middlewares/ST/STM32_GNSS/LibNMEA/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -O2 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-TeseoIII

clean-Drivers-2f-BSP-2f-TeseoIII:
	-$(RM) ./Drivers/BSP/TeseoIII/teseo_i2c.d ./Drivers/BSP/TeseoIII/teseo_i2c.o ./Drivers/BSP/TeseoIII/teseo_i2c.su ./Drivers/BSP/TeseoIII/teseo_io.d ./Drivers/BSP/TeseoIII/teseo_io.o ./Drivers/BSP/TeseoIII/teseo_io.su ./Drivers/BSP/TeseoIII/teseo_queue.d ./Drivers/BSP/TeseoIII/teseo_queue.o ./Drivers/BSP/TeseoIII/teseo_queue.su ./Drivers/BSP/TeseoIII/teseo_uart.d ./Drivers/BSP/TeseoIII/teseo_uart.o ./Drivers/BSP/TeseoIII/teseo_uart.su

.PHONY: clean-Drivers-2f-BSP-2f-TeseoIII

