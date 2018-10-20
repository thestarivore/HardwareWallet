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

/* Includes ------------------------------------------------------------------*/
#include "eeprom_manager.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//Data Lenghts: number of uint16_t variables to use
//The SUM of their lenghts must be exactly NB_OF_VAR(in eeprom.h) 
#define FINGERPRINT_EE_LEGNHT           FINGERPRINT_PIECES


//Data Indexes: index of the first uint16_t variable to use
#define FINGERPRINT_EE_INDEX            0


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
uint16_t VirtAddVarTab[NB_OF_VAR];
uint16_t VarDataTab[NB_OF_VAR];

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/**
  * @brief  EEPROM Manager Initializzation
  * @param fp: FlashData struct pointer, containing all the  variable stored in falsh
  * @retval error: FError variable indicating errors
  */
FError flashInit(FlashParams* fp)
{ 
    FError error = FLASH_ALL_OK;
    if (!fp)
    {
        return FLASH_INVALID_POINTER;
    }
    else
    {
        //Set array defaults
        for(int i=0; i < NB_OF_VAR; i++){
            //Set incremental addresses
            VirtAddVarTab[i] = i+1;        
            
            //Set Default to 0
            VarDataTab[i] = 0;
        }  
        
        //FlashData fingerprint data init
        for(int i=0; i < FINGERPRINT_EE_LEGNHT; i++){
            fp->array[i] = 0;
        }  
        
        //Read all data from flash
        flashReadEverything(fp);
    }
    return error;
}

/**
  * @brief  EEPROM Manager: Read Wifi Authentication data
  * @param fp: FlashData struct pointer, containing all the  variable stored in falsh
  * @retval error: FError variable indicating errors
  */
FError flashReadSomeVAR(FlashParams* fp)
{
    FError error = FLASH_ALL_OK;
    if (!fp)
    {
        return FLASH_INVALID_POINTER;
    }
    else
    {
        uint16_t tmp = 0, i = 0;
      
        //array read
        for(i=FINGERPRINT_EE_INDEX; i < FINGERPRINT_EE_INDEX + FINGERPRINT_EE_LEGNHT; i++){
            EE_ReadVariable(VirtAddVarTab[i], &tmp);
            fp->array[i] = tmp;
            
            //Stop at end of string
            //if(tmp == '\0')
            //    break;
        }
    }
    return error;
}

/**
  * @brief  EEPROM Manager: Write Wifi Authentication data
  * @param fp: FlashData struct pointer, containing all the  variable stored in falsh
  * @param wcp: WifiConnectionPair struct pointer, containing the SSID and PSK pair for wifi settings
  * @retval error: FError variable indicating errors
  */
FError flashWriteSomeVAR(FlashParams* fp, uint16_t array[], uint16_t arraySize)
{
    FError error = FLASH_ALL_OK;
    if (!fp || !array)
    {
        return FLASH_INVALID_POINTER;
    }
    else
    {
        int i = 0;
        
        //array write
        for(i=FINGERPRINT_EE_INDEX; i < FINGERPRINT_EE_INDEX + arraySize; i++){
            EE_WriteVariable(VirtAddVarTab[i], (uint16_t)array[i]);
        }
    }
    return error;
}


/**
  * @brief  EEPROM Manager: Read All data in Flash
  * @param fp: FlashData struct pointer, containing all the  variable stored in falsh
  * @retval error: FError variable indicating errors
  */
FError flashReadEverything(FlashParams* fp)
{
    FError error = FLASH_ALL_OK;
    if (!fp)
    {
        return FLASH_INVALID_POINTER;
    }
    else
    {
        flashReadSomeVAR(fp);
    }
    return error;
}

/*************************** End of file ****************************/