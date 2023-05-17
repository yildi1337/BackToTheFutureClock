/**
 ******************************************************************************
 * @file    ir.c
 * @author  Phillip Durdaut
 * @date    2014-04-12
 * @brief   IR driver (RC-5).
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define IR_RC5_NUMBER_OF_BITS               14
#define IR_THRESHOLD_HALF_BIT_TIM_TICKS     10000    /* 1.25 ms @ 8 MHz */
#define IR_LOW_PULSE                        0
#define IR_HIGH_PULSE                       1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static EXTITrigger_TypeDef m_InterruptTrigger;
static uint8_t m_ReceivedBits[IR_RC5_NUMBER_OF_BITS];
static uint8_t m_ReceivedBitIndex;
static IR_KEY_t m_ReceivedKey;

/* Private function prototypes -----------------------------------------------*/
static void IR_DecodeStream(uint16_t p_CounterValue);
static void IR_Analysis(void);
static void IR_Action(uint8_t p_Key);
static void IR_InterruptEnable(EXTITrigger_TypeDef p_InterruptTrigger);
static void IR_InterruptDisable(void);
static EXTITrigger_TypeDef IR_GetInterruptTrigger(void);

/* Public Function implementations -------------------------------------------*/

/**
 * Initializes the infrared driver.
 */
void IR_Init(void)
{
    /* Disable IR interrupt */
    IR_InterruptDisable();

    /* Index points on the start bit */
    m_ReceivedBitIndex = 0;

    /* No key received yet */
    m_ReceivedKey = IR_KEY_UNKNOWN;

    /* Start bit means falling edge */
    IR_InterruptEnable(EXTI_Trigger_Falling);
}

/**
 * Interrupt service routine.
 */
void IR_ISR(void)
{
    LED_On(LED_IR);

    if (EXTI_GetITStatus(PIN_IR_INT_EXTI_LINE) != RESET) {
        EXTI_ClearITPendingBit(PIN_IR_INT_EXTI_LINE);

        if (IR_GetInterruptTrigger() == EXTI_Trigger_Falling) {

            if (m_ReceivedBitIndex == 0) {

                /* Start bit detected */
                m_ReceivedBits[0] = 1;
                m_ReceivedBitIndex++;
            }
            else {
                IR_DecodeStream(TIM3_GetCounterValue());
            }

            /* Start the timer and wait for the next rising edge */
            TIM3_ClearAndStart();
            IR_InterruptEnable(EXTI_Trigger_Rising);
        }
        else if (IR_GetInterruptTrigger() == EXTI_Trigger_Rising) {

            IR_DecodeStream(TIM3_GetCounterValue());

            /* Start the timer and wait for the next falling edge */
            TIM3_ClearAndStart();
            IR_InterruptEnable(EXTI_Trigger_Falling);
        }
    }

    LED_Off(LED_IR);
}

/**
 * Returns the last received IR key code.
 */
IR_KEY_t IR_GetReceivedKey(void)
{
    IR_KEY_t tmp = m_ReceivedKey;
    m_ReceivedKey = IR_KEY_UNKNOWN;
    return tmp;
}

/* Private Function implementations ------------------------------------------*/

/**
 * Manchester decoding of the incoming bit stream.
 */
