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

/* Port for IR LED */
#define PORT_LED_IR                 GPIOA

/* Port for alarm enable */
#define PORT_ALARM_EN               GPIOA

/* Port for RTC interrupt */
#define PORT_RTC_INT                GPIOA

/* Port for DCF77 interrupt */
#define PORT_DCF77_INT              GPIOA

/* Port for IR interrupt */
#define PORT_IR_INT                 GPIOA

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

/* Pin for IR LED */
#define PIN_LED_IR                  GPIO_Pin_2

/* Pin for alarm enable */
#define PIN_ALARM_EN                GPIO_Pin_5

/* Pin for RTC interrupt */
#define PIN_RTC_INT                 GPIO_Pin_0
#define PIN_RTC_INT_EXTI_LINE       EXTI_Line0
#define PIN_RTC_INT_EXTI_IRQn       EXTI0_IRQn

/* Pin for DCF77 interrupt */
#define PIN_DCF77_INT               GPIO_Pin_1
#define PIN_DCF77_INT_EXTI_LINE     EXTI_Line1
#define PIN_DCF77_INT_EXTI_IRQn     EXTI1_IRQn

/* Pin for IR interrupt */
#define PIN_IR_INT                  GPIO_Pin_3
#define PIN_IR_INT_EXTI_LINE        EXTI_Line3
#define PIN_IR_INT_EXTI_IRQn        EXTI3_IRQn

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void BOARD_RCC_Configuration(void);
extern void BOARD_GPIO_Configuration(void);
extern void BOARD_NVIC_Configuration(void);

#endif /* __BOARD_H */
