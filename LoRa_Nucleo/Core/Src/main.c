/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief           : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define MSGQUEUE_OBJECTS 1                     // number of Message Queue Objects
#define FLAG_DATA 0x00000001U
#define FLAG_SLEEP 0x00000001U

// Define I2C address and register addresses
#define ADPD188BI         0x64
#define MODE              0x10
#define SAMPLE_CLK        0x4B

#define PROGRAM_MODE_MSB          0b00000000
#define PROGRAM_MODE_LSB          0b00000001
#define NORMAL_MODE_MSB           0b00000000
#define NORMAL_MODE_LSB           0b00000010
#define SAMPLE_CLK_ENABLED_MSB    0b01001100
#define SAMPLE_CLK_ENABLED_LSB    0b10010010

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi1;
I2C_HandleTypeDef hi2c2;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

/* Definitions for Flame */
osThreadId_t SendDataHandle;
const osThreadAttr_t SendData_attributes = {
  .name = "SendData",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

/* Definitions for Command */
osThreadId_t CommandHandle;
const osThreadAttr_t Command_attributes = {
  .name = "Command",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for DataCollection */
osThreadId_t DataCollectionHandle;
const osThreadAttr_t DataCollection_attributes = {
  .name = "DataCollection",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
int Rotations = 0; // cup rotation counter used in interrupt routine
int ContactBounceTime = 0; // Timer to avoid contact bounce in interrupt routine
int WindSpeed; // speed miles per hour
int counter=-1;

osEventFlagsId_t evt_data;
osEventFlagsId_t evt_sleep;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C2_Init(void);

void SendDataTask(void *argument);
void CommandTask(void *argument);
void DataCollectionTask(void *argument);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t SENSOR_DATA[4];
int STAND_BY = 0;

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	char msg7[50];


	  if (GPIO_Pin == GPIO_PIN_10) {
		  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
		  	  {
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		  		  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFE);
		  	  }


		  //sprintf(msg7, "ROTATION\r\n");
		  //HAL_UART_Transmit(&huart2, (uint8_t*)msg7, strlen(msg7), HAL_MAX_DELAY);

		  if (HAL_GetTick() - ContactBounceTime > 15 ) { // debounce the switch contact.

		  Rotations++;

		 // sprintf(msg7, "rotations: %d\r\n", Rotations ); // mph * 100
		 // HAL_UART_Transmit(&huart2, (uint8_t*)msg7, strlen(msg7), HAL_MAX_DELAY);
		  ContactBounceTime = HAL_GetTick();
		  }
	  }
	  if (GPIO_Pin == GPIO_PIN_9) {

		  __SEV();
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		  //sprintf(msg7, "PA9\r\n");
		  //HAL_UART_Transmit(&huart2, (uint8_t*)msg7, strlen(msg7), HAL_MAX_DELAY);
		  SystemClock_Config();

		  HAL_ResumeTick();


	  }




}

uint8_t i2c_readRegisterByte(uint8_t deviceAddress, uint8_t registerAddress) {
    uint8_t data;
    HAL_I2C_Mem_Read(&hi2c2, deviceAddress << 1, registerAddress, 1, &data, 1, HAL_MAX_DELAY);
    return data;
}

uint8_t i2c_writeRegisterByte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t MSB_RegisterByte, uint8_t LSB_RegisterByte) {
    uint8_t data[2] = {MSB_RegisterByte, LSB_RegisterByte};
    return HAL_I2C_Mem_Write(&hi2c2, deviceAddress << 1, registerAddress, 1, data, 2, HAL_MAX_DELAY);
}


