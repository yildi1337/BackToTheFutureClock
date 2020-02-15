/**
 ******************************************************************************
 * @file    main.h
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   Back to the Future Clock v0.1 main program.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
    STATE_INIT,
    STATE_SHOWTIME,
    STATE_DCF77UPDATE
} STATE_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
