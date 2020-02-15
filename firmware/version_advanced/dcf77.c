/**
 ******************************************************************************
 * @file    dcf77.c
 * @author  Phillip Durdaut
 * @date    2014-04-12
 * @brief   DCF77 driver.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  DCF77_PULSE_ERROR = 0,
  DCF77_PULSE_SHORT,
  DCF77_PULSE_LONG
} DCF77_PULSE_t;

typedef enum
{
  DCF77_PARITY_OK = 0,
  DCF77_PARITY_NOK
} DCF77_PARITY_t;

/* Private define ------------------------------------------------------------*/
#define DCF77_NUMBER_OF_BITS                59
#define DCF77_TIM_TICKS_SHORT_PULSE_MIN     800     /* 80 ms @ 10 kHz */
#define DCF77_TIM_TICKS_SHORT_PULSE_MAX     1200    /* 120 ms @ 10 kHz */
#define DCF77_TIM_TICKS_LONG_PULSE_MIN      1800    /* 180 ms @ 10 kHz */
#define DCF77_TIM_TICKS_LONG_PULSE_MAX      2200    /* 220 ms @ 10 kHz */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static EXTITrigger_TypeDef m_InterruptTrigger;
static uint8_t m_ReceivedBits[DCF77_NUMBER_OF_BITS];
static uint8_t m_ReceivedBitIndex;
static uint32_t m_UpdateStatus;

/* Private function prototypes -----------------------------------------------*/
static void DCF77_DecodeStream(uint16_t p_CounterValue);
static void DCF77_Analysis(void);
static DCF77_PARITY_t DCF77_CheckParity(uint8_t p_LowerIndex, uint8_t p_UpperIndex);
static void DCF77_InterruptEnable(EXTITrigger_TypeDef p_Trigger);
static void DCF77_InterruptDisable(void);
static EXTITrigger_TypeDef DCF77_GetInterruptTrigger(void);

/* Public Function implementations -------------------------------------------*/

/**
 * Initializes the DCF77 module.
 */
void DCF77_Init(void)
{
    /* Disable DCF77 interrupt */
    DCF77_InterruptDisable();

    /* Timer not yet needed */
    TIM2_StopAndClear();

    /* Index points on the start bit */
    m_ReceivedBitIndex = 0;

    /* Update the DCF77 update status */
    m_UpdateStatus = DCF77_GetUpdateStatus();
}

/**
 * Starts DCF77 reception and decoding by enabling the external interrupt.
 */
void DCF77_Start(void)
{
    DCF77_Init();

    /* Update is in progress from now on */
    m_UpdateStatus = DCF77_UPDATE_STATUS_UPDATE_IN_PROGRESS;

    /* Start bit means rising edge */
    DCF77_InterruptEnable(EXTI_Trigger_Rising);
}

/**
 * Stops DCF77 reception and decoding.
 */
void DCF77_Stop(void)
{
    DCF77_Init();

    /* Update just finished */
    m_UpdateStatus = DCF77_UPDATE_STATUS_UPDATE_NOT_NECESSARY;
}

/**
 * Interrupt service routine.
 */
void DCF77_ISR(void)
{
    if (EXTI_GetITStatus(PIN_DCF77_INT_EXTI_LINE) != RESET) {
        EXTI_ClearITPendingBit(PIN_DCF77_INT_EXTI_LINE);

       if (DCF77_GetInterruptTrigger() == EXTI_Trigger_Rising) {

            /* Start the timer and wait for the next falling edge */
            TIM2_ClearAndStart();
            DCF77_InterruptEnable(EXTI_Trigger_Falling);
        }
        else if (DCF77_GetInterruptTrigger() == EXTI_Trigger_Falling) {

            if (m_ReceivedBitIndex == 0) {

                /* Start bit detected */
                m_ReceivedBits[0] = 0;
                m_ReceivedBitIndex++;
            }
            else {
                DCF77_DecodeStream(TIM2_GetCounterValue());
            }

            /* Wait for the next rising edge */
            DCF77_InterruptEnable(EXTI_Trigger_Rising);
        }
    }
}

