/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Bleeper board SPI driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
/**
  ******************************************************************************
  * @file    hw_spi.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    10-July-2018
  * @brief   manages the SPI interface
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

/** @addtogroup PROJECTS PROJECTS
  * @{
  */
   
/** @addtogroup STEVAL-STRKT01 STEVAL-STRKT01
  * @{
  */  
   
/** @addtogroup STEVAL-STRKT01_ASSET_TRACKER Asset Tracker
  * @{
  */    
    
/** @addtogroup STEVAL-STRKT01_LoRaWAN LoRaWAN
  * @{
 */

/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "utilities.h"
#define PRINTF_MAIN1(...)                       PRINTF(__VA_ARGS__)


#define SPIx                             SPI2
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()


/* Definition for SPIx Pins */


#define SPIx_SCK_PIN                     GPIO_PIN_13
#define SPIx_SCK_GPIO_PORT               GPIOB
#define SPIx_SCK_AF                      GPIO_AF5_SPI2
#define SPIx_MISO_PIN                    GPIO_PIN_14
#define SPIx_MISO_GPIO_PORT              GPIOB
#define SPIx_MISO_AF                     GPIO_AF5_SPI2
#define SPIx_MOSI_PIN                    GPIO_PIN_15
#define SPIx_MOSI_GPIO_PORT              GPIOB
#define SPIx_MOSI_AF                     GPIO_AF5_SPI2

/* Size of buffer */
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef hspi;

//ADDS
static SPI_HandleTypeDef hspi2;


/* Private function prototypes -----------------------------------------------*/

/*!
 * @brief Calculates Spi Divisor based on Spi Frequency and Mcu Frequency
 *
 * @param [IN] Spi Frequency
 * @retval Spi divisor
 */
static uint32_t SpiFrequency( uint32_t hz );

/* Exported functions ---------------------------------------------------------*/
/* Buffer used for transmission */
uint8_t aRxBuffer = 0x6FU;
uint8_t aTxBuffer = 0xA1U;
uint8_t nucleo_data[4];
HAL_StatusTypeDef status = HAL_OK;

/*!
 * @brief Initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */

HAL_SPI_StateTypeDef structspi2;

void HW_SPI_Init( void )
{
  
  /* Configuration of the SPI peripheral */
  /* Set the SPI parameters */

  hspi.Instance = SPI1;

  hspi.Init.BaudRatePrescaler = SpiFrequency( 10000000 );
  hspi.Init.Direction      = SPI_DIRECTION_2LINES;
  hspi.Init.Mode           = SPI_MODE_MASTER;
  hspi.Init.CLKPolarity    = SPI_POLARITY_LOW;
  hspi.Init.CLKPhase       = SPI_PHASE_1EDGE;
  hspi.Init.DataSize       = SPI_DATASIZE_8BIT;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;  
  hspi.Init.FirstBit       = SPI_FIRSTBIT_MSB;
  hspi.Init.NSS            = SPI_NSS_SOFT;
  hspi.Init.TIMode         = SPI_TIMODE_DISABLE;


  hspi2.Instance = SPI2;
  hspi2.Init.BaudRatePrescaler = SpiFrequency( 10000000 );
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

  SPI_CLK_ENABLE(); 

  SPIx_CLK_ENABLE();


  if(HAL_SPI_Init( &hspi) != HAL_OK)
  {
    /* Initialization Error */
     Error_Handler();
  }

  if(HAL_SPI_Init( &hspi2) != HAL_OK)
    {
      /* Initialization Error */
       Error_Handler();
    }

  /* Configuration of the SPI GPIOs */
  HW_SPI_IoInit(  );
}

/*!
 * @brief De-initializes the SPI object and MCU peripheral
 *
 * @param [IN] none
 */
