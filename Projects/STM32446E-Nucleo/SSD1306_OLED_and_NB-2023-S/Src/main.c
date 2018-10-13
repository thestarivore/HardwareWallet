/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "stm32f4xx_hal.h"

#include "ssd1306_tests.h"
#include "ssd1306.h"

#include <NBDevicesLibrary.h>
#include <nb2023s.h>

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
TIM_HandleTypeDef htim4;
static int state = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
void        _Error_Handler(char * file, int line);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM4_Init(void);
static void MX_DMA_Init(void);

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
 * Misc functions
 ******************************************************************************/

static void ShowMessageOnDisplayA(const char * szMessage)
{
	char str1[12];
	char str2[12];
	int i=0;

	for(i=0; i<12; i++){
		str1[i]=' ';
		str2[i]=' ';
	}

	for(i=0; i<strlen(szMessage); i++){
		if(i<=12){
			str1[i]=szMessage[i];
		}
		else{
			str2[i]=szMessage[i];
		}
	}

    ssd1306_Fill(Black);
	ssd1306_SetCursor(2, 0);
	ssd1306_WriteString(str1, Font_11x18, White);
	ssd1306_SetCursor(2, 18);
	ssd1306_WriteString(str2, Font_11x18, White);
	ssd1306_UpdateScreen();
}

static void HandleError(NBResult iErrorCode)
{
    NBAChar szErrorMessage[256] = { 0 };
    NBUInt uiErrorMessageLength = sizeof(szErrorMessage) / sizeof(szErrorMessage[0]), uiActualErrorMessageLength;
    NBResult iResult = NBErrorsGetMessageA(iErrorCode, szErrorMessage, uiErrorMessageLength, &uiActualErrorMessageLength);
    if (NBFailed(iResult))
    {
        ShowMessageOnDisplayA("Failed to get error message");
    }
    else
    {
        ShowMessageOnDisplayA(szErrorMessage);
    }
    //SetLedRed();
    while(1);
}

/*******************************************************************************
 * Callback function while image capture
 ******************************************************************************/

static NBResult NB_API ScanPreviewCallback(const HNBDevice hDevice, NBDeviceScanStatus *peStatus, const NBDeviceScanFormatInfo *psScanFormatInfo, const NBByte *pBuffer, NBSizeType stBufferSize, const NBDeviceScanPreviewDetails *psDetails, void * pParam)
{
    if (psDetails->iFingerDetectValue > NB_DEVICE_DEFAULT_FINGER_DETECT_THRESHOLD)
    {
        /*SetMessageProperties();
        NB_LCD_Clear(LCD_COLOR_RED);
        SetLedBlue();
        NB_Delay_ms(70);
        SetLedOff();*/
    	ssd1306_Fill(White);
		ssd1306_UpdateScreen();
		NB_Delay_ms(70);
		ssd1306_Fill(Black);
		ssd1306_UpdateScreen();
		NB_Delay_ms(70);
		ssd1306_Fill(White);
		ssd1306_UpdateScreen();
		NB_Delay_ms(70);
		ssd1306_Fill(Black);
		ssd1306_UpdateScreen();
    }
    return NB_OK;
}


/*******************************************************************************
 * Init of HW STM32F429 Discovery board LCD
 ******************************************************************************/
static void HardwareLcdInit()
{
    ShowMessageOnDisplayA("System OK");
    HAL_Delay(1000);
}

/*******************************************************************************
 * Init of HW STM32F429 Discovery board
 ******************************************************************************/
static void HardwareInit()
{
    /* Initialize all configured peripherals */
    MX_DMA_Init();
    MX_TIM4_Init();
    MX_SPI1_Init();
    DWT_Delay_Init();

    /* Indication of end of HW setup */
    HardwareLcdInit();
}

