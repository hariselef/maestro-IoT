/*!
 * \file      sx1276-board.c
 *
 * \brief     Target board SX1276 driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
/**
  *******************************************************************************
  * @file    mlm32l07x01.c
  * @author  MCD Application Team
  * @brief   driver LoRa module murata cmwx1zzabz-078
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/

#include "hw.h"
#include "radio.h"
#include "sx1276.h"
#include "mlm32l07x01.h"


#define IRQ_HIGH_PRIORITY  0

#if USE_TCXO_VDD

#define TCXO_ON()   HW_GPIO_Write( RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, 1) 

#define TCXO_OFF()  HW_GPIO_Write( RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, 0) 

#else //USE_TCXO_VDD

  //TCXO_VCC is straight connected to LORA_VDD on LORA tracker
  #define TCXO_ON()
  #define TCXO_OFF()

#endif //USE_TCXO_VDD

static void SX1276AntSwInit( void );

static void SX1276AntSwDeInit( void );

void SX1276SetXO( uint8_t state );

uint32_t SX1276GetWakeTime( void );

void SX1276IoIrqInit( DioIrqHandler **irqHandlers );

uint8_t SX1276GetPaSelect( int8_t power );

void SX1276SetAntSwLowPower( bool status );

void SX1276SetAntSw( uint8_t opMode );
/*!
 * \brief Controls the antena switch if necessary.
 *
 * \remark see errata note
 *
 * \param [IN] opMode Current radio operating mode
 */
static LoRaBoardCallback_t BoardCallbacks = { SX1276SetXO,
                                              SX1276GetWakeTime,
                                              SX1276IoIrqInit,
                                              SX1276SetRfTxPower,
                                              SX1276SetAntSwLowPower,
                                              SX1276SetAntSw};

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1276IoInit,
    SX1276IoDeInit,
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby,
    SX1276SetRx,
    SX1276StartCad,
    SX1276SetTxContinuousWave,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength,
    SX1276SetPublicNetwork,
    SX1276GetWakeupTime
};

uint32_t SX1276GetWakeTime( void )
{
  return  BOARD_WAKEUP_TIME;
}

void SX1276SetXO( uint8_t state )
{

  if (state == SET )
  {
    TCXO_ON(); 
    
    DelayMs( BOARD_WAKEUP_TIME ); //start up time of TCXO
  }
  else
  {
    TCXO_OFF(); 
  }
}
void SX1276IoInit( void )
{
  GPIO_InitTypeDef initStruct={0};
  
  SX1276BoardInit( &BoardCallbacks );
  
  initStruct.Mode =GPIO_MODE_IT_RISING;
  initStruct.Pull = GPIO_PULLDOWN;
  initStruct.Speed = GPIO_SPEED_HIGH;

  HW_GPIO_Init( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct );
#ifdef RADIO_DIO_4
  HW_GPIO_Init( RADIO_DIO_4_PORT, RADIO_DIO_4_PIN, &initStruct );
#endif
#ifdef RADIO_DIO_5
  HW_GPIO_Init( RADIO_DIO_5_PORT, RADIO_DIO_5_PIN, &initStruct );
#endif
#if USE_TCXO_VDD
  initStruct.Mode =GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL;  
  HW_GPIO_Init( RADIO_TCXO_VCC_PORT, RADIO_TCXO_VCC_PIN, &initStruct );
#endif //USE_TCXO_VDD
}

void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{
  HW_GPIO_SetIrq( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, IRQ_HIGH_PRIORITY, irqHandlers[0] );
  HW_GPIO_SetIrq( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, IRQ_HIGH_PRIORITY, irqHandlers[1] );
  HW_GPIO_SetIrq( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, IRQ_HIGH_PRIORITY, irqHandlers[2] );
  HW_GPIO_SetIrq( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, IRQ_HIGH_PRIORITY, irqHandlers[3] );
}

