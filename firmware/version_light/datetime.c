/**
 ******************************************************************************
 * @file    datetime.c
 * @author  Phillip Durdaut
 * @date    2014-04-16
 * @brief   Date/time control.
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
 * Converts date/time from DCF77 to PCF8583.
 */
DATETIME_PCF8583_t DATETIME_ConvertDCF77ToPCF8583(DATETIME_DCF77_t * p_pDateTimeDCF77)
{
    DATETIME_PCF8583_t ret;

    ret.month = p_pDateTimeDCF77->month;
    ret.day = p_pDateTimeDCF77->day;
    ret.year = p_pDateTimeDCF77->year;
    ret.hour = p_pDateTimeDCF77->hour;
    ret.minute = p_pDateTimeDCF77->minute;
    ret.second = 0;
    ret.weekday = (p_pDateTimeDCF77->weekday == 7) ? 0 : p_pDateTimeDCF77->weekday;

    return ret;
}

/**
 * Converts date/time from PCF8583 to display.
 */
DATETIME_DISPLAY_t DATETIME_ConvertPCF8583ToDisplay(DATETIME_PCF8583_t * p_pDateTimePCF8583)
{
    DATETIME_DISPLAY_t ret;

    ret.month = p_pDateTimePCF8583->month - 1;
    ret.day = p_pDateTimePCF8583->day;
    ret.year = p_pDateTimePCF8583->year + 2000;
    ret.hour = p_pDateTimePCF8583->hour;
    ret.minute = p_pDateTimePCF8583->minute;
    ret.second = p_pDateTimePCF8583->second;
    ret.weekday = (p_pDateTimePCF8583->weekday == 0) ? 7 : p_pDateTimePCF8583->weekday;

    return ret;
}

/**
 * Converts date/time from display to PCF8583.
 */
DATETIME_PCF8583_t DATETIME_ConvertDisplayToPCF8583(DATETIME_DISPLAY_t * p_pDateTimeDisplay)
{
    DATETIME_PCF8583_t ret;

    ret.month = p_pDateTimeDisplay->month + 1;
    ret.day = p_pDateTimeDisplay->day;
    ret.year = p_pDateTimeDisplay->year - 2000;
    ret.hour = p_pDateTimeDisplay->hour;
    ret.minute = p_pDateTimeDisplay->minute;
    ret.second = p_pDateTimeDisplay->second;
    ret.weekday = (p_pDateTimeDisplay->weekday == 7) ? 0 : p_pDateTimeDisplay->weekday;

    return ret;
}

/**
 * Returns the difference between to display date/times in seconds.
 */
uint32_t DATETIME_DifferenceInSeconds(DATETIME_DISPLAY_t * p_pDateTimeA, DATETIME_DISPLAY_t * p_pDateTimeB)
{
    uint8_t monthDiff = abs(p_pDateTimeA->month - p_pDateTimeB->month);
    uint8_t dayDiff = abs(p_pDateTimeA->day - p_pDateTimeB->day);
    uint8_t yearDiff = abs(p_pDateTimeA->year - p_pDateTimeB->year);
    uint8_t hourDiff = abs(p_pDateTimeA->hour - p_pDateTimeB->hour);
    uint8_t minuteDiff = abs(p_pDateTimeA->minute - p_pDateTimeB->minute);
    uint8_t secondDiff = abs(p_pDateTimeA->second - p_pDateTimeB->second);

    return (secondDiff + 60 * minuteDiff + 60 * 60 * hourDiff + 60 * 60 * 24 * 365 * yearDiff + 60 * 60 * 24 * dayDiff + 60 * 60 * 24 * 30 * monthDiff);
}