void HW_SPI_DeInit( void )
{

  HAL_SPI_DeInit( &hspi);

  /* Reset of the peripheral */
  __HAL_RCC_SPI1_FORCE_RESET();
  __HAL_RCC_SPI1_RELEASE_RESET();
  
  SPIx_FORCE_RESET();
  SPIx_RELEASE_RESET();

  /* Reset the SPI GPIOs */
  HW_SPI_IoDeInit( );
}

void HW_SPI_IoInit( void )
{
  GPIO_InitTypeDef initStruct={0};
  

  initStruct.Mode =GPIO_MODE_AF_PP;
  initStruct.Pull =GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;
  initStruct.Alternate= SPI1_AF;

  HW_GPIO_Init( RADIO_SCLK_PORT, RADIO_SCLK_PIN, &initStruct);
  HW_GPIO_Init( RADIO_MISO_PORT, RADIO_MISO_PIN, &initStruct);
  HW_GPIO_Init( RADIO_MOSI_PORT, RADIO_MOSI_PIN, &initStruct);

  initStruct.Mode = GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;

  HW_GPIO_Init(  RADIO_NSS_PORT, RADIO_NSS_PIN, &initStruct );

  HW_GPIO_Write ( RADIO_NSS_PORT, RADIO_NSS_PIN, 1 );



   GPIO_InitTypeDef initStruct2={0};





    initStruct2.Mode =GPIO_MODE_AF_PP;
    initStruct2.Pull =GPIO_NOPULL;
    initStruct2.Speed = GPIO_SPEED_HIGH;
    initStruct2.Alternate= GPIO_AF0_SPI2 ;

    HW_GPIO_Init( GPIOB, GPIO_PIN_13, &initStruct2);
    HW_GPIO_Init( GPIOB, GPIO_PIN_14, &initStruct2);
    HW_GPIO_Init( GPIOB, GPIO_PIN_15, &initStruct2);

    initStruct2.Mode = GPIO_MODE_OUTPUT_PP;
    initStruct2.Pull = GPIO_NOPULL;

    HW_GPIO_Init(  GPIOB, GPIO_PIN_12, &initStruct2 );

    HW_GPIO_Write ( GPIOB, GPIO_PIN_12, 0);



}

void HW_SPI_IoDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};
    
  initStruct.Mode =GPIO_MODE_OUTPUT_PP;

  initStruct.Pull = GPIO_NOPULL;
  HW_GPIO_Init ( RADIO_MOSI_PORT, RADIO_MOSI_PIN, &initStruct ); 
  HW_GPIO_Write( RADIO_MOSI_PORT, RADIO_MOSI_PIN, 0 );
  
  initStruct.Pull = GPIO_NOPULL; 
  HW_GPIO_Init ( RADIO_MISO_PORT, RADIO_MISO_PIN, &initStruct ); 
  HW_GPIO_Write( RADIO_MISO_PORT, RADIO_MISO_PIN, 0 );
  
  initStruct.Pull = GPIO_NOPULL; 
  HW_GPIO_Init ( RADIO_SCLK_PORT, RADIO_SCLK_PIN, &initStruct ); 
  HW_GPIO_Write(  RADIO_SCLK_PORT, RADIO_SCLK_PIN, 0 );

  initStruct.Pull =GPIO_NOPULL  ; 
  HW_GPIO_Init ( RADIO_NSS_PORT, RADIO_NSS_PIN , &initStruct ); 
  HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN , 1 );


  GPIO_InitTypeDef initStruct2={0};

  initStruct2.Mode =GPIO_MODE_OUTPUT_PP;

  initStruct2.Pull = GPIO_NOPULL;
  HW_GPIO_Init ( GPIOB, GPIO_PIN_15, &initStruct2 );
  HW_GPIO_Write( GPIOB, GPIO_PIN_15, 0 );

  initStruct2.Pull = GPIO_NOPULL;
  HW_GPIO_Init ( GPIOB, GPIO_PIN_14, &initStruct2 );
  HW_GPIO_Write( GPIOB, GPIO_PIN_14, 0 );

  initStruct2.Pull = GPIO_NOPULL;
  HW_GPIO_Init ( GPIOB, GPIO_PIN_13, &initStruct2 );
  HW_GPIO_Write(  GPIOB, GPIO_PIN_13, 0 );

  initStruct2.Pull =GPIO_NOPULL  ;
  HW_GPIO_Init ( GPIOB, GPIO_PIN_12 , &initStruct2 );
  HW_GPIO_Write( GPIOB, GPIO_PIN_12 , 1 );

}


