/**
 ******************************************************************************
 * @file    stm32f10x_irq.c
 * @author  Phillip Durdaut
 * @date    2014-04-10
 * @brief   Interrupt Service Routines.
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
/* Private functions ---------------------------------------------------------*/

/* Public Function implementations -------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * Handles NMI exception.
 */
void NMI_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
 * Handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
 * Handles Memory Manage exception.
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

/**
 * Handles Bus Fault exception.
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

/**
 * Handles Usage Fault exception.
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

/**
 * Handles Debug Monitor exception.
 */
void DebugMon_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * Handles EXTI0 global interrupt requests.
 */
void EXTI0_IRQHandler(void)
{
    PCF8583_ISR();
}

/**
 * Handles EXTI1 global interrupt requests.
 */
void EXTI1_IRQHandler(void)
{
    DCF77_ISR();
}

/**
 * Handles EXTI3 global interrupt requests.
 */
void EXTI3_IRQHandler(void)
{
    IR_ISR();
}

/**
 * Handles TIM3 global interrupt requests.
 */
void TIM2_IRQHandler(void)
{
    TIM2_ISR();
}

/**
 * Handles TIM3 global interrupt requests.
 */
void TIM3_IRQHandler(void)
{
    TIM3_ISR();
}