/**
 * Returns whether an update is necessary or not.
 */
DCF77_UPDATE_STATUS_t DCF77_GetUpdateStatus(void)
{
    if (m_UpdateStatus == DCF77_UPDATE_STATUS_UPDATE_IN_PROGRESS)
        return DCF77_UPDATE_STATUS_UPDATE_IN_PROGRESS;

    /* The following code is only executed when no update is in progress */

    /* Get current and last DCF77 update date/time from RTC */
    DATETIME_PCF8583_t dateTimeFromRTC = { 0, 0, 0, 0, 0, 0, 0 };
    PCF8583_GetDateTime(&dateTimeFromRTC);
    DATETIME_PCF8583_t lastDCF77UpdateDateTimeFromRTC = { 0, 0, 0, 0, 0, 0, 0 };
    PCF8583_GetLastDCF77UpdateDateTime(&lastDCF77UpdateDateTimeFromRTC);

    /* Calculate the differnce between the date/time in seconds */
    DATETIME_DISPLAY_t currentDateTime = DATETIME_ConvertPCF8583ToDisplay(&dateTimeFromRTC);
    DATETIME_DISPLAY_t lastDCF77UpdateDateTime = DATETIME_ConvertPCF8583ToDisplay(&lastDCF77UpdateDateTimeFromRTC);
    uint32_t diffSeconds = DATETIME_DifferenceInSeconds(&currentDateTime, &lastDCF77UpdateDateTime);

    /* Decide whether an update is neccessary or not */
    if (diffSeconds > DCF77_UPDATE_EVERY_X_SECONDS)
        m_UpdateStatus = DCF77_UPDATE_STATUS_UPDATE_NECESSARY;
    else
        m_UpdateStatus = DCF77_UPDATE_STATUS_UPDATE_NOT_NECESSARY;

    return m_UpdateStatus;
}

/* Private Function implementations ------------------------------------------*/

/**
 * Decoding of the incoming bit stream.
 */
static void DCF77_DecodeStream(uint16_t p_CounterValue)
{
    if (m_ReceivedBitIndex > 0 && m_ReceivedBitIndex < DCF77_NUMBER_OF_BITS) {

        DCF77_PULSE_t pulse = DCF77_PULSE_ERROR;
        if (p_CounterValue >= DCF77_TIM_TICKS_SHORT_PULSE_MIN && p_CounterValue <= DCF77_TIM_TICKS_SHORT_PULSE_MAX)
            pulse = DCF77_PULSE_SHORT;
        else if (p_CounterValue >= DCF77_TIM_TICKS_LONG_PULSE_MIN && p_CounterValue <= DCF77_TIM_TICKS_LONG_PULSE_MAX)
            pulse = DCF77_PULSE_LONG;

        if (pulse == DCF77_PULSE_SHORT) {
            /* 0 */
            m_ReceivedBits[m_ReceivedBitIndex] = 0;
            m_ReceivedBitIndex++;
        }

        if (pulse == DCF77_PULSE_LONG) {
            /* 1 */
            m_ReceivedBits[m_ReceivedBitIndex] = 1;
            m_ReceivedBitIndex++;
        }

        if (m_ReceivedBitIndex == DCF77_NUMBER_OF_BITS) {

            /* All bits received, Analysis can be started */
            DCF77_InterruptDisable();
            DCF77_Analysis();
        }

    }
    else {
        /* Received too many bits -> error */
        DCF77_Start();
    }
}

/**
 * Checks the received DCF77 bits and updates the RTC in case the parity check was successful.
 */
