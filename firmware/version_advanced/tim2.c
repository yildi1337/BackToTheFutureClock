/**
 ******************************************************************************
 * @file    tim2.c
 * @author  Phillip Durdaut
 * @date    2014-04-15
 * @brief   TIM2 driver (used for DCF-77).
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
void TIM2_Init(void)
{
    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Enable the TIM2 global Interrupt */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Initialise data. */
    TIM_DeInit(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 15000;              /* 1.5 s */
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;            /* 72 MHz / (1+7199) = 10 kHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM2 disable overflow interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}

/**
 * Switches the timer on.
 */
void TIM2_Start(void)
{
    /* Enable timer TIM2 */
    TIM_Cmd(TIM2, ENABLE);

    /* TIM2 enable overflow interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

/**
 * Clears and switches the timer on.
 */
void TIM2_ClearAndStart(void)
{
    TIM2_StopAndClear();
    TIM2_Start();
}

/**
 * Switches the timer off.
 */
void TIM2_Stop(void)
{
    /* TIM2 disable overflow interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);

    /* Disable timer TIM2 */
    TIM_Cmd(TIM2, DISABLE);
}

/**
 * Stops the timer and sets the timer counter to 0.
 */
void TIM2_StopAndClear(void)
{
    TIM2_Stop();
    TIM_SetCounter(TIM2, 0);
}

/**
 * Returns the timer counter value.
 */
uint16_t TIM2_GetCounterValue(void)
{
    uint16_t value;

    TIM2_Stop();
    value = TIM_GetCounter(TIM2);
    TIM2_Start();

    return value;
}

/**
 * Interrupt service routine.
 */
void TIM2_ISR(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        /* Timer overflow */
        DCF77_Start();
    }
}
