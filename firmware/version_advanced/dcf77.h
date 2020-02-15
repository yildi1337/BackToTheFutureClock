/**
 ******************************************************************************
 * @file    dcf77.h
 * @author  Phillip Durdaut
 * @date    2014-04-12
 * @brief   DCF77 driver.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DCF77_H
#define __DCF77_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
#define DCF77_UPDATE_EVERY_X_SECONDS        (60*60*24)

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  DCF77_UPDATE_STATUS_UPDATE_NOT_NECESSARY = 0,
  DCF77_UPDATE_STATUS_UPDATE_NECESSARY,
  DCF77_UPDATE_STATUS_UPDATE_IN_PROGRESS
} DCF77_UPDATE_STATUS_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void DCF77_Init(void);
extern void DCF77_Start(void);
extern void DCF77_Stop(void);
extern void DCF77_ISR(void);
extern DCF77_UPDATE_STATUS_t DCF77_GetUpdateStatus(void);

#endif /* __DCF77_H */
