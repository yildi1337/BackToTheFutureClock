/**
 ******************************************************************************
 * @file    error.h
 * @author  Phillip Durdaut
 * @date    2014-04-10
 * @brief   Error handling.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ERROR_H
#define __ERROR_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  ERROR_XXX = 0,
} ERROR_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void ERROR_Report(ERROR_t p_Error);

#endif /* __ERROR_H */
