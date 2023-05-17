/**
 ******************************************************************************
 * @file    board.c
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   Board configuration.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public Function implementations -------------------------------------------*/

/**
 * Configures the different system clocks.
 */
void BOARD_RCC_Configuration(void)
{
    /* PCLK1 = HCLK/2 = 36 MHz */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PCLK2 = HCLK = 72 MHz */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* GPIO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

/**
 * Configures the GPIO Pins.
 */
void BOARD_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /************************************************/
    /*  Pins for I2C1                               */
    /************************************************/

    GPIO_InitStructure.GPIO_Pin = PIN_I2C1_SCL | PIN_I2C1_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;             /* open Drain output */
    GPIO_Init(PORT_I2C1, &GPIO_InitStructure);

    /************************************************/
    /*  Pins for I2C2                               */
    /************************************************/

    GPIO_InitStructure.GPIO_Pin = PIN_I2C2_SCL | PIN_I2C2_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;             /* open Drain output */
    GPIO_Init(PORT_I2C2, &GPIO_InitStructure);

    /************************************************/
    /*  Pins for seconds and AM/PM LEDs             */
    /************************************************/

    GPIO_InitStructure.GPIO_Pin = PIN_LEDS_SECONDS | PIN_LEDS_AMPM_RED | PIN_LEDS_AMPM_GREEN | PIN_LEDS_AMPM_YELLOW;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(PORT_LEDS_SECONDS_AMPM, &GPIO_InitStructure);

    /************************************************/
    /*  Pin for RTC interrupt                       */
    /************************************************/

    GPIO_InitStructure.GPIO_Pin = PIN_RTC_INT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PORT_RTC_INT, &GPIO_InitStructure);

    /************************************************/
    /*  Pin for DCF77 interrupt                     */
    /************************************************/

    GPIO_InitStructure.GPIO_Pin = PIN_DCF77_INT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(PORT_DCF77_INT, &GPIO_InitStructure);
}

/**
 * Configures the nested vectored interrupt controller.
 */
void BOARD_NVIC_Configuration(void)
{
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    /* Ensure that all 4 interrupt priority bits are used as the pre-emption priority. */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}