void HW_SPI_InOut_Message( void ) {


	HAL_Delay(3000);


	 //PRINTF_MAIN1("MASTER: i sent: %d\r\n", aTxBuffer);
	 //NSS = 0;
	 HW_GPIO_Write( GPIOB, GPIO_PIN_12, 0 );


	 status=HAL_SPI_TransmitReceive( &hspi2, (uint8_t*)&aTxBuffer, (uint8_t*)&aRxBuffer, 1, HAL_MAX_DELAY);
	 PRINTF_MAIN1("%d\r\n",status);
	 PRINTF_MAIN1("%d - rx: %d\r\n",aTxBuffer, aRxBuffer);

	 nucleo_data[aTxBuffer-1]= aRxBuffer;

	 if (status  != HAL_OK)
	 {
		PRINTF_MAIN1("Error_Handler\r\n");
	 	Error_Handler();
	 }

	 //status=HAL_SPI_TransmitReceive( &hspi2, (uint8_t*)&aTxBuffer, (uint8_t*)&aRxBuffer, 1, HAL_MAX_DELAY);

	// PRINTF_MAIN1("MASTER: i got: %d\r\n", aRxBuffer);
	 //NSS = 1;

	// aRxBuffer = 0x0;

	 HW_GPIO_Write( GPIOB, GPIO_PIN_12, 1 );



	 HAL_Delay(1000);

}


void receive_SPI_data() {
	aRxBuffer = 0x0;
	aTxBuffer = 0x0;
	//HW_SPI_InOut_Message();
	for(int i=0; i<5; i++){
		HW_SPI_InOut_Message();
		aTxBuffer++;
	}
}

uint8_t get_nucleo_data(int index) {

	return nucleo_data[index];
}

/*
void SX1276WriteBuffer( uint16_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 0 );

    HW_SPI_InOut( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        HW_SPI_InOut( buffer[i] );
    }

    //NSS = 1;
    HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 1 );
}

void SX1276ReadBuffer( uint16_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 0 );

    HW_SPI_InOut( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = HW_SPI_InOut( 0 );
    }

    //NSS = 1;
    HW_GPIO_Write( RADIO_NSS_PORT, RADIO_NSS_PIN, 1 );
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}
*/

/*!
 * @brief Sends outData and receives inData
 *
 * @param [IN] outData Byte to be sent
 * @retval inData      Received byte.
 */
uint16_t HW_SPI_InOut( uint16_t txData )
{
  uint16_t rxData ;

  HAL_SPI_TransmitReceive( &hspi, ( uint8_t * ) &txData, ( uint8_t* ) &rxData, 1, HAL_MAX_DELAY);	

  return rxData;
}

/* Private functions ---------------------------------------------------------*/

static uint32_t SpiFrequency( uint32_t hz )
{
  uint32_t divisor = 0;
  uint32_t SysClkTmp = SystemCoreClock;
  uint32_t baudRate;
  
  while( SysClkTmp > hz)
  {
    divisor++;
    SysClkTmp= ( SysClkTmp >> 1);
    
    if (divisor >= 7)
      break;
  }
  
  baudRate =((( divisor & 0x4 ) == 0 )? 0x0 : SPI_CR1_BR_2  )| 
            ((( divisor & 0x2 ) == 0 )? 0x0 : SPI_CR1_BR_1  )| 
            ((( divisor & 0x1 ) == 0 )? 0x0 : SPI_CR1_BR_0  );
  
  return baudRate;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
