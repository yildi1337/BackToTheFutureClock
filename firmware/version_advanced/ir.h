/**
 ******************************************************************************
 * @file    ir.h
 * @author  Phillip Durdaut
 * @date    2014-04-12
 * @brief   IR driver (RC-5).
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IR_H
#define __IR_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
#define IR_SYSTEM_ADDRESS            0x15    /* Grundig */
#define IR_KEY_CODE_ARROW_BOTTOM     0x11
#define IR_KEY_CODE_ARROW_LEFT       0x15
#define IR_KEY_CODE_ARROW_TOP        0x10
#define IR_KEY_CODE_ARROW_RIGHT      0x16
#define IR_KEY_CODE_ENTER            0x17
#define IR_KEY_CODE_POWER            0x0C
#define IR_KEY_CODE_QUESTION_MARK    0x2F

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {
    IR_KEY_UNKNOWN,
    IR_KEY_ARROW_BOTTOM,
    IR_KEY_ARROW_LEFT,
    IR_KEY_ARROW_TOP,
    IR_KEY_ARROW_RIGHT,
    IR_KEY_ENTER,
    IR_KEY_POWER,
    IR_KEY_QUESTION_MARK
} IR_KEY_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void IR_Init(void);
extern void IR_ISR(void);
extern IR_KEY_t IR_GetReceivedKey(void);

#endif /* __IR_H */
