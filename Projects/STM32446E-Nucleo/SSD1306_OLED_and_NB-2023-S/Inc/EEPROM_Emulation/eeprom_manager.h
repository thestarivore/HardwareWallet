/**
******************************************************************************
* @file    eeprom_manager.c
* @author  Eduard I. Chirica
* @version V1.0
* @date    19-October-2018
* @brief   EEPROM Emulation Manager Library
******************************************************************************
* @attention
*
******************************************************************************
*/

#ifndef  __EEPROM_MANAGER_H
#define  __EEPROM_MANAGER_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "eeprom.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define FINGERPRINT_PIECES      200


/*********************************************************************
*
*       Public typedefs
*
**********************************************************************
*/
/**
 * FlashParams: Flash Parameters manager struct
 */
typedef struct{
    uint16_t array[FINGERPRINT_PIECES];
}FlashParams;

/**
 * FError: Flash Errors enumeration values
 */
typedef enum{
    FLASH_ALL_OK,
    FLASH_GENERIC_ERROR,
    FLASH_INVALID_POINTER,
}FError;

/*********************************************************************
*
*       Public Function Prototype
*
**********************************************************************
*/
FError flashInit(FlashParams* fp);
FError flashReadSomeVAR(FlashParams* fp);
FError flashWriteSomeVAR(FlashParams* fp, uint16_t array[], uint16_t arraySize);
FError flashReadEverything(FlashParams* fp);


#endif   /* ifdef __EEPROM_MANAGER_H */
/*************************** End of file ****************************/