/**
 ******************************************************************************
 * @file    i2cx.h
 * @author  Phillip Durdaut
 * @date    2014-04-10
 * @brief   I2Cx control.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2Cx_H
#define __I2Cx_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
#define I2C1_BUS_SPEED      400000      /* 400 kbps */
#define I2C1_OWN_ADDRESS    0x00

#define I2C2_BUS_SPEED      100000      /* 100 kbps */
#define I2C2_OWN_ADDRESS    0x00

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void I2Cx_Init(I2C_TypeDef * p_pI2Cx);
extern void I2Cx_WriteCommand(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Command);
extern void I2Cx_WriteByte(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Address, uint8_t p_Data);
extern void I2Cx_WriteBuffer(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Address, uint8_t * p_pBuffer, uint8_t p_NumOfBytes);
extern void I2Cx_ReadBuffer(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Address, uint8_t * p_pBuffer, uint8_t p_NumOfBytes);

#endif /* __I2Cx_H */
