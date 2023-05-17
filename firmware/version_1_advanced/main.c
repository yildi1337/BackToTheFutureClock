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
static STATE_t State = STATE_INIT;
static STATE_t NextState = STATE_INIT;

/* Private function prototypes -----------------------------------------------*/
static void BoardConfiguration(void);
static void BTTFC_FSM(void);
static void BTTFC_State_Init(void);
static void BTTFC_State_ShowTime(void);
static void BTTFC_State_DCF77Update(void);
static void BTTFC_State_SetAlarmMonth(void);
static void BTTFC_State_SetAlarmDay(void);
static void BTTFC_State_SetAlarmYear(void);
static void BTTFC_State_SetAlarmHour(void);
static void BTTFC_State_SetAlarmMinute(void);

/* Entry point ---------------------------------------------------------------*/

int main(void)
{
    /* Configure the hardware. */
    BoardConfiguration();

    /* Enter the Back to the Future clock state machine */
    BTTFC_FSM();

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

static void BTTFC_FSM(void)
{
    // ---------------------------------------
    // ------------ INFINITE LOOP ------------
    // ---------------------------------------
    while(1) {

        /* Jump to the next state */
        State = NextState;

        /* Distinguish between the different states */
        switch(State) {

            case STATE_INIT:
                BTTFC_State_Init();
                break;

            case STATE_SHOWTIME:
                BTTFC_State_ShowTime();
                break;

            case STATE_DCF77UPDATE:
                BTTFC_State_DCF77Update();
                break;

            case STATE_SETALARM_MONTH:
                BTTFC_State_SetAlarmMonth();
                break;

            case STATE_SETALARM_DAY:
                BTTFC_State_SetAlarmDay();
                break;

            case STATE_SETALARM_YEAR:
                BTTFC_State_SetAlarmYear();
                break;

            case STATE_SETALARM_HOUR:
                BTTFC_State_SetAlarmHour();
                break;

            case STATE_SETALARM_MINUTE:
                BTTFC_State_SetAlarmMinute();
                break;

            default:
                BTTFC_State_Init();
                break;
        }
    }
}

static void BTTFC_State_Init(void)
{
    // ---------------------------------------
    // ------- INIT MCU-INTERNAL UNITS -------
    // ---------------------------------------
    I2Cx_Init(I2C1);
    I2Cx_Init(I2C2);
    TIM2_Init();
    TIM3_Init();

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

    // ---------------------------------------
    // ------------- INIT ALARM --------------
    // ---------------------------------------
    ALARM_Init();

    // ---------------------------------------
    // ------------ INIT PCF8583 -------------
    // ---------------------------------------
    PCF8583_Init();

    // ---------------------------------------
    // ------------- INIT DCF77 --------------
    // ---------------------------------------
    DCF77_Init();

    // ---------------------------------------
    // --------------- INIT IR ---------------
    // ---------------------------------------
    IR_Init();

    /* Change to the main state */
    DISPLAY_On();
    NextState = STATE_SHOWTIME;
}

static void BTTFC_State_ShowTime(void)
{
    /* Assume to stay in this state */
    NextState = STATE_SHOWTIME;

    if (DCF77_GetUpdateStatus() == DCF77_UPDATE_STATUS_UPDATE_NECESSARY) {

        /* Switch display because of EMV problems during the necessary DCF77 update */
        DISPLAY_Off();
        DCF77_Start();
        NextState = STATE_DCF77UPDATE;
        return;
    }

    /* Check for received IR key in case DCF77 update is not neccessary */
    IR_KEY_t receivedKey = IR_GetReceivedKey();
    switch(receivedKey) {

        case IR_KEY_ARROW_BOTTOM:       break;
        case IR_KEY_ARROW_LEFT:         break;
        case IR_KEY_ARROW_TOP:          break;
        case IR_KEY_ARROW_RIGHT:        break;

        case IR_KEY_ENTER:

            DISPLAY_EnableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_MONTH, DISPLAY_BLINKFREQ_FAST);
            NextState = STATE_SETALARM_MONTH;
            break;

        case IR_KEY_POWER:              break;
        case IR_KEY_QUESTION_MARK:

            /* Switch display because of EMV problems during the necessary DCF77 update */
            DISPLAY_Off();
            DCF77_Start();
            NextState = STATE_DCF77UPDATE;
            break;

        default:                        break;
    }
}

static void BTTFC_State_DCF77Update(void)
{
    if (DCF77_GetUpdateStatus() == DCF77_UPDATE_STATUS_UPDATE_IN_PROGRESS) {

        /* DCF77 update in progress */
        NextState = STATE_DCF77UPDATE;
    }
    else {

        /* DCF77 update just finished */
        DISPLAY_On();
        NextState = STATE_SHOWTIME;
    }
}

