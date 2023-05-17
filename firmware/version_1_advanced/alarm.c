/**
 ******************************************************************************
 * @file    alarm.c
 * @author  Phillip Durdaut
 * @date    2014-04-17
 * @brief   Alarm control.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DATETIME_DISPLAY_t m_AlarmDateTime;

/* Private function prototypes -----------------------------------------------*/
static void ALARM_UpdateAlarmTimeInRTC(void);

/* Public Function implementations -------------------------------------------*/

/**
 * TODO
 */
void ALARM_Init(void)
{
    DATETIME_PCF8583_t dateTimeFromRTC = { 11, 4, 14, 13, 37, 0, 2 };
    //PCF8583_GetDateTime(&dateTimeFromRTC);
    //PCF8583_SetAlarmDateTime(&dateTimeFromRTC);
    m_AlarmDateTime = DATETIME_ConvertPCF8583ToDisplay(&dateTimeFromRTC);
    ALARM_UpdateDisplayWithAlarmDateTime();
}

/**
 * TODO
 */
void ALARM_UpdateDisplayWithAlarmDateTime(void)
{
    DISPLAY_SetDateTime(DISPLAY_LINE_DESTINATION, &m_AlarmDateTime);
}

/**
 * TODO
 */
void ALARM_SetDateTime(DATETIME_DISPLAY_t * p_pDateTime)
{
    m_AlarmDateTime.month = p_pDateTime->month;
    m_AlarmDateTime.day = p_pDateTime->day;
    m_AlarmDateTime.year = p_pDateTime->year;
    m_AlarmDateTime.hour = p_pDateTime->hour;
    m_AlarmDateTime.minute = p_pDateTime->minute;
    m_AlarmDateTime.second = p_pDateTime->second;
    m_AlarmDateTime.weekday = p_pDateTime->weekday;

    ALARM_UpdateAlarmTimeInRTC();
}

/**
 * TODO
 */
void ALARM_GetDateTime(DATETIME_DISPLAY_t * p_pDateTime)
{
    p_pDateTime->month = m_AlarmDateTime.month;
    p_pDateTime->day = m_AlarmDateTime.day;
    p_pDateTime->year = m_AlarmDateTime.year;
    p_pDateTime->hour = m_AlarmDateTime.hour;
    p_pDateTime->minute = m_AlarmDateTime.minute;
    p_pDateTime->second = m_AlarmDateTime.second;
    p_pDateTime->weekday = m_AlarmDateTime.weekday;
}

/**
 * TODO
 */
void ALARM_ChangeMonth(ALARM_CHANGE_TYPE_t p_ChangeType)
{
    /* Borders: 0-11 */
    if (p_ChangeType == ALARM_CHANGE_TYPE_INCREMENT && m_AlarmDateTime.month < 11)
        m_AlarmDateTime.month++;

    if (p_ChangeType == ALARM_CHANGE_TYPE_DECREMENT && m_AlarmDateTime.month > 0)
        m_AlarmDateTime.month--;

    ALARM_UpdateAlarmTimeInRTC();
}

/**
 * TODO
 */
void ALARM_ChangeDay(ALARM_CHANGE_TYPE_t p_ChangeType)
{
    /* Borders: 1-31 */
    if (p_ChangeType == ALARM_CHANGE_TYPE_INCREMENT && m_AlarmDateTime.day < 31)
        m_AlarmDateTime.day++;

    if (p_ChangeType == ALARM_CHANGE_TYPE_DECREMENT && m_AlarmDateTime.day > 1)
        m_AlarmDateTime.day--;

    ALARM_UpdateAlarmTimeInRTC();
}

/**
 * TODO
 */
void ALARM_ChangeYear(ALARM_CHANGE_TYPE_t p_ChangeType)
{
    /* Borders: 2014-2099 */
    if (p_ChangeType == ALARM_CHANGE_TYPE_INCREMENT && m_AlarmDateTime.year < 2099)
        m_AlarmDateTime.year++;

    if (p_ChangeType == ALARM_CHANGE_TYPE_DECREMENT && m_AlarmDateTime.year > 2014)
        m_AlarmDateTime.year--;

    ALARM_UpdateAlarmTimeInRTC();
}

/**
 * TODO
 */
void ALARM_ChangeHour(ALARM_CHANGE_TYPE_t p_ChangeType)
{
    /* Borders: 0-23 */
    if (p_ChangeType == ALARM_CHANGE_TYPE_INCREMENT && m_AlarmDateTime.hour < 23)
        m_AlarmDateTime.hour++;

    if (p_ChangeType == ALARM_CHANGE_TYPE_DECREMENT && m_AlarmDateTime.hour > 0)
        m_AlarmDateTime.hour--;

    ALARM_UpdateAlarmTimeInRTC();
}

/**
 * TODO
 */
void ALARM_ChangeMinute(ALARM_CHANGE_TYPE_t p_ChangeType)
{
    /* Borders: 0-59 */
    if (p_ChangeType == ALARM_CHANGE_TYPE_INCREMENT && m_AlarmDateTime.minute < 59)
        m_AlarmDateTime.minute++;

    if (p_ChangeType == ALARM_CHANGE_TYPE_DECREMENT && m_AlarmDateTime.minute > 0)
        m_AlarmDateTime.minute--;

    ALARM_UpdateAlarmTimeInRTC();
}

/* Private Function implementations ------------------------------------------*/

/**
 * TODO
 */
static void ALARM_UpdateAlarmTimeInRTC(void)
{
    //DATETIME_PCF8583_t alarmDateTimeForRTC = DATETIME_ConvertDisplayToPCF8583(&m_AlarmDateTime);
    //PCF8583_SetAlarmDateTime(&alarmDateTimeForRTC);
}
