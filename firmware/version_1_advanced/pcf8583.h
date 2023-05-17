/**
 ******************************************************************************
 * @file    pcf8583.h
 * @author  Phillip Durdaut
 * @date    2014-04-11
 * @brief   PCF8583 driver.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PCF8583_H
#define __PCF8583_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
#define PCF8583_I2CBUS                 I2C2
#define PCF8583_I2C_ADDRESS            0xA0

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void PCF8583_Init(void);
extern void PCF8583_InterruptEnable(void);
extern void PCF8583_InterruptDisable(void);
extern void PCF8583_ISR(void);
extern void PCF8583_GetDateTime(DATETIME_PCF8583_t * p_pDateTime);
extern void PCF8583_SetDateTime(DATETIME_PCF8583_t * p_pDateTime);
extern void PCF8583_GetLastDCF77UpdateDateTime(DATETIME_PCF8583_t * p_pDateTime);
extern void PCF8583_SetLastDCF77UpdateDateTime(DATETIME_PCF8583_t * p_pDateTime);
extern void PCF8583_GetAlarmDateTime(DATETIME_PCF8583_t * p_pDateTime);
extern void PCF8583_SetAlarmDateTime(DATETIME_PCF8583_t * p_pDateTime);

#endif /* __PCF8583_H */
