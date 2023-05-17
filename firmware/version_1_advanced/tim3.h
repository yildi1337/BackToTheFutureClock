/**
 ******************************************************************************
 * @file    tim3.h
 * @author  Phillip Durdaut
 * @date    2014-04-15
 * @brief   TIM3 driver (used for IR).
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM3_H
#define __TIM3_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void TIM3_Init(void);
extern void TIM3_Start(void);
extern void TIM3_ClearAndStart(void);
extern void TIM3_Stop(void);
extern void TIM3_StopAndClear(void);
extern uint16_t TIM3_GetCounterValue(void);
extern void TIM3_ISR(void);

#endif /* __TIM3_H */
