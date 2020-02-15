/**
 ******************************************************************************
 * @file    tim2.h
 * @author  Phillip Durdaut
 * @date    2014-04-15
 * @brief   TIM2 driver (used for DCF-77).
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM2_H
#define __TIM2_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void TIM2_Init(void);
extern void TIM2_Start(void);
extern void TIM2_ClearAndStart(void);
extern void TIM2_Stop(void);
extern void TIM2_StopAndClear(void);
extern uint16_t TIM2_GetCounterValue(void);
extern void TIM2_ISR(void);

#endif /* __TIM2_H */