static void DCF77_Analysis(void) {

    DATETIME_DCF77_t decodedDateTime = { 0, 0, 0, 0, 0, 0 };

    /* Start bit or bit 20 wrong? */
    if (m_ReceivedBits[0] == 1 || m_ReceivedBits[20] == 0)
        return;

    decodedDateTime.minute = m_ReceivedBits[21] * 1 +
                             m_ReceivedBits[22] * 2 +
                             m_ReceivedBits[23] * 4 +
                             m_ReceivedBits[24] * 8 +
                             m_ReceivedBits[25] * 10 +
                             m_ReceivedBits[26] * 20 +
                             m_ReceivedBits[27] * 40;

    decodedDateTime.hour = m_ReceivedBits[29] * 1 +
                           m_ReceivedBits[30] * 2 +
                           m_ReceivedBits[31] * 4 +
                           m_ReceivedBits[32] * 8 +
                           m_ReceivedBits[33] * 10 +
                           m_ReceivedBits[34] * 20;

    decodedDateTime.day = m_ReceivedBits[36] * 1 +
                          m_ReceivedBits[37] * 2 +
                          m_ReceivedBits[38] * 4 +
                          m_ReceivedBits[39] * 8 +
                          m_ReceivedBits[40] * 10 +
                          m_ReceivedBits[41] * 20;

    decodedDateTime.weekday = m_ReceivedBits[42] * 1 +
                              m_ReceivedBits[43] * 2 +
                              m_ReceivedBits[44] * 4;
    if (decodedDateTime.weekday == 7)
        decodedDateTime.weekday = 0;

    decodedDateTime.month = m_ReceivedBits[45] * 1 +
                            m_ReceivedBits[46] * 2 +
                            m_ReceivedBits[47] * 4 +
                            m_ReceivedBits[48] * 8 +
                            m_ReceivedBits[49] * 10;

    decodedDateTime.year = m_ReceivedBits[50] * 1 +
                           m_ReceivedBits[51] * 2 +
                           m_ReceivedBits[52] * 4 +
                           m_ReceivedBits[53] * 8 +
                           m_ReceivedBits[54] * 10 +
                           m_ReceivedBits[55] * 20 +
                           m_ReceivedBits[56] * 40 +
                           m_ReceivedBits[57] * 80;

    /* Check parity */
    if (DCF77_CheckParity(21, 28) == DCF77_PARITY_OK && DCF77_CheckParity(29, 35) == DCF77_PARITY_OK && DCF77_CheckParity(36, 58) == DCF77_PARITY_OK) {

        /* Successfully received and decoded current date/time */
        /* Update RTC and leave DCF77 update sequence */
        DATETIME_PCF8583_t dateTimeForRTC = DATETIME_ConvertDCF77ToPCF8583(&decodedDateTime);
        PCF8583_SetDateTime(&dateTimeForRTC);
        PCF8583_SetLastDCF77UpdateDateTime(&dateTimeForRTC);
        DCF77_Stop();
    }
}

/**
 * Performs the parity check for the bits of the given indexes.
 */
static DCF77_PARITY_t DCF77_CheckParity(uint8_t p_LowerIndex, uint8_t p_UpperIndex)
{
    uint8_t i;
    uint8_t parity = 0;

    for (i = p_LowerIndex; i <= p_UpperIndex; i++)
        parity += m_ReceivedBits[i];

    if (parity % 2 == 0)
        return DCF77_PARITY_OK;
    else
        return DCF77_PARITY_NOK;
}


/**
 * Enables the DCF77 interrupt with the given trigger.
 */
static void DCF77_InterruptEnable(EXTITrigger_TypeDef p_InterruptTrigger)
{
    /* PPM input signal pin can trigger an interrupt */
    EXTI_InitTypeDef  EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = PIN_DCF77_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = p_InterruptTrigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    m_InterruptTrigger = p_InterruptTrigger;

    /* Enable Interrupt */
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = PIN_DCF77_INT_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * Disables the IR interrupt.
 */
static void DCF77_InterruptDisable(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = PIN_DCF77_INT_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * Returns the currently configured interrupt trigger.
 */
static EXTITrigger_TypeDef DCF77_GetInterruptTrigger(void)
{
    return m_InterruptTrigger;
}
