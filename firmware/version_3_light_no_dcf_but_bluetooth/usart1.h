/**
 ******************************************************************************
 * @file    usart1.h
 * @author  Phillip Durdaut
 * @date    2023-05-17
 * @brief   USART1 controlling.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART1_H
#define __USART1_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void USART1_Init(uint32_t p_Baudrate, uint16_t p_Mode);
extern void USART1_WriteString(const char * const p_String);
extern uint8_t USART1_ReadBytes(uint8_t * p_Buffer, uint32_t p_Timeout);

#endif /* __USART1_H */
