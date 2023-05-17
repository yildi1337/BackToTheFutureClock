/**
 ******************************************************************************
 * @file    main.c
 * @author  Phillip Durdaut
 * @date    2023-05-17
 * @brief   Back to the Future clock v0.1 main program.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 *
 * @status  - There might be a bug in I2C communication as on startup the
 *            date/time from the RTC is sometimes read wrong.
 *          - DCF77 not implemented in this version.
 *          - Instead, bluetooth/USART communication is used for updating
 *            date/time in RTC.
 *          - Infrared not omplemented in this version.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define RX_BUFFER_SIZE_IN_BYTES         100
#define RX_TIMEOUT_MILLISECONDS         10
#define RX_NUMBER_OF_EXPECTED_BYTES     19

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DATETIME_DISPLAY_t dateTimeDisplayDestination = { 9, 13, 2040, 18, 54, 0, 0 };
uint8_t rxBuffer[RX_BUFFER_SIZE_IN_BYTES] = { 0 };

/* Private function prototypes -----------------------------------------------*/
static void BoardConfiguration(void);
static uint8_t DayOfWeek(uint8_t y, uint8_t m, uint8_t d);

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
    DISPLAY_SetDateTime(DISPLAY_LINE_DESTINATION, &dateTimeDisplayDestination);

    // ---------------------------------------
    // ------------ INIT PCF8583 -------------
    // ---------------------------------------
    PCF8583_Init();

    // ---------------------------------------
    // ------- INIT USART1 / BLUETOOTH -------
    // ---------------------------------------
    /* This configuration is only needed once (non-volatile memory on bluetooth module) */
    //USART1_Init(38400, USART_Mode_Tx);
    //USART1_WriteString("AT+NAME=Back to the Future Clock\r\n");
    //USART1_WriteString("AT+PSWD=1337\r\n");

    // ---------------------------------------
    // ------------- INIT USART1 -------------
    // ---------------------------------------
    USART1_Init(9600, USART_Mode_Rx);

    // ---------------------------------------
    // ------------ INFINITE LOOP ------------
    // ---------------------------------------
    while(1) {

        /* Clear receive buffer */
        for (uint8_t i = 0; i < RX_BUFFER_SIZE_IN_BYTES; i++)
            rxBuffer[i] = 0;

        /* Receives data from the UART */
        uint8_t numberOfReceivedBytes = USART1_ReadBytes(rxBuffer, RX_TIMEOUT_MILLISECONDS);

        /* Parse received data if number of bytes is correct */
        if (numberOfReceivedBytes == RX_NUMBER_OF_EXPECTED_BYTES) {

            /* Date/time format */
            int month;      /* 1 .. 12 */
            int day;        /* 1 .. 31 */
            int year;      /* < 2000, for example 14 for 2014 */
            int hour;       /* 0 to 23 */
            int minute;     /* 0 to 59 */
            int second;     /* 0 to 59 */
            int weekday;    /* 0 -> sunday, 1 -> monday .. 6 -> saturday */

            /* Parse data */
            sscanf((const char *)rxBuffer, "%d-%d-%d_%d:%d:%d", (int *)&year, (int *)&month, (int *)&day, (int *)&hour, (int *)&minute, (int *)&second);
            year = year - 2000;
            weekday = DayOfWeek((uint8_t)year, (uint8_t)month, (uint8_t)day);

            /* Update RTC */
            DATETIME_PCF8583_t dateTimeReceived = { (uint8_t)month, (uint8_t)day, (uint32_t)year, (uint8_t)hour, (uint8_t)minute, (uint8_t)second, (uint8_t)weekday };
            PCF8583_SetDateTime(&dateTimeReceived);
        }
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

static uint8_t DayOfWeek(uint8_t y, uint8_t m, uint8_t d)
{
    uint8_t x = y + 24 - (m < 3);
    x = x + (x >> 2) + d - m;
    if (m & 1)
        x += (m & 8) ? 4 : 3;
    if (m < 3)
        x += 3;

    return x % 7;
}
