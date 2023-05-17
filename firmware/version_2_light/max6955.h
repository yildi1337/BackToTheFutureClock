/**
 ******************************************************************************
 * @file    max6955.h
 * @author  Phillip Durdaut
 * @date    2014-04-09
 * @brief   MAX6955 driver.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAX6955_H
#define __MAX6955_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
#define MAX6955_I2CBUS                              I2C1
#define MAX6955_I2C_ADDRESS_ALPHANUMERIC            0xC0
#define MAX6955_I2C_ADDRESS_NUMERIC_RED             0xC2
#define MAX6955_I2C_ADDRESS_NUMERIC_GREEN           0xC4

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  MAX6955_DEVICE_ALPHANUMERIC = MAX6955_I2C_ADDRESS_ALPHANUMERIC,
  MAX6955_DEVICE_NUMERIC_RED = MAX6955_I2C_ADDRESS_NUMERIC_RED,
  MAX6955_DEVICE_NUMERIC_GREEN = MAX6955_I2C_ADDRESS_NUMERIC_GREEN
} MAX6955_DEVICE_t;

typedef enum
{
  MAX6955_PLANE_0 = 0,
  MAX6955_PLANE_1,
  MAX6955_PLANE_BOTH
} MAX6955_PLANE_t;

typedef enum
{
  MAX6955_BLINKFREQ_SLOW = 0,
  MAX6955_BLINKFREQ_FAST
} MAX6955_BLINKFREQ_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void MAX6955_Init(void);
extern void MAX6955_Enable(MAX6955_DEVICE_t p_Device);
extern void MAX6955_Disable(MAX6955_DEVICE_t p_Device);
extern void MAX6955_SetChar(MAX6955_DEVICE_t p_Device, uint8_t p_Position, uint8_t p_Char, MAX6955_PLANE_t p_Plane);
extern uint8_t MAX6955_GetChar(MAX6955_DEVICE_t p_Device, uint8_t p_Position, MAX6955_PLANE_t p_Plane);
extern void MAX6955_EnableBlinking(MAX6955_DEVICE_t p_Device, MAX6955_BLINKFREQ_t p_BlinkFreq);
extern void MAX6955_DisableBlinking(MAX6955_DEVICE_t p_Device);

#endif /* __MAX6955_H */