void ADPD188BI_StartSampling(void) {
    // Set CLK32K_EN bit to 1 and force the device into program mode
    i2c_writeRegisterByte(ADPD188BI, SAMPLE_CLK, SAMPLE_CLK_ENABLED_MSB, SAMPLE_CLK_ENABLED_LSB);
    i2c_writeRegisterByte(ADPD188BI, MODE, PROGRAM_MODE_MSB, PROGRAM_MODE_LSB);

    // Recommended settings ADPD188BI (Table 26 from Data Sheet)
    i2c_writeRegisterByte(ADPD188BI, 0x11, 0x30, 0x68);
    i2c_writeRegisterByte(ADPD188BI, 0x12, 0x02, 0x00);
    i2c_writeRegisterByte(ADPD188BI, 0x14, 0x01, 0x1D);
    i2c_writeRegisterByte(ADPD188BI, 0x15, 0x00, 0x00);
    i2c_writeRegisterByte(ADPD188BI, 0x17, 0x00, 0x09);
    i2c_writeRegisterByte(ADPD188BI, 0x18, 0x00, 0x00);
    i2c_writeRegisterByte(ADPD188BI, 0x19, 0x3F, 0xFF);
    i2c_writeRegisterByte(ADPD188BI, 0x1A, 0x3F, 0xFF);
    i2c_writeRegisterByte(ADPD188BI, 0x1B, 0x3F, 0xFF);
    i2c_writeRegisterByte(ADPD188BI, 0x1D, 0x00, 0x09);
    i2c_writeRegisterByte(ADPD188BI, 0x1E, 0x00, 0x00);
    i2c_writeRegisterByte(ADPD188BI, 0x1F, 0x3F, 0xFF);
    i2c_writeRegisterByte(ADPD188BI, 0x20, 0x3F, 0xFF);
    i2c_writeRegisterByte(ADPD188BI, 0x21, 0x3F, 0xFF);
    i2c_writeRegisterByte(ADPD188BI, 0x22, 0x35, 0x39);
    i2c_writeRegisterByte(ADPD188BI, 0x23, 0x35, 0x36);
    i2c_writeRegisterByte(ADPD188BI, 0x24, 0x15, 0x30);
    i2c_writeRegisterByte(ADPD188BI, 0x25, 0x63, 0x0C);
    i2c_writeRegisterByte(ADPD188BI, 0x30, 0x03, 0x20);
    i2c_writeRegisterByte(ADPD188BI, 0x31, 0x04, 0x0E);
    i2c_writeRegisterByte(ADPD188BI, 0x35, 0x03, 0x20);
    i2c_writeRegisterByte(ADPD188BI, 0x36, 0x04, 0x0E);
    i2c_writeRegisterByte(ADPD188BI, 0x39, 0x22, 0xF0);
    i2c_writeRegisterByte(ADPD188BI, 0x3B, 0x22, 0xF0);
    i2c_writeRegisterByte(ADPD188BI, 0x3C, 0x31, 0xC6);
    i2c_writeRegisterByte(ADPD188BI, 0x42, 0x1C, 0x34);
    i2c_writeRegisterByte(ADPD188BI, 0x43, 0xAD, 0xA5);
    i2c_writeRegisterByte(ADPD188BI, 0x44, 0x1C, 0x34);
    i2c_writeRegisterByte(ADPD188BI, 0x45, 0xAD, 0xA5);
    i2c_writeRegisterByte(ADPD188BI, 0x58, 0x05, 0x44);
    i2c_writeRegisterByte(ADPD188BI, 0x11, 0x30, 0xA9);

    // Start normal Sampling operation
    i2c_writeRegisterByte(ADPD188BI, MODE, NORMAL_MODE_MSB, NORMAL_MODE_LSB);
}



