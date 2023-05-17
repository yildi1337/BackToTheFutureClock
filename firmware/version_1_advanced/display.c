/**
 ******************************************************************************
 * @file    display.c
 * @author  Phillip Durdaut
 * @date    2014-04-11
 * @brief   Display controlling.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static DISPLAY_BLINKSTATE_t m_BlinkState[DISPLAY_LINE_CNT][DISPLAY_ELEMENT_CNT];

/* Private function prototypes -----------------------------------------------*/
static uint8_t DISPLAY_MonthToChar(uint8_t p_Month, uint8_t p_Pos);
static uint8_t DISPLAY_Decompose1(uint32_t p_Number);
static uint8_t DISPLAY_Decompose10(uint32_t p_Number);
static uint8_t DISPLAY_Decompose100(uint32_t p_Number);
static uint8_t DISPLAY_Decompose1000(uint32_t p_Number);

/* Public Function implementations -------------------------------------------*/

/**
 * Initialzes the Display.
 */
void DISPLAY_Init(void)
{
    /* Yellow PM LED always on */
    LED_Off(LED_AMPM_YELLOW);
}

/**
 * Enables the MAX6955 driver ICs.
 */
void DISPLAY_On(void)
{
    /* Enable the segment driver ICs */
    MAX6955_Enable(MAX6955_DEVICE_ALPHANUMERIC);
    MAX6955_Enable(MAX6955_DEVICE_NUMERIC_RED);
    MAX6955_Enable(MAX6955_DEVICE_NUMERIC_GREEN);

    /* Enable the RTC interrupt again */
    PCF8583_InterruptEnable();
}

/**
 * Disables the MAX6955 driver ICs.
 */
void DISPLAY_Off(void)
{
    /* Disable the segment driver ICs */
    MAX6955_Disable(MAX6955_DEVICE_ALPHANUMERIC);
    MAX6955_Disable(MAX6955_DEVICE_NUMERIC_RED);
    MAX6955_Disable(MAX6955_DEVICE_NUMERIC_GREEN);

    /* Disable RTC interrupt as this interrupt is only for toggling the seconds
     * LEDs and sending the current date/time to the display driver ICs */
    PCF8583_InterruptDisable();
}

/**
 * Sets the given date/time to the given line of the display.
 */
void DISPLAY_SetDateTime(DISPLAY_LINE_t p_DisplayLine, DATETIME_DISPLAY_t * p_DateTime)
{
    uint8_t hour = p_DateTime->hour;

    /* --- Alphanumeric --- */
    MAX6955_SetChar(MAX6955_DEVICE_ALPHANUMERIC, (p_DisplayLine == DISPLAY_LINE_DESTINATION) ? 0 : 3, DISPLAY_MonthToChar(p_DateTime->month, 0), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_MONTH] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar(MAX6955_DEVICE_ALPHANUMERIC, (p_DisplayLine == DISPLAY_LINE_DESTINATION) ? 1 : 4, DISPLAY_MonthToChar(p_DateTime->month, 1), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_MONTH] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar(MAX6955_DEVICE_ALPHANUMERIC, (p_DisplayLine == DISPLAY_LINE_DESTINATION) ? 2 : 5, DISPLAY_MonthToChar(p_DateTime->month, 2), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_MONTH] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);

    /* --- 7 segments --- */

    /* day */
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 0, DISPLAY_Decompose10(p_DateTime->day), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_DAY] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 1, DISPLAY_Decompose1(p_DateTime->day), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_DAY] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);

    /* year */
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 2, DISPLAY_Decompose1000(p_DateTime->year), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_YEAR] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 3, DISPLAY_Decompose100(p_DateTime->year), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_YEAR] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 4, DISPLAY_Decompose10(p_DateTime->year), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_YEAR] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 5, DISPLAY_Decompose1(p_DateTime->year), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_YEAR] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);

    /* AM/PM LED */
    if (hour > 12) {
        hour -= 12;
        LED_On((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? LED_AMPM_RED : LED_AMPM_GREEN);
    }
    else {
        LED_Off((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? LED_AMPM_RED : LED_AMPM_GREEN);
    }

    /* hour */
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 6, DISPLAY_Decompose10(hour), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_HOUR] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 7, DISPLAY_Decompose1(hour), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_HOUR] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);

    /* second */
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 8, DISPLAY_Decompose10(p_DateTime->minute), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_MINUTE] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
    MAX6955_SetChar((p_DisplayLine == DISPLAY_LINE_DESTINATION) ? MAX6955_DEVICE_NUMERIC_RED : MAX6955_DEVICE_NUMERIC_GREEN, 9, DISPLAY_Decompose1(p_DateTime->minute), (m_BlinkState[p_DisplayLine][DISPLAY_ELEMENT_MINUTE] == DISPLAY_BLINKSTATE_ENABLED) ? MAX6955_PLANE_0 : MAX6955_PLANE_BOTH);
}

