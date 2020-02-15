/**
 ******************************************************************************
 * @file    led.h
 * @author  Phillip Durdaut
 * @date    2014-07-14
 * @brief   Busy wait delay functions.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
#define DELAY_CYCLES_PER_MS     4750
#define DELAY_CYCLES_PER_US     5

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void DELAY_ms(uint32_t p_Delay_ms);
extern void DELAY_us(uint32_t p_Delay_us);

#endif /* __DELAY_H */