// Function for SPI Communication to send data to IoT Tracker
int iot_tracker_communication(uint8_t* TX_Data, uint8_t*  RX_Data ) {
		//char spimsg[50];
		//char msg[50];
		int status;

		//Begin
		//sprintf(spimsg,"----- Communication begin -----\r\n");
		//HAL_UART_Transmit(&huart2, (uint8_t*) spimsg, strlen(spimsg), HAL_MAX_DELAY);

		//SPI Communication

		status = HAL_SPI_TransmitReceive( &hspi1, (uint8_t *) TX_Data,(uint8_t *) RX_Data,1, 0xFFFFFFFFU);

		uint8_t i = *RX_Data;
		//NEED TO MAKE NORMILIZATION FOR THE SENSOR DATA
		if (i == 0 || i == 3){
			*TX_Data = SENSOR_DATA[i];
			//sprintf(msg, "I got the command to send the %d st and i will send the value: %d \r\n", i, *TX_Data);
			//HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);
		}
		else if (i>0 || i < 3){
			*TX_Data = SENSOR_DATA[i]/16;
			//sprintf(msg, "I got the command to send the %d st and i will send the value: %d \r\n", i, *TX_Data);
			//HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);
		}


		return 0;
	}
/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();
  MX_I2C2_Init();

  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  evt_data = osEventFlagsNew(NULL);
  evt_sleep = osEventFlagsNew(NULL);
  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of SendData */
  SendDataHandle = osThreadNew(SendDataTask, NULL, &SendData_attributes);

  /* creation of DataCollection */
  DataCollectionHandle = osThreadNew(DataCollectionTask, NULL, &DataCollection_attributes);

  /* creation of Command */
  CommandHandle = osThreadNew(CommandTask, NULL, &Command_attributes);



  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB a /* while (1)nd APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  *
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 7999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  /*GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
*/
  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}



/* USER CODE END 4 */

/* USER CODE BEGIN Header_CommandTask */
/**
  * @brief  Function implementing the Command thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_CommandTask */
void CommandTask(void *argument)
{

	char msg[50];
	uint32_t flags;

		  /* Infinite loop */
		  for(;;)
		  {

			  __WFE();
			  sprintf(msg, "SLEEP MODE TASK!!! \r\n");
			  HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);

			  flags = osEventFlagsWait(evt_sleep, FLAG_SLEEP, osFlagsWaitAny, osWaitForever);

			  sprintf(msg, "SLEEP MODE!!! \r\n");
			  HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);
			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);





			  HAL_SuspendTick();
			  HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE);
			  //MX_SPI1_Init();
			  sprintf(msg, "exit from stop mode!!! \r\n");
			  HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);

			 // STAND_BY = 0;
			  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			 //vTaskDelay(1000);
		  }

}

/* USER CODE BEGIN Header_SendDataTask */
/**
  * @brief  Function implementing the Flame thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_SendDataTask */
void SendDataTask(void *argument)
{

	uint32_t flags;
	char msg[50];
	/* Infinite loop */
	for(;;)
	{


		flags = osEventFlagsWait(evt_data, FLAG_DATA, osFlagsWaitAny, osWaitForever);
		sprintf(msg, "SEND DATA TASK \r\n");
		HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);


		uint8_t TX_Data;
		uint8_t RX_Data;

		TX_Data = SENSOR_DATA[0];

		while(counter !=4){
			iot_tracker_communication((uint8_t*) &TX_Data,(uint8_t*)  &RX_Data );
			counter = RX_Data;
		}




		//if(counter == 3){
			counter = -1;
			osEventFlagsSet(evt_sleep, FLAG_SLEEP);
		//}



	}


}

