/**
 ******************************************************************************
 * @file    datetime.h
 * @author  Phillip Durdaut
 * @date    2014-04-16
 * @brief   Date/time control.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATETIME_H
#define __DATETIME_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Date/time used by the display */
typedef struct {
    uint8_t month;      /* 0 .. 11 */
    uint8_t day;        /* 1 .. 31 */
    uint32_t year;      /* > 2000 */
    uint8_t hour;       /* 0 to 23 */
    uint8_t minute;     /* 0 to 59 */
    uint8_t second;     /* 0 to 59 */
    uint8_t weekday;    /* 1 -> monday .. 7 -> sunday */
} DATETIME_DISPLAY_t;

/* Date/time used by the RTC */
typedef struct {
    uint8_t month;      /* 1 .. 12 */
    uint8_t day;        /* 1 .. 31 */
    uint32_t year;      /* < 2000, for example 14 for 2014 */
    uint8_t hour;       /* 0 to 23 */
    uint8_t minute;     /* 0 to 59 */
    uint8_t second;     /* 0 to 59 */
    uint8_t weekday;    /* 0 -> sunday, 1 -> monday .. 6 -> saturday */
} DATETIME_PCF8583_t;

/* Date/time used by DCF77 */
typedef struct {
    uint8_t month;      /* 1 .. 12 */
    uint8_t day;        /* 1 .. 31 */
    uint32_t year;      /* < 2000, for example 14 for 2014 */
    uint8_t hour;       /* 0 to 23 */
    uint8_t minute;     /* 0 to 59 */
    uint8_t weekday;    /* 1 -> monday .. 7 -> sunday */
} DATETIME_DCF77_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern DATETIME_PCF8583_t DATETIME_ConvertDCF77ToPCF8583(DATETIME_DCF77_t * p_pDateTimeDCF77);
extern DATETIME_DISPLAY_t DATETIME_ConvertPCF8583ToDisplay(DATETIME_PCF8583_t * p_pDateTimePCF8583);
extern DATETIME_PCF8583_t DATETIME_ConvertDisplayToPCF8583(DATETIME_DISPLAY_t * p_pDateTimeDisplay);
extern uint32_t DATETIME_DifferenceInSeconds(DATETIME_DISPLAY_t * p_pDateTimeA, DATETIME_DISPLAY_t * p_pDateTimeB);

#endif /* __DATETIME_H */
