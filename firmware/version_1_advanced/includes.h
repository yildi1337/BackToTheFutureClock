/**
 ******************************************************************************
 * @file    includes.h
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   All needed includes.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INCLUDES_H
#define __INCLUDES_H

/* Includes ------------------------------------------------------------------*/

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

/* STM32 Library includes. */
#include "stm32f10x.h"
#include "stm32_dsp.h"

/* Program includes. */
#include "main.h"
#include "datetime.h"
#include "alarm.h"
#include "board.h"
#include "dcf77.h"
#include "delay.h"
#include "display.h"
#include "error.h"
#include "i2cx.h"
#include "ir.h"
#include "led.h"
#include "max6955.h"
#include "pcf8583.h"
#include "stm32f10x_irq.h"
#include "tim2.h"
#include "tim3.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __INCLUDES_H */