static void BTTFC_State_SetAlarmMonth(void)
{
    /* Check for received IR key in case DCF77 update is not neccessary */
    IR_KEY_t receivedKey = IR_GetReceivedKey();
    switch(receivedKey) {

        case IR_KEY_ARROW_BOTTOM:

            ALARM_ChangeMonth(ALARM_CHANGE_TYPE_DECREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_MONTH;
            break;

        case IR_KEY_ARROW_LEFT:         break;
        case IR_KEY_ARROW_TOP:

            ALARM_ChangeMonth(ALARM_CHANGE_TYPE_INCREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_MONTH;
            break;

        case IR_KEY_ARROW_RIGHT:        break;

        case IR_KEY_ENTER:

            DISPLAY_DisableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_MONTH);
            DISPLAY_EnableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_DAY, DISPLAY_BLINKFREQ_FAST);
            NextState = STATE_SETALARM_DAY;
            break;

        case IR_KEY_POWER:              break;
        case IR_KEY_QUESTION_MARK:      break;
        default:

            /* Stay in this state */
            NextState = STATE_SETALARM_MONTH;
            break;
    }
}

static void BTTFC_State_SetAlarmDay(void)
{
    /* Check for received IR key in case DCF77 update is not neccessary */
    IR_KEY_t receivedKey = IR_GetReceivedKey();
    switch(receivedKey) {

        case IR_KEY_ARROW_BOTTOM:

            ALARM_ChangeDay(ALARM_CHANGE_TYPE_DECREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_DAY;
            break;

        case IR_KEY_ARROW_LEFT:         break;
        case IR_KEY_ARROW_TOP:

            ALARM_ChangeDay(ALARM_CHANGE_TYPE_INCREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_DAY;
            break;

        case IR_KEY_ARROW_RIGHT:        break;

        case IR_KEY_ENTER:

            DISPLAY_DisableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_DAY);
            DISPLAY_EnableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_YEAR, DISPLAY_BLINKFREQ_FAST);
            NextState = STATE_SETALARM_YEAR;
            break;

        case IR_KEY_POWER:              break;
        case IR_KEY_QUESTION_MARK:      break;
        default:

            /* Stay in this state */
            NextState = STATE_SETALARM_DAY;
            break;
    }
}

static void BTTFC_State_SetAlarmYear(void)
{
    /* Check for received IR key in case DCF77 update is not neccessary */
    IR_KEY_t receivedKey = IR_GetReceivedKey();
    switch(receivedKey) {

        case IR_KEY_ARROW_BOTTOM:

            ALARM_ChangeYear(ALARM_CHANGE_TYPE_DECREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_YEAR;
            break;

        case IR_KEY_ARROW_LEFT:         break;
        case IR_KEY_ARROW_TOP:

            ALARM_ChangeYear(ALARM_CHANGE_TYPE_INCREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_YEAR;
            break;

        case IR_KEY_ARROW_RIGHT:        break;

        case IR_KEY_ENTER:

            DISPLAY_DisableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_YEAR);
            DISPLAY_EnableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_HOUR, DISPLAY_BLINKFREQ_FAST);
            NextState = STATE_SETALARM_HOUR;
            break;

        case IR_KEY_POWER:              break;
        case IR_KEY_QUESTION_MARK:      break;
        default:

            /* Stay in this state */
            NextState = STATE_SETALARM_YEAR;
            break;
    }
}

static void BTTFC_State_SetAlarmHour(void)
{
    /* Check for received IR key in case DCF77 update is not neccessary */
    IR_KEY_t receivedKey = IR_GetReceivedKey();
    switch(receivedKey) {

        case IR_KEY_ARROW_BOTTOM:

            ALARM_ChangeHour(ALARM_CHANGE_TYPE_DECREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_HOUR;
            break;

        case IR_KEY_ARROW_LEFT:         break;
        case IR_KEY_ARROW_TOP:

            ALARM_ChangeHour(ALARM_CHANGE_TYPE_INCREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_HOUR;
            break;

        case IR_KEY_ARROW_RIGHT:        break;

        case IR_KEY_ENTER:

            DISPLAY_DisableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_HOUR);
            DISPLAY_EnableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_MINUTE, DISPLAY_BLINKFREQ_FAST);
            NextState = STATE_SETALARM_MINUTE;
            break;

        case IR_KEY_POWER:              break;
        case IR_KEY_QUESTION_MARK:      break;
        default:

            /* Stay in this state */
            NextState = STATE_SETALARM_HOUR;
            break;
    }
}

static void BTTFC_State_SetAlarmMinute(void)
{
    /* Check for received IR key in case DCF77 update is not neccessary */
    IR_KEY_t receivedKey = IR_GetReceivedKey();
    switch(receivedKey) {

        case IR_KEY_ARROW_BOTTOM:

            ALARM_ChangeMinute(ALARM_CHANGE_TYPE_DECREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_MINUTE;
            break;

        case IR_KEY_ARROW_LEFT:         break;
        case IR_KEY_ARROW_TOP:

            ALARM_ChangeMinute(ALARM_CHANGE_TYPE_INCREMENT);
            ALARM_UpdateDisplayWithAlarmDateTime();
            NextState = STATE_SETALARM_MINUTE;
            break;

        case IR_KEY_ARROW_RIGHT:        break;

        case IR_KEY_ENTER:

            DISPLAY_DisableBlinking(DISPLAY_LINE_DESTINATION, DISPLAY_ELEMENT_MINUTE);
            NextState = STATE_SHOWTIME;
            break;

        case IR_KEY_POWER:              break;
        case IR_KEY_QUESTION_MARK:      break;
        default:

            /* Stay in this state */
            NextState = STATE_SETALARM_MINUTE;
            break;
    }
}
