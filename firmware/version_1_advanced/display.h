/**
 ******************************************************************************
 * @file    display.h
 * @author  Phillip Durdaut
 * @date    2014-04-11
 * @brief   Display controlling.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DISPLAY_H
#define __DISPLAY_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  DISPLAY_LINE_DESTINATION = 0,
  DISPLAY_LINE_PRESENT,
  DISPLAY_LINE_CNT
} DISPLAY_LINE_t;

typedef enum
{
  DISPLAY_ELEMENT_MONTH = 0,
  DISPLAY_ELEMENT_DAY,
  DISPLAY_ELEMENT_YEAR,
  DISPLAY_ELEMENT_HOUR,
  DISPLAY_ELEMENT_MINUTE,
  DISPLAY_ELEMENT_CNT
} DISPLAY_ELEMENT_t;

typedef enum
{
  DISPLAY_BLINKSTATE_DISABLED = 0,
  DISPLAY_BLINKSTATE_ENABLED
} DISPLAY_BLINKSTATE_t;

typedef enum
{
  DISPLAY_BLINKFREQ_SLOW = 0,
  DISPLAY_BLINKFREQ_FAST
} DISPLAY_BLINKFREQ_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void DISPLAY_Init(void);
extern void DISPLAY_On(void);
extern void DISPLAY_Off(void);
extern void DISPLAY_SetDateTime(DISPLAY_LINE_t p_DisplayLine, DATETIME_DISPLAY_t * p_DateTime);
extern void DISPLAY_EnableBlinking(DISPLAY_LINE_t p_DisplayLine, DISPLAY_ELEMENT_t p_DisplayElement, DISPLAY_BLINKFREQ_t p_BlinkFreq);
extern void DISPLAY_DisableBlinking(DISPLAY_LINE_t p_DisplayLine, DISPLAY_ELEMENT_t p_DisplayElement);

#endif /* __DISPLAY_H */
