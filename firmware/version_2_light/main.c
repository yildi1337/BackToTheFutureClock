/**
 ******************************************************************************
 * @file    main.c
 * @author  Phillip Durdaut
 * @date    2014-04-08
 * @brief   Back to the Future clock v0.1 main program.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 *
 * @status  - There might be a bug in I2C communication as on startup the
 *            date/time from the RTC is sometimes read wrong.
 *          - Alarm (incl. DAC/DMA) not yet implemented.
 *          - Bluetooth/USART communication not yet implemented.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BoardConfiguration(void);

/* Entry point ---------------------------------------------------------------*/

int main(void)
{
    /* Configure the hardware. */
    BoardConfiguration();

    // ---------------------------------------
    // ------- INIT MCU-INTERNAL UNITS -------
    // ---------------------------------------
    I2Cx_Init(I2C1);
    I2Cx_Init(I2C2);
    TIM2_Init();

    // ---------------------------------------
    // ------------- INIT LEDS ---------------
    // ---------------------------------------
    LED_Init();

    // ---------------------------------------
    // ------------ INIT MAX6955 -------------
    // ---------------------------------------
    MAX6955_Init();

    // ---------------------------------------
    // ------------ INIT DISPLAY -------------
    // ---------------------------------------
    DISPLAY_Init();
    DISPLAY_On();

    DATETIME_DISPLAY_t dateTimeForDisplay = { 7, 1, 2019, 11, 30, 0, 0 };
    DISPLAY_SetDateTime(DISPLAY_LINE_DESTINATION, &dateTimeForDisplay);

    // ---------------------------------------
    // ------------ INIT PCF8583 -------------
    // ---------------------------------------
    PCF8583_Init();

    // ---------------------------------------
    // ------------- INIT DCF77 --------------
    // ---------------------------------------
    DCF77_Init();
    DCF77_Start();

    // ---------------------------------------
    // ------------ INFINITE LOOP ------------
    // ---------------------------------------
    while(1) {

    }

    /* Never reached */
    return 0;
}

/* Private Function implementations ------------------------------------------*/

static void BoardConfiguration(void)
{
    /* System clocks configuration */
    BOARD_RCC_Configuration();

    /* GPIO configuration */
    BOARD_GPIO_Configuration();

    /* NVIC configuration */
    BOARD_NVIC_Configuration();
}
