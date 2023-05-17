/**
 ******************************************************************************
 * @file    led.h
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   LED control.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LED_SECONDS = 0,
  LED_AMPM_RED,
  LED_AMPM_GREEN,
  LED_AMPM_YELLOW
} LED_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void LED_Init(void);
extern void LED_On(LED_t p_LED);
extern void LED_Off(LED_t p_LED);
extern void LED_Toggle(LED_t p_LED);

#endif /* __LED_H */
