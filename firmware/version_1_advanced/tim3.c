/**
 ******************************************************************************
 * @file    tim3.c
 * @author  Phillip Durdaut
 * @date    2014-04-15
 * @brief   TIM3 driver (used for IR).
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
/* Function implementations --------------------------------------------------*/

/**
 * Initialises the timer.
 */
void TIM3_Init(void)
{
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* Enable the TIM3 global Interrupt */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Initialise data. */
    TIM_DeInit(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 40000;           /* 5 ms */
    TIM_TimeBaseStructure.TIM_Prescaler = 8;            /* 72 MHz / (1+8) = 8 MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* TIM3 disable overflow interrupt */
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
}

/**
 * Switches the timer on.
 */
void TIM3_Start(void)
{
    /* Enable timer TIM3 */
    TIM_Cmd(TIM3, ENABLE);

    /* TIM3 enable overflow interrupt */
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

/**
 * Clears and switches the timer on.
 */
void TIM3_ClearAndStart(void)
{
    TIM3_StopAndClear();
    TIM3_Start();
}

/**
 * Switches the timer off.
 */
void TIM3_Stop(void)
{
    /* TIM3 disable overflow interrupt */
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);

    /* Disable timer TIM3 */
    TIM_Cmd(TIM3, DISABLE);
}

/**
 * Stops the timer and sets the timer counter to 0.
 */
void TIM3_StopAndClear(void)
{
    TIM3_Stop();
    TIM_SetCounter(TIM3, 0);
}

/**
 * Returns the timer counter value.
 */
uint16_t TIM3_GetCounterValue(void)
{
    uint16_t value;

    TIM3_Stop();
    value = TIM_GetCounter(TIM3);
    TIM3_Start();

    return value;
}

/**
 * Interrupt service routine.
 */
void TIM3_ISR(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        /* Timer overflow */
        IR_Init();
    }
}