/* USER CODE BEGIN Header_DataCollectionTask */
/**
* @brief Function implementing the WindSpeed thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DataCollectionTask */
void DataCollectionTask(void *argument)
{
	 /* USER CODE BEGIN 5 */
	uint8_t wind_speed_val;
	uint16_t wind_direction_val;
	uint16_t flame_val;
	uint16_t smoke_detection_data;

	char msg[50];

	/* Infinite loop */
	  for(;;)
	  {
		   sprintf(msg, "DATA COLLECTION TASK START!!! \r\n");
		   HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);


		   /* READ FLAME SENSOR! */

		    ADC_ChannelConfTypeDef sConfig = {0};
		 	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. */
		    	sConfig.Channel = ADC_CHANNEL_1;
		    	sConfig.Rank = 1;
		  	  	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
		  	  	  {
		  	  	    Error_Handler();
		  	  	 }
			// Get ADC value
		      HAL_ADC_Start(&hadc);
		      HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
		      flame_val = HAL_ADC_GetValue(&hadc);
		      HAL_ADC_Stop(&hadc);

				sConfig.Rank = ADC_RANK_NONE;
				if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
				  {
					Error_Handler();
				 }
			//Pretend we have to do something else for a while
			   ADC_ChannelConfTypeDef sConfigd = {0};
			   sConfigd.Channel = ADC_CHANNEL_0;
			   sConfigd.Rank = 1;
			   if (HAL_ADC_ConfigChannel(&hadc, &sConfigd) != HAL_OK)
			   	   {
		 	   		Error_Handler();
			   	   }
		   /* END FLAME SENSOR! */

		   /* READ WIND DIRECTION! */
			// Get ADC value
		 	   	HAL_ADC_Start(&hadc);
		 	   	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
		 	   	wind_direction_val = HAL_ADC_GetValue(&hadc);
		 	   	HAL_ADC_Stop(&hadc);

		 	   	sConfigd.Rank = ADC_RANK_NONE;
		 	    		if (HAL_ADC_ConfigChannel(&hadc, &sConfigd) != HAL_OK)
		 	   		  {
		 	   			Error_Handler();
		 	   		  }
		  /* END WIND DIRECTION! */

		  /* READ WIND SPEED */
		 	  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
			  __enable_irq();
			 // vTaskDelay(3000);
			  Rotations = 0; // current fix (bcz exti callback is called before infinitely many time)
			  TickType_t currentTick = xTaskGetTickCount();
			  while(xTaskGetTickCount() - currentTick < pdMS_TO_TICKS(3000));
			 __disable_irq();

			 // convert to mp/h using the formula V=P(2.25/T)
		     // V = P(2.25/3) = P * 0.75


			 wind_speed_val = Rotations ; //* 75  ;

			 /*For debug print as above*/
			 // sprintf(msg, "Wind Speed: %d\r\n", wind_speed_val ); // mph * 100
			 // HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

			  Rotations = 0;
			  ContactBounceTime = 0;
		     /* END WIND SPEED */

			 //Set sensor values to SENSOR_DATA array
			 SENSOR_DATA[0] = wind_speed_val;
			 SENSOR_DATA[1] = wind_direction_val;
			 SENSOR_DATA[2] = flame_val;



			 ADPD188BI_StartSampling(); // Start sampling operation

			 // Add your main loop logic here
			 smoke_detection_data = i2c_readRegisterByte(ADPD188BI, 0x64) << 8;
			 smoke_detection_data |= i2c_readRegisterByte(ADPD188BI, 0x65);

			 // Process the data as needed
			 //char buffer[20]; // A buffer to hold the string representation of the data
			 //sprintf(buffer, "Smoke data: %02d\r\n", smoke_detection_data);
			 //HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), HAL_MAX_DELAY);

			 i2c_writeRegisterByte(ADPD188BI, MODE, 0x00, 0x00); //sleep mode

			 SENSOR_DATA[3] = smoke_detection_data/125;

			 sprintf(msg, "DATA COLLECTION TASK END!2!! %d %d %d %d \r\n", wind_speed_val, wind_direction_val/16, flame_val/16, smoke_detection_data/125 );
			 HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);

			 osEventFlagsSet(evt_data, FLAG_DATA);

			 vTaskDelay(5000);

	  }
	  /* USER CODE END 5 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  char msg[50];

  while (1)
  {
	  sprintf(msg, "ERROR HANDLER\r\n");
	  HAL_UART_Transmit(&huart2, (uint16_t*)msg, strlen(msg), HAL_MAX_DELAY);

  }
  /* USER CODE END Error_Handler_Debug */
}


static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00707CBB;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
