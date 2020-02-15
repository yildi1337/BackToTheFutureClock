/**
 ******************************************************************************
 * @file    alarm.h
 * @author  Phillip Durdaut
 * @date    2014-04-17
 * @brief   Alarm control.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALARM_H
#define __ALARM_H

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  ALARM_CHANGE_TYPE_INCREMENT = 0,
  ALARM_CHANGE_TYPE_DECREMENT,
} ALARM_CHANGE_TYPE_t;

/* Exported variables---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern void ALARM_Init(void);
extern void ALARM_UpdateDisplayWithAlarmDateTime(void);
extern void ALARM_SetDateTime(DATETIME_DISPLAY_t * p_pDateTime);
extern void ALARM_GetDateTime(DATETIME_DISPLAY_t * p_pDateTime);
extern void ALARM_ChangeMonth(ALARM_CHANGE_TYPE_t p_ChangeType);
extern void ALARM_ChangeDay(ALARM_CHANGE_TYPE_t p_ChangeType);
extern void ALARM_ChangeYear(ALARM_CHANGE_TYPE_t p_ChangeType);
extern void ALARM_ChangeHour(ALARM_CHANGE_TYPE_t p_ChangeType);
extern void ALARM_ChangeMinute(ALARM_CHANGE_TYPE_t p_ChangeType);

#endif /* __ALARM_H */