/**
 * Enables blinking for the given display character.
 */
void DISPLAY_EnableBlinking(DISPLAY_LINE_t p_DisplayLine, DISPLAY_ELEMENT_t p_DisplayElement, DISPLAY_BLINKFREQ_t p_BlinkFreq)
{
    MAX6955_DEVICE_t max6955Device;

    /* --- Alphanumeric --- */
    if (p_DisplayLine == DISPLAY_LINE_DESTINATION) {

        if (p_DisplayElement == DISPLAY_ELEMENT_MONTH) {

            max6955Device = MAX6955_DEVICE_ALPHANUMERIC;

            /* Write empty char (32) to plane 1 */
            MAX6955_SetChar(max6955Device, 0, 32, MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 1, 32, MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 2, 32, MAX6955_PLANE_1);
        }
        else {

            max6955Device = MAX6955_DEVICE_NUMERIC_RED;
        }
    }
    else if (p_DisplayLine == DISPLAY_LINE_PRESENT) {

        if (p_DisplayElement == DISPLAY_ELEMENT_MONTH) {

            max6955Device = MAX6955_DEVICE_ALPHANUMERIC;

            /* Write empty char (32) to plane 1 */
            MAX6955_SetChar(max6955Device, 3, 32, MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 4, 32, MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 5, 32, MAX6955_PLANE_1);
        }
        else {

            max6955Device = MAX6955_DEVICE_NUMERIC_GREEN;
        }
    }

    /* --- 7 segments --- */
    if (p_DisplayElement != DISPLAY_ELEMENT_MONTH) {

        switch (p_DisplayElement) {

            case DISPLAY_ELEMENT_DAY:

                /* Write empty char (32) to plane 1 */
                MAX6955_SetChar(max6955Device, 0, 32, MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 1, 32, MAX6955_PLANE_1);
                break;

            case DISPLAY_ELEMENT_YEAR:

                /* Write empty char (32) to plane 1 */
                MAX6955_SetChar(max6955Device, 2, 32, MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 3, 32, MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 4, 32, MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 5, 32, MAX6955_PLANE_1);
                break;

            case DISPLAY_ELEMENT_HOUR:

                /* Write empty char (32) to plane 1 */
                MAX6955_SetChar(max6955Device, 6, 32, MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 7, 32, MAX6955_PLANE_1);
                break;

            case DISPLAY_ELEMENT_MINUTE:

                /* Write empty char (32) to plane 1 */
                MAX6955_SetChar(max6955Device, 8, 32, MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 9, 32, MAX6955_PLANE_1);
                break;

            default:    break;
        }
    }

    /* Enable blinking for this segment driver IC */
    MAX6955_EnableBlinking(max6955Device, p_BlinkFreq);
    m_BlinkState[p_DisplayLine][p_DisplayElement] = DISPLAY_BLINKSTATE_ENABLED;
}

/**
 * Disables blinking for the given character.
 */
