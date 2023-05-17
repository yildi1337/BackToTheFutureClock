/**
 ******************************************************************************
 * @file    stm32f10x_irq.h
 * @author  Phillip Durdaut
 * @date    2014-04-10
 * @brief   Interrupt Service Routines.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void DebugMon_Handler(void);
extern void EXTI0_IRQHandler(void);

#endif /* __STM32F10x_IT_H */
