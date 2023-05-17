/**
 ******************************************************************************
 * @file    usart1.c
 * @author  Phillip Durdaut
 * @date    2023-05-17
 * @brief   USART1 controlling.
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
 * Initialises the USART1 with 8N1 @ 9600 Baud.
 */
void USART1_Init(uint32_t p_Baudrate, uint16_t p_Mode)
{
    USART_InitTypeDef USART_InitStructure;

    /* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* 8N1 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;

    /* 115200 Baud */
    USART_InitStructure.USART_BaudRate = p_Baudrate;

    /* No hardware flow control */
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

    /* Enable transmission */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

/**
 * Writes a string to the UART.
 */
void USART1_WriteString(const char * const p_String)
{
    uint32_t i;

    for (i = 0; i < strlen(p_String); i++) {

        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, p_String[i]);
    }
}

/**
 * Reads bytes from the UART until timeout reached.
 */
uint8_t USART1_ReadBytes(uint8_t * p_Buffer, uint32_t p_Timeout)
{
    uint32_t timeoutCnt = 0;
    uint8_t i = 0;

    while (1) {

        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {

            p_Buffer[i] = (uint8_t)USART_ReceiveData(USART1);
            i++;
            timeoutCnt = 0;
        }
        else {

            timeoutCnt++;
            if (timeoutCnt == p_Timeout)
                return i;
            else
                DELAY_ms(1);
        }
    }
}
