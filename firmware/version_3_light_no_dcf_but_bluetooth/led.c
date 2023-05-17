/**
 ******************************************************************************
 * @file    led.c
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   LED control.
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
static void LED_OnOff(LED_t p_LED, BitAction p_Value);
static GPIO_TypeDef* LED_GetPort(LED_t p_LED);
static uint16_t LED_GetPin(LED_t p_LED);

/* Public Function implementations -------------------------------------------*/

/**
 * Initializes the LEDs.
 */
void LED_Init(void)
{
    LED_Off(LED_SECONDS);
    LED_Off(LED_AMPM_RED);
    LED_Off(LED_AMPM_GREEN);
    LED_Off(LED_AMPM_YELLOW);
}

/**
 * Switches the given LED on.
 */
void LED_On(LED_t p_LED)
{
    switch(p_LED) {
        case LED_SECONDS:       /* active low */
            LED_OnOff(p_LED, Bit_RESET);
        case LED_AMPM_RED:      /* active low */
            LED_OnOff(p_LED, Bit_RESET);
        case LED_AMPM_GREEN:    /* active low */
            LED_OnOff(p_LED, Bit_RESET);
        case LED_AMPM_YELLOW:   /* active low */
            LED_OnOff(p_LED, Bit_RESET);
    }
}

/**
 * Switches the given LED off.
 */
void LED_Off(LED_t p_LED)
{
    switch(p_LED) {
        case LED_SECONDS:       /* active low */
            LED_OnOff(p_LED, Bit_SET);
        case LED_AMPM_RED:      /* active low */
            LED_OnOff(p_LED, Bit_SET);
        case LED_AMPM_GREEN:    /* active low */
            LED_OnOff(p_LED, Bit_SET);
        case LED_AMPM_YELLOW:   /* active low */
            LED_OnOff(p_LED, Bit_SET);
    }
}

/**
 * Toggles the given LED.
 */
void LED_Toggle(LED_t p_LED)
{
    LED_OnOff(p_LED, (BitAction)(1 - GPIO_ReadOutputDataBit(LED_GetPort(p_LED), LED_GetPin(p_LED))));
}

/* Private Function implementations ------------------------------------------*/

/**
 * Switches the given LED on or off.
 */
static void LED_OnOff(LED_t p_LED, BitAction p_Value)
{
    GPIO_WriteBit(LED_GetPort(p_LED), LED_GetPin(p_LED), p_Value);
}

/**
 * Returns the port the given LED is connected to.
 */
static GPIO_TypeDef* LED_GetPort(LED_t p_LED)
{
    switch(p_LED) {
        case LED_SECONDS:
            return PORT_LEDS_SECONDS_AMPM;
        case LED_AMPM_RED:
            return PORT_LEDS_SECONDS_AMPM;
        case LED_AMPM_GREEN:
            return PORT_LEDS_SECONDS_AMPM;
        case LED_AMPM_YELLOW:
            return PORT_LEDS_SECONDS_AMPM;
        default:
            return PORT_LEDS_SECONDS_AMPM;
    }
}

/**
 * Returns the pin the given LED is connected to.
 */
static uint16_t LED_GetPin(LED_t p_LED)
{
    switch(p_LED) {
        case LED_SECONDS:
            return PIN_LEDS_SECONDS;
        case LED_AMPM_RED:
            return PIN_LEDS_AMPM_RED;
        case LED_AMPM_GREEN:
            return PIN_LEDS_AMPM_GREEN;
        case LED_AMPM_YELLOW:
            return PIN_LEDS_AMPM_YELLOW;
        default:
            return PIN_LEDS_SECONDS;
    }
}
