#ifndef NB2023S_H
#define NB2023S_H

#include <stm32f4xx.h>
#include <stm32f446xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_hal_spi.h>
#include <stm32f4xx_hal_gpio.h>
#include <NBDevicesLibrary.h>

/*******************************************************************************
 * Macro for setting SPI pins
 ******************************************************************************/
#define SPI_1
#define SPI_BUS                     SPI1
#define MOSI_PORT                   GPIOA
#define MOSI_PIN                    GPIO_PIN_7
#define MISO_PORT                   GPIOA
#define MISO_PIN                    GPIO_PIN_6
#define SCK_PORT                    GPIOA
#define SCK_PIN                     GPIO_PIN_5

// Macro for setting and clearing the module nRST pin all modules
#define MODULE_RESET_PORT           GPIOC
#define MODULE_RESET_PIN            GPIO_PIN_2

// Macro for setting and clearing the module AWAKE pin for 202x-S
#define MODULE_AWAKE_PORT           GPIOC
#define MODULE_AWAKE_PIN            GPIO_PIN_8

// Macro for setting and clearing the CS pin for x020-S modules
#define MODULE_CS_PORT              GPIOC
#define MODULE_CS_PIN               GPIO_PIN_13


/*******************************************************************************
 * Macro for setting BLUE BUTTON on STM32F429 Discovery
 ******************************************************************************/

// Macro for setting BLUE BUTTON
#define BLUE_BUTTON_PORT            GPIOA
#define BLUE_BUTTON_PIN             GPIO_PIN_0

extern I2C_HandleTypeDef hi2c3;
extern DMA_HandleTypeDef hdma_i2c3_rx;
extern DMA_HandleTypeDef hdma_i2c3_tx;

extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern TIM_HandleTypeDef htim4;



/*****************************************
 * Functions Prototypes
 *****************************************/
NBResult NB_API NBDeviceGetTimestamp(void * pContext, NBUInt32 * pui32Tick);
NBResult NB_API NBDeviceAwakeGetValueInternal(void * pContext, NBDevicePinValue * pValue);
NBResult NB_API NBDeviceResetSetValueInternal(void * pContext, NBDevicePinValue value);
NBResult NB_API NBDeviceDelayMicrosecondsInternal(void * pContext, NBUInt32 u32Value);
NBResult NB_API NBDeviceSendReceiveDataInternal(void * pContext, NBUInt8 * pTxBuffer, NBUInt16 txLength, NBUInt8 * pRxBuffer, NBUInt16 rxLength);
NBResult DeviceInit(HNBDevice * phDevice, NBDeviceScanFormatInfo * psScanFormatInfo);

void _Error_Handler(char * file, int line);

void NB_Config_HW ();
void NB_Delay_ms(uint32_t);
void NB_Delay_us(uint32_t);
void NB_Delay_s(uint8_t);
void NB_Module_nRST_High();
void NB_Module_nRST_Low();
void NB_Module_nRST(GPIO_PinState);

uint32_t NB_HAL_GetTick();
uint32_t NB_GetTick();

void SPI_Config();
void NB_Module_Status_Pin_Configure_Interrupt();
HAL_StatusTypeDef NB_SPI_ReadWriteBuffer (uint8_t* txBuff, uint8_t* rxBuff, uint16_t length);
void NB_Module_Wake();
GPIO_PinState NB_AWAKE_pin_isSET();

uint32_t DWT_Delay_Init();

__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}

#endif // !NB2023S_H
