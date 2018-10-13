#include "nb2023s.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <stm32f446xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_spi.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_tim.h>
#include <NBDevicesLibrary.h>


/*******************************************************************************
 * HW API
 ******************************************************************************/

NBResult NB_API NBDeviceGetTimestamp(void * pContext, NBUInt32 * pui32Tick)
{
    if (!pui32Tick) return NB_ERROR_ARGUMENT_NULL;
    *pui32Tick = HAL_GetTick();
    return NB_OK;
}

NBResult NB_API NBDeviceAwakeGetValueInternal(void * pContext, NBDevicePinValue * pValue)
{
    if (!pValue) return NB_ERROR_ARGUMENT_NULL;
    *pValue = (HAL_GPIO_ReadPin(MODULE_AWAKE_PORT, MODULE_AWAKE_PIN) == GPIO_PIN_SET) ? NBDevicePinValueHigh : NBDevicePinValueLow;
    return NB_OK;
}

NBResult NB_API NBDeviceResetSetValueInternal(void * pContext, NBDevicePinValue value)
{
    if (value == NBDevicePinValueUnknown) return NB_ERROR_ARGUMENT;
    // Bring the sensor out of reset / Put the sensor into reset
    NB_Module_nRST(value == NBDevicePinValueHigh ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return NB_OK;
}

NBResult NB_API NBDeviceDelayMicrosecondsInternal(void * pContext, NBUInt32 u32Value)
{
    NB_Delay_us((NBUInt32) u32Value);
    return NB_OK;
}

NBResult NB_API NBDeviceSendReceiveDataInternal(void * pContext, NBUInt8 * pTxBuffer, NBUInt16 txLength, NBUInt8 * pRxBuffer, NBUInt16 rxLength)
{
    HAL_StatusTypeDef eResult;
    if (!pTxBuffer) return NB_ERROR_ARGUMENT_NULL;
    if (!pRxBuffer) return NB_ERROR_ARGUMENT_NULL;
    eResult = NB_SPI_ReadWriteBuffer ((NBUInt8 *) pTxBuffer, (NBUInt8 *) pRxBuffer, (NBUInt16) rxLength);
    if (eResult != HAL_OK) return NB_ERROR_FAILED;
    return NB_OK;
}

/*******************************************************************************
 * Init of NEXT Device
 ******************************************************************************/
NBResult DeviceInit(HNBDevice * phDevice, NBDeviceScanFormatInfo * psScanFormatInfo)
{
    NBResult iResult;
    NBVersion sFwVersion;
    NBAChar szSerialNumber[25];
    NBUInt uiSerialLength = sizeof(szSerialNumber) / sizeof(szSerialNumber[0]), uiActualSerialLength;
    NBDeviceScanFormat areScanFormats[2];
    NBUInt uiScanFormatsCount = sizeof(areScanFormats) / sizeof(areScanFormats[0]), uiActualScanFormats;
    HNBDevice hDevice = NULL;

    // DeviceIO implementation
    NBDeviceIO sDeviceIO = { 0 };
    sDeviceIO.pAwakeGetValue = NBDeviceAwakeGetValueInternal;
    sDeviceIO.pResetSetValue = NBDeviceResetSetValueInternal;
    sDeviceIO.pDelayMicroseconds = NBDeviceDelayMicrosecondsInternal;
    sDeviceIO.pSendReceiveData = NBDeviceSendReceiveDataInternal;
    sDeviceIO.pGetTimestamp = NBDeviceGetTimestamp;

    // Initialize of SPI Device
    iResult = NBDeviceConnectToSpiRaw(&sDeviceIO, 0, &hDevice);
    if (NBFailed(iResult))
    {
        goto finally;
    }

    // Get device firmware version
    iResult = NBDeviceGetFirmwareVersion(hDevice, &sFwVersion);
    if (NBFailed(iResult))
    {
        goto finally;
    }

    // Get device serial number
    iResult = NBDeviceGetSerialNumberA(hDevice, szSerialNumber, uiSerialLength, &uiActualSerialLength);
    if (NBFailed(iResult))
    {
        goto finally;
    }

    // Get device scan formats
    iResult = NBDeviceGetSupportedScanFormats(hDevice, areScanFormats, uiScanFormatsCount, &uiActualScanFormats);
    if (NBFailed(iResult))
    {
        goto finally;
    }

    iResult = NBDeviceGetScanFormatInfo(hDevice, areScanFormats[0], psScanFormatInfo);
    *phDevice = hDevice; hDevice = NULL;
finally:
    if (hDevice) NBDeviceDestroy(hDevice);
    return iResult;
}



/*******************************************************************************
 * Function for setting of HW interfaces
 ******************************************************************************/

HAL_StatusTypeDef NB_SPI_ReadWriteBuffer (uint8_t* txBuff, uint8_t* rxBuff, uint16_t length)
{
    uint32_t timeout = 50;
    HAL_StatusTypeDef retval = 0;

    HAL_GPIO_WritePin(MODULE_CS_PORT, MODULE_CS_PIN, GPIO_PIN_RESET);

    retval = HAL_SPI_TransmitReceive(&hspi1, txBuff, rxBuff, length, timeout);
    HAL_GPIO_WritePin(MODULE_CS_PORT, MODULE_CS_PIN, GPIO_PIN_SET);

    return retval;
}

void NB_Delay_s(uint8_t s){

    HAL_Delay(s * 1000);
}

void NB_Delay_ms(uint32_t ms){

    if (__get_IPSR() == 0)
    {
        /* Called from thread mode */
        uint32_t tickstart = HAL_GetTick();

        /* Count interrupts */
        while ((HAL_GetTick() - tickstart) < ms)
        {
        }
    }
    else
    {
        /* Called from interrupt mode */
        while (ms)
        {
            /* Check if timer reached zero after we last checked COUNTFLAG bit */
            if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
            {
                ms--;
            }
        }
    }
}

void NB_Delay_us(uint32_t us)
{
    DWT_Delay_us(us);
}

uint32_t DWT_Delay_Init(void)
{
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

    /* Reset the clock cycle counter value */
    DWT->CYCCNT = 0;

    /* 3 NO OPERATION instructions */
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");

    /* Check if clock cycle counter has started */
    if(DWT->CYCCNT)
    {
        return 0; /*clock cycle counter started*/
    }
    else
    {
        return 1; /*clock cycle counter not started*/
    }
}

void NB_Module_nRST(GPIO_PinState value)
{
    HAL_GPIO_WritePin(MODULE_RESET_PORT, MODULE_RESET_PIN, value);
}

void NB_Module_nRST_High(void){

    HAL_GPIO_WritePin(MODULE_RESET_PORT, MODULE_RESET_PIN, GPIO_PIN_SET);
}

void NB_Module_nRST_Low(void)
{
    HAL_GPIO_WritePin(MODULE_RESET_PORT, MODULE_RESET_PIN, GPIO_PIN_RESET);
}

GPIO_PinState NB_AWAKE_pin_isSET()
{
    return HAL_GPIO_ReadPin(MODULE_AWAKE_PORT, MODULE_AWAKE_PIN);
}

uint32_t NB_HAL_GetTick(){
    return HAL_GetTick();
}

uint32_t NB_GetTick(){
    return NB_HAL_GetTick();
}