static void IR_DecodeStream(uint16_t p_CounterValue)
{
    if (m_ReceivedBitIndex > 0 && m_ReceivedBitIndex < IR_RC5_NUMBER_OF_BITS) {

        uint8_t lastBit = m_ReceivedBits[m_ReceivedBitIndex-1];
        uint8_t pulse = (p_CounterValue < IR_THRESHOLD_HALF_BIT_TIM_TICKS) ? IR_LOW_PULSE : IR_HIGH_PULSE;

        if (IR_GetInterruptTrigger() == EXTI_Trigger_Falling) {

            if (lastBit == 1) {

                if (pulse == IR_HIGH_PULSE) {
                    /* Invalid Bit */
                    return;
                }

                if (pulse == IR_LOW_PULSE) {
                    /* 1 */
                    m_ReceivedBits[m_ReceivedBitIndex] = 1;
                    m_ReceivedBitIndex++;
                }
            }
            else if (lastBit == 0) {

                if (pulse == IR_HIGH_PULSE) {
                    /* 1 */
                    m_ReceivedBits[m_ReceivedBitIndex] = 1;
                    m_ReceivedBitIndex++;
                }

                if (pulse == IR_LOW_PULSE) {
                    /* NAN */
                }
            }
        }
        else if (IR_GetInterruptTrigger() == EXTI_Trigger_Rising) {

            if (lastBit == 1) {

                if (pulse == IR_HIGH_PULSE) {
                    /* 1 */
                    m_ReceivedBits[m_ReceivedBitIndex] = 0;
                    m_ReceivedBitIndex++;
                }

                if (pulse == IR_LOW_PULSE) {
                    /* NAN */
                }
            }
            else if (lastBit == 0) {

                if (pulse == IR_HIGH_PULSE) {
                    /* Invalid Bit */
                    return;
                }

                if (pulse == IR_LOW_PULSE) {
                    /* 1 */
                    m_ReceivedBits[m_ReceivedBitIndex] = 0;
                    m_ReceivedBitIndex++;
                }
            }
        }

        if (m_ReceivedBitIndex == IR_RC5_NUMBER_OF_BITS) {

            /* All bits received, Analysis can be started */
            IR_InterruptDisable();
            IR_Analysis();
        }

    }
    else {
        /* Received too many bits -> error */
        /* Wait for TIM3 overflow */
    }
}

/**
 * Analyzes the received bits.
 */
static void IR_Analysis(void)
{
    uint8_t systemAddress = 0x00;
    uint8_t key = 0x00;

    systemAddress = (m_ReceivedBits[3] << 4) | (m_ReceivedBits[4] << 3) | (m_ReceivedBits[5] << 2) | (m_ReceivedBits[6] << 1) | (m_ReceivedBits[7] << 0);
    key = (m_ReceivedBits[8] << 5) | (m_ReceivedBits[9] << 4) | (m_ReceivedBits[10] << 3) | (m_ReceivedBits[11] << 2) | (m_ReceivedBits[12] << 1) | (m_ReceivedBits[13] << 0);

    if (systemAddress == IR_SYSTEM_ADDRESS) {

        /* Correct IR remote used */
        IR_Action(key);
    }
}

/**
 * Performs an action depending on the received key.
 */
static void IR_Action(uint8_t p_Key)
{
    switch(p_Key) {

        case IR_KEY_CODE_ARROW_BOTTOM:       m_ReceivedKey = IR_KEY_ARROW_BOTTOM; break;
        case IR_KEY_CODE_ARROW_LEFT:         m_ReceivedKey = IR_KEY_ARROW_LEFT; break;
        case IR_KEY_CODE_ARROW_TOP:          m_ReceivedKey = IR_KEY_ARROW_TOP; break;
        case IR_KEY_CODE_ARROW_RIGHT:        m_ReceivedKey = IR_KEY_ARROW_RIGHT; break;
        case IR_KEY_CODE_ENTER:              m_ReceivedKey = IR_KEY_ENTER; break;
        case IR_KEY_CODE_POWER:              m_ReceivedKey = IR_KEY_POWER; break;
        case IR_KEY_CODE_QUESTION_MARK:      m_ReceivedKey = IR_KEY_QUESTION_MARK; break;
        default:                             m_ReceivedKey = IR_KEY_UNKNOWN; break;
    }
}

/**
 * Enables the IR interrupt with the given trigger.
 */
static void IR_InterruptEnable(EXTITrigger_TypeDef p_InterruptTrigger)
{
    /* PPM input signal pin can trigger an interrupt */
    EXTI_InitTypeDef  EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = PIN_IR_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = p_InterruptTrigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    m_InterruptTrigger = p_InterruptTrigger;

    /* Enable Interrupt */
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = PIN_IR_INT_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * Disables the IR interrupt.
 */
static void IR_InterruptDisable(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = PIN_IR_INT_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * Returns the currently configured interrupt trigger.
 */
static EXTITrigger_TypeDef IR_GetInterruptTrigger(void)
{
    return m_InterruptTrigger;
}