void ssd1306_test_all() {
    ssd1306_TestAll();
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	HNBDevice hDevice = NULL;
	NBDeviceScanFormatInfo sScanFormatInfo;
	NBResult iResult;
	NBByte * pImage = NULL;
	NBSizeType stImageSize;
	NBInt iTimeout = NB_DEVICE_SCAN_TIMEOUT_INFINITE;
	NBDeviceScanStatus eStatus;
	NBBool bTerminate = NBFalse;


	/* STM32F4xx HAL library initialization:
	- Configure the Flash prefetch and Buffer caches
	- Systick timer is configured by default as source of time base, but user
	can eventually implement his proper time base source (a general purpose
	timer for example or other time source), keeping in mind that Time base
	duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	handled in milliseconds basis.
	- Low Level Initialization
	*/
	HAL_Init();

	/* Configure the system clock to 100 MHz */
	SystemClock_Config();

	/* Add your application code here
	 */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI2_Init();

	//SSD1306 Display Initialization
	ssd1306_Init();
	//ssd1306_test_all();

	//NB-2023-S Sensor Initialization
	HardwareInit();

	/* Initialize NBDevices library */
	iResult = NBDevicesInitializeA(NULL, NULL, NULL, 0);
	if (NBFailed(iResult))
	{
		goto finally;
	}
	bTerminate = NBTrue;

	/* NBDevice initialize */
	iResult = DeviceInit(&hDevice, &sScanFormatInfo);
	if (NBFailed(iResult))
	{
		goto finally;
	}

	/* Allocate image on heap (can be on stack) */
	stImageSize = sScanFormatInfo.uiWidth * sScanFormatInfo.uiHeight;
	pImage = (NBByte*)malloc(sizeof(NBByte) * stImageSize);
	if (!pImage)
	{
		iResult = NB_ERROR_OUT_OF_MEMORY;
		goto finally;
	}

	ShowMessageOnDisplayA("Put finger on");

	/* Scan Finger with finger detect option */
	iResult = NBDeviceScanEx(hDevice, sScanFormatInfo.eScanFormat, iTimeout, ScanPreviewCallback, NULL, pImage, stImageSize, 0, &eStatus);
	if (NBFailed(iResult))
	{
		goto finally;
	}

	//SetLedGreen();
	ShowMessageOnDisplayA("Image captured");


finally :
	// Cleaning up
	if (NBFailed(iResult)) HandleError(iResult);
	if (pImage) free(pImage);
	if (hDevice) NBDeviceDestroy(hDevice);
	if (bTerminate) NBDevicesTerminate();
	return iResult;

	/* Infinite loop */
	while (1)
	{
	 /* switch (state){
		  case 0:{
			  ssd1306_Fill(Black);
			  ssd1306_SetCursor(2, 0);
			  ssd1306_WriteString("Place your", Font_11x18, White);
			  ssd1306_SetCursor(2, 18);
			  ssd1306_WriteString("finger!", Font_11x18, White);
			  ssd1306_UpdateScreen();
			  state++;
			  break;
		  }
		  case 1:{
			  HAL_Delay(2000);
			  state++;
			  break;
		  }
		  case 2:{
			  ssd1306_Fill(Black);
			  ssd1306_SetCursor(2, 0);
			  ssd1306_WriteString("Well done!", Font_11x18, White);
			  ssd1306_UpdateScreen();
			  state++;
			  break;
		  }
		  case 3:{

			  break;
		  }
	  }*/
	}
	return 0;
}

/* SPI1 init function for NB-2023*/
static void MX_SPI1_Init()
{
    /* Enable SPI1 clock */
    __SPI1_CLK_ENABLE();

    /* Enable DMA2 clock */
    __DMA2_CLK_ENABLE();

    /* Enable GPIO clock */
    __GPIOA_CLK_ENABLE();

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    HAL_SPI_Init(&hspi1);

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_1LINE;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* Timer TIM4 init function */
static void MX_TIM4_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;

    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 840-1; // ABP1 Timer clock = 84MHz -> f(tick) = 10us
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 65535;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&htim4);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

static void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __DMA2_CLK_ENABLE();
    __DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA2   ------> USART2_TX
     PA3   ------> USART2_RX
     PB8   ------> I2C1_SCL
     PB9   ------> I2C1_SDA
*/
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA8 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configure GPIO pins for nb-2023-s: */

  // CS PIN 202x
  GPIO_InitStruct.Pin = MODULE_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(MODULE_CS_PORT, &GPIO_InitStruct);
  HAL_GPIO_WritePin(MODULE_CS_PORT, MODULE_CS_PIN, GPIO_PIN_SET);

  // RESET PIN 202x
  GPIO_InitStruct.Pin = MODULE_RESET_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(MODULE_RESET_PORT, &GPIO_InitStruct);

  // BLUE BUTTON
  GPIO_InitStruct.Pin = BLUE_BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BLUE_BUTTON_PORT, &GPIO_InitStruct);

  // AWAKE PIN 202x
  GPIO_InitStruct.Pin = MODULE_AWAKE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(MODULE_AWAKE_PORT, &GPIO_InitStruct);

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 200
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}
                                                        
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