void DISPLAY_DisableBlinking(DISPLAY_LINE_t p_DisplayLine, DISPLAY_ELEMENT_t p_DisplayElement)
{
    MAX6955_DEVICE_t max6955Device;

    /* --- Alphanumeric --- */
    if (p_DisplayLine == DISPLAY_LINE_DESTINATION) {

        if (p_DisplayElement == DISPLAY_ELEMENT_MONTH) {

            max6955Device = MAX6955_DEVICE_ALPHANUMERIC;

            /* Write char from plane 0 back to plane 1 */
            MAX6955_SetChar(max6955Device, 0, MAX6955_GetChar(max6955Device, 0, MAX6955_PLANE_0), MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 1, MAX6955_GetChar(max6955Device, 1, MAX6955_PLANE_0), MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 2, MAX6955_GetChar(max6955Device, 2, MAX6955_PLANE_0), MAX6955_PLANE_1);
        }
        else {

            max6955Device = MAX6955_DEVICE_NUMERIC_RED;
        }
    }
    else if (p_DisplayLine == DISPLAY_LINE_PRESENT) {

        if (p_DisplayElement == DISPLAY_ELEMENT_MONTH) {

            max6955Device = MAX6955_DEVICE_ALPHANUMERIC;

            /* Write char from plane 0 back to plane 1 */
            MAX6955_SetChar(max6955Device, 3, MAX6955_GetChar(max6955Device, 3, MAX6955_PLANE_0), MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 4, MAX6955_GetChar(max6955Device, 4, MAX6955_PLANE_0), MAX6955_PLANE_1);
            MAX6955_SetChar(max6955Device, 5, MAX6955_GetChar(max6955Device, 5, MAX6955_PLANE_0), MAX6955_PLANE_1);
        }
        else {

            max6955Device = MAX6955_DEVICE_NUMERIC_GREEN;
        }
    }

    /* --- 7 segments --- */
    if (p_DisplayElement != DISPLAY_ELEMENT_MONTH) {

        switch (p_DisplayElement) {

            case DISPLAY_ELEMENT_DAY:

                /* Write char from plane 0 back to plane 1 */
                MAX6955_SetChar(max6955Device, 0, MAX6955_GetChar(max6955Device, 0, MAX6955_PLANE_0), MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 1, MAX6955_GetChar(max6955Device, 1, MAX6955_PLANE_0), MAX6955_PLANE_1);
                break;

            case DISPLAY_ELEMENT_YEAR:

                /* Write char from plane 0 back to plane 1 */
                MAX6955_SetChar(max6955Device, 2, MAX6955_GetChar(max6955Device, 2, MAX6955_PLANE_0), MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 3, MAX6955_GetChar(max6955Device, 3, MAX6955_PLANE_0), MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 4, MAX6955_GetChar(max6955Device, 4, MAX6955_PLANE_0), MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 5, MAX6955_GetChar(max6955Device, 5, MAX6955_PLANE_0), MAX6955_PLANE_1);
                break;

            case DISPLAY_ELEMENT_HOUR:

                /* Write char from plane 0 back to plane 1 */
                MAX6955_SetChar(max6955Device, 6, MAX6955_GetChar(max6955Device, 6, MAX6955_PLANE_0), MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 7, MAX6955_GetChar(max6955Device, 7, MAX6955_PLANE_0), MAX6955_PLANE_1);
                break;

            case DISPLAY_ELEMENT_MINUTE:

                /* Write char from plane 0 back to plane 1 */
                MAX6955_SetChar(max6955Device, 8, MAX6955_GetChar(max6955Device, 8, MAX6955_PLANE_0), MAX6955_PLANE_1);
                MAX6955_SetChar(max6955Device, 9, MAX6955_GetChar(max6955Device, 9, MAX6955_PLANE_0), MAX6955_PLANE_1);
                break;

            default:    break;
        }
    }

    /* Disable blinking for this segment driver IC */
    MAX6955_DisableBlinking(max6955Device);
    m_BlinkState[p_DisplayLine][p_DisplayElement] = DISPLAY_BLINKSTATE_DISABLED;
}

/* Private Function implementations ------------------------------------------*/

/**
 * Returns the three representing characters of a month (0-11).
 */
static uint8_t DISPLAY_MonthToChar(uint8_t p_Month, uint8_t p_Pos)
{
    uint8_t monthChar[12][3] = {
            {'J', 'A', 'N'},
            {'F', 'E', 'B'},
            {'M', 'A', 'R'},
            {'A', 'P', 'R'},
            {'M', 'A', 'Y'},
            {'J', 'U', 'N'},
            {'J', 'U', 'L'},
            {'A', 'U', 'G'},
            {'S', 'E', 'P'},
            {'O', 'C', 'T'},
            {'N', 'O', 'V'},
            {'D', 'E', 'C'}
    };

    return monthChar[p_Month][p_Pos];
}

/**
 * Decomposes the given number and returns the last digit.
 */
static uint8_t DISPLAY_Decompose1(uint32_t p_Number)
{
    return p_Number % 10;
}

/**
 * Decomposes the given number and returns the second last digit.
 */
static uint8_t DISPLAY_Decompose10(uint32_t p_Number)
{
    return p_Number % 100 / 10;
}

/**
 * Decomposes the given number and returns the third last digit.
 */
static uint8_t DISPLAY_Decompose100(uint32_t p_Number)
{
    return p_Number % 1000 / 100;
}

/**
 * Decomposes the given number and returns the fourth last digit.
 */
static uint8_t DISPLAY_Decompose1000(uint32_t p_Number)
{
    return p_Number / 1000;
}
