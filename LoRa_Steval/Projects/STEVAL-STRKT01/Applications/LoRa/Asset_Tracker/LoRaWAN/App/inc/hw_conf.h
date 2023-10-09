/**
  ******************************************************************************
  * @file    hw_conf.h
  * @author  MCD Application Team
  * @brief   contains hardware configuration Macros and Constants
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONF_H__
#define __HW_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#ifdef USE_STM32F0XX_NUCLEO
  #include "stm32f0xx_hal.h"
  #include "stm32f0xx_nucleo.h"
  #include "stm32f0xx_hal_conf.h"
  #include "stm32f0xx_ll_rtc.h"
  #error "create stm32f0xx_hw_conf.h "
#endif

#ifdef USE_STM32F1XX_NUCLEO
  #include "stm32f1xx_hal.h"
  #include "stm32f1xx_nucleo.h"
  #include "stm32f1xx_hal_conf.h"
  #include "stm32f1xx_ll_rtc.h"
  #error "create stm32f1xx_hw_conf.h "
#endif

#ifdef USE_STM32F3XX_NUCLEO
  #include "stm32f3xx_hal.h"
  #include "stm32f3xx_nucleo.h"
  #include "stm32f3xx_hal_conf.h"
  #include "stm32f3xx_ll_rtc.h"
  #error "create stm32f3xx_hw_conf.h "
#endif

#ifdef USE_STM32F4XX_NUCLEO
  #include "stm32f4xx_hal.h"
  #include "stm32f4xx_nucleo.h"
  #include "stm32f4xx_hal_conf.h"
  #include "stm32f4xx_ll_rtc.h"
  #error "create stm32f4xx_hw_conf.h "
#endif

#ifdef USE_STM32L0XX_NUCLEO
  #include "stm32l0xx_hal.h"
  #include "stm32l0xx_nucleo.h"
  #include "stm32l0xx_hal_conf.h"
  #include "stm32l0xx_hw_conf.h"
  #include "stm32l0xx_ll_rtc.h"
#endif

#ifdef USE_STM32L1XX_NUCLEO
  #include "stm32l1xx_hal.h"
  #include "stm32l1xx_nucleo.h"
  #include "stm32l1xx_hal_conf.h"
  #include "stm32l1xx_hw_conf.h"
  #include "stm32l1xx_ll_rtc.h"
#endif

#ifdef USE_STM32L4XX_NUCLEO
  #include "stm32l4xx_hal.h"
  #include "stm32l4xx_nucleo.h"
  #include "stm32l4xx_hal_conf.h"
  #include "stm32l4xx_hw_conf.h"
  #include "stm32l4xx_ll_rtc.h"
#endif

#ifdef USE_B_L072Z_LRWAN1
  #include "stm32l0xx_hal.h"
  #include "b-l072z-lrwan1.h"
  #include "stm32l0xx_hal_conf.h"
  #include "mlm32l0xx_hw_conf.h"
  #include "stm32l0xx_ll_rtc.h"
#endif

#ifdef LORA_SMART_TRACKER
  #include "stm32l0xx_hal.h"
  #include "LoRaTracker.h"
  #include "stm32l0xx_hal_conf.h"
  #include "mlm32l0xx_hw_conf.h"
  #include "stm32l0xx_ll_rtc.h"
  #include "stm32l0xx_ll_usb.h"
#endif

/* --------Preprocessor compile swicth------------ */
/* debug swicth in debug.h */
/** @addtogroup COMPILER_SWITCHES Compiler switches
  * @{
  */    
   
/* --------Preprocessor compile swicth------------ */

/* debug swicthes in debug.h */
#ifdef LORA_SMART_TRACKER
#define DEBUG
#define SENSOR_ENABLED
#define GNSS_ENABLED
//#define LOW_POWER_DISABLE
//#define BLOCK_ON_ERROR
//#define TRACE
#define USB_ENABLED
#define MEMORY_ENABLED
#define USE_TCXO_VDD                                    0   /* Use PA.12 to supply the TCXO. Allowed values: 0 (default) disabled , 1 enabled. Note: when enabled, USB_ENABLED must not be defined*/
#define CHECK_DEBUGGER_CONNECTION                       0   /* Check the debbugger connection. Allowed values: 0 (default) disabled , 1 enabled */
#define SAVE_LORA_KEYS_IN_INTERNAL_EEPROM               1   /* Storing LoRa keys in EEPROM. Allowed values: 0 (default) disabled , 1 enabled */
#define SAVE_PLATFORM_STATUS_IN_INTERNAL_EEPROM         1   /* Storing platform status in EEPROM. Allowed values: 0 (default) disabled , 1 enabled */
#define GNSS_STANDBY_WAKEUP                             0   /* During low power, the TESEO-LIV3F can be either in standby or completely OFF (VDD off) Allowed values: 1 (default) GNSS in standby, 0 GNSS VDD off*/
   
#ifdef USB_ENABLED
#if USE_TCXO_VDD
#warning If USB is used, TCXO_VDD cannot be driven by PA.12
#endif //USE_TCXO_VDD
#endif //USB_ENABLED
   
#else //LORA_SMART_TRACKER
//#define DEBUG
   
/* uncomment below line to never enter lowpower modes in main.c*/
//#define LOW_POWER_DISABLE

/* debug swicthes in bsp.c */
#define SENSOR_ENABLED
#define GNSS_ENABLED
   
#endif //LORA_SMART_TRACKER
   
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __HW_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