void SX1276IoDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode = GPIO_MODE_IT_RISING ; //GPIO_MODE_ANALOG;
  initStruct.Pull = GPIO_PULLDOWN;
  
  HW_GPIO_Init( RADIO_DIO_0_PORT, RADIO_DIO_0_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_1_PORT, RADIO_DIO_1_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_2_PORT, RADIO_DIO_2_PIN, &initStruct );
  HW_GPIO_Init( RADIO_DIO_3_PORT, RADIO_DIO_3_PIN, &initStruct );
  
#ifdef RADIO_DIO_4
  HW_GPIO_Init( RADIO_DIO_4_PORT, RADIO_DIO_4_PIN, &initStruct );
#endif
#ifdef RADIO_DIO_5
  HW_GPIO_Init( RADIO_DIO_5_PORT, RADIO_DIO_5_PIN, &initStruct );
#endif
}

void SX1276SetRfTxPower( int8_t power )
{
    uint8_t paConfig = 0;
    uint8_t paDac = 0;

    paConfig = SX1276Read( REG_PACONFIG );
    paDac = SX1276Read( REG_PADAC );

    paConfig = ( paConfig & RF_PACONFIG_PASELECT_MASK ) | SX1276GetPaSelect( power );

    if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
    {
        if( power > 17 )
        {
            paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_ON;
        }
        else
        {
            paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_OFF;
        }
        if( ( paDac & RF_PADAC_20DBM_ON ) == RF_PADAC_20DBM_ON )
        {
            if( power < 5 )
            {
                power = 5;
            }
            if( power > 20 )
            {
                power = 20;
            }
            paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
        }
        else
        {
            if( power < 2 )
            {
                power = 2;
            }
            if( power > 17 )
            {
                power = 17;
            }
            paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
        }
    }
    else
    {
        if( power > 0 )
        {
            if( power > 15 )
            {
                power = 15;
            }
            paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( 7 << 4 ) | ( power );
        }
        else
        {
            if( power < -4 )
            {
                power = -4;
            }
            paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( 0 << 4 ) | ( power + 4 );
        }
    }
    SX1276Write( REG_PACONFIG, paConfig );
    SX1276Write( REG_PADAC, paDac );
}

uint8_t SX1276GetPaSelect( int8_t power )
{
    if (power >14)
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

void SX1276SetAntSwLowPower( bool status )
{
    if( status == false )
    {
      SX1276AntSwInit( );
    }
    else 
    {
      SX1276AntSwDeInit( );
    }
}

static void SX1276AntSwInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode =GPIO_MODE_OUTPUT_PP;
  initStruct.Pull = GPIO_NOPULL; 
  initStruct.Speed = GPIO_SPEED_HIGH;
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, 0);
}

static void SX1276AntSwDeInit( void )
{
  GPIO_InitTypeDef initStruct={0};

  initStruct.Mode = GPIO_MODE_ANALOG ;
  
  initStruct.Pull = GPIO_NOPULL;
  initStruct.Speed = GPIO_SPEED_HIGH;

  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, 0);
  
  HW_GPIO_Init( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, &initStruct  ); 
  HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, 0);
}

void SX1276SetAntSw( uint8_t opMode )
{
 uint8_t paConfig =  SX1276Read( REG_PACONFIG );
    switch( opMode )
    {
    case RFLR_OPMODE_TRANSMITTER:
      if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
      {
        HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_BOOST, RADIO_ANT_SWITCH_PIN_TX_BOOST, 1 );
      }
      else
      {
        HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_TX_RFO, RADIO_ANT_SWITCH_PIN_TX_RFO, 1 );
      }
      SX1276.RxTx = 1;
        break;
    case RFLR_OPMODE_RECEIVER:
    case RFLR_OPMODE_RECEIVER_SINGLE:
    case RFLR_OPMODE_CAD:
    default:
     SX1276.RxTx = 0;
     HW_GPIO_Write( RADIO_ANT_SWITCH_PORT_RX, RADIO_ANT_SWITCH_PIN_RX, 1 );
     break;
    }
}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
