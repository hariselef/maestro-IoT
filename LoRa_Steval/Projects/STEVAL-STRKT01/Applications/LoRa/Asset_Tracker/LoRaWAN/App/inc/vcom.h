/**
  ******************************************************************************
  * @file    vcom.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    10-July-2018
  * @brief   Header for vcom.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VCOM_H__
#define __VCOM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/** 
* @brief  init vcom 
* @param  callback when Tx buffer has been sent
* @return None
*/
void vcom_Init(  void (*Txcb)(void) ); 
  
/** 
* @brief  send buffer @p_data of size size to vcom in dma mode
* @param  p_data data to be sent
* @param  szie of buffer p_data to be sent
* @return None
*/
void vcom_Trace(  uint8_t *p_data, uint16_t size );

   /** 
* @brief  DeInit the VCOM.
* @param  None
* @return None
*/
void vcom_DeInit(void);

   /** 
* @brief  Init the VCOM IOs.
* @param  None
* @return None
*/
void vcom_IoInit(void);
  
/** 
* @brief  DeInit the VCOM IOs.
* @param  None
* @return None
*/
void vcom_IoDeInit(void);

/** 
* @brief  last byte has been sent on the uart line
* @param  None
* @return None
*/
void vcom_IRQHandler(void);

/** 
* @brief  last byte has been sent from memeory to uart data register
* @param  None
* @return None
*/
void vcom_DMA_TX_IRQHandler(void);

/** 
* @brief  Records string on circular Buffer and set SW interrupt
* @note   Set NVIC to call vcom_Send
* @param  string
* @return None
*/
void vcom_Send( char *format, ... );

/** 
* @brief  Records string on circular Buffer
* @note   To be called only from critical section from low power section
*         Other wise use vcom_Send
* @param  string
* @return None
*/
void vcom_Send_Lp( char *format, ... );

/* Exported macros -----------------------------------------------------------*/
//#ifdef LORA_SMART_TRACKER
////#define PRINTF(...)            vcom_Send(__VA_ARGS__)
//#ifdef USB_ENABLED
//  #define PRINTF(...)            do{DBG_PRINTF_LOG(0, __VA_ARGS__);extern uint8_t bDebuggerConnected;if(bDebuggerConnected)printf(__VA_ARGS__);}while(0)
//#else //USB_ENABLED
//  #define PRINTF(...)            do{extern uint8_t bDebuggerConnected; if(bDebuggerConnected)printf(__VA_ARGS__);}while(0)
//#endif //USB_ENABLED
////#define PRINTF(...)
//#else
//#define PRINTF(...)            vcom_Send(__VA_ARGS__)
//#endif

#ifdef __cplusplus
}
#endif

#endif /* __VCOM_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
