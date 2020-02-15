/**
 ******************************************************************************
 * @file    delay.c
 * @author  Phillip Durdaut
 * @date    2014-07-14
 * @brief   Busy wait delay functions.
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
static void DELAY(uint32_t p_Delay);

/* Public Function implementations -------------------------------------------*/

/**
 * Busy waiting for the given number of milliseconds.
 */
void DELAY_ms(uint32_t p_Delay_ms)
{
    while (p_Delay_ms > 0) {
        DELAY(DELAY_CYCLES_PER_MS);
        p_Delay_ms--;
    }
}

/**
 * Busy waiting for the given number of microseconds.
 */
void DELAY_us(uint32_t p_Delay_us)
{
    while (p_Delay_us > 0) {
        DELAY(DELAY_CYCLES_PER_US);
        p_Delay_us--;
    }
}

/* Private Function implementations ------------------------------------------*/

/**
 * Busy waiting for the given number of cpu cycles.
 */
static void DELAY(uint32_t p_Delay)
{
    while (p_Delay > 0) {
        asm("nop");
        p_Delay--;
    }
}
