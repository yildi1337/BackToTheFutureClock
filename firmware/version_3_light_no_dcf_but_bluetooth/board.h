/**
 ******************************************************************************
 * @file    board.h
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   Board configuration.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARD_H
#define __BOARD_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/

/************************************************/
/*  Ports                                       */
/************************************************/

/* Port for I2C1 */
#define PORT_I2C1                   GPIOB

/* Port for I2C2 */
#define PORT_I2C2                   GPIOB

/* Port for seconds and AM/PM LEDs */
#define PORT_LEDS_SECONDS_AMPM      GPIOC

/* Port for RTC interrupt */
#define PORT_RTC_INT                GPIOA

/* Port for USART1 */
#define PORT_USART1                 GPIOA

/************************************************/
/*  Pins                                        */
/************************************************/

/* Pins for I2C1 */
#define PIN_I2C1_SCL                GPIO_Pin_6
#define PIN_I2C1_SDA                GPIO_Pin_7

/* Pins for I2C2 */
#define PIN_I2C2_SCL                GPIO_Pin_10
#define PIN_I2C2_SDA                GPIO_Pin_11

/* Pins for for seconds and AM/PM LEDs */
#define PIN_LEDS_SECONDS            GPIO_Pin_0
#define PIN_LEDS_AMPM_RED           GPIO_Pin_1
#define PIN_LEDS_AMPM_GREEN         GPIO_Pin_2
#define PIN_LEDS_AMPM_YELLOW        GPIO_Pin_3

/* Pin for RTC interrupt */
#define PIN_RTC_INT                 GPIO_Pin_0
#define PIN_RTC_INT_EXTI_LINE       EXTI_Line0
#define PIN_RTC_INT_EXTI_IRQn       EXTI0_IRQn

/* Pins for USART1 */
#define PIN_USART1_TX               GPIO_Pin_9
#define PIN_USART1_RX               GPIO_Pin_10

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void BOARD_RCC_Configuration(void);
extern void BOARD_GPIO_Configuration(void);
extern void BOARD_NVIC_Configuration(void);

#endif /* __BOARD_H */
