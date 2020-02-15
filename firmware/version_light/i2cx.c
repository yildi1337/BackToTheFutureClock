/**
 ******************************************************************************
 * @file    i2cx.c
 * @author  Phillip Durdaut
 * @date    2014-04-10
 * @brief   I2Cx control.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define I2Cx_WaitEvent(x) timeout = 0xFFFF; while (x) { if (timeout-- == 0) break;}

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Public Function implementations -------------------------------------------*/

/**
 * Initializes I2Cx.
 */
void I2Cx_Init(I2C_TypeDef * p_pI2Cx)
{
    I2C_InitTypeDef I2C_InitStructure;

    if (p_pI2Cx == I2C1) {

        /* I2C1 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

        /* I2C1 reset */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

        /* I2C1 configuration */
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
        I2C_InitStructure.I2C_OwnAddress1 = I2C1_OWN_ADDRESS;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed = I2C1_BUS_SPEED;
    }
    else if (p_pI2Cx == I2C2) {

        /* I2C2 clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

        /* I2C2 reset */
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
        RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

        /* I2C2 configuration */
        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
        I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
        I2C_InitStructure.I2C_OwnAddress1 = I2C2_OWN_ADDRESS;
        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_InitStructure.I2C_ClockSpeed = I2C2_BUS_SPEED;
    }

    /* I2Cx configuration */
    I2C_Init(p_pI2Cx, &I2C_InitStructure);

    /* I2Cx enable */
    I2C_Cmd(p_pI2Cx, ENABLE);
}

/**
 * Executes the given command byte on the slave.
 */
void I2Cx_WriteCommand(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Command)
{
    volatile uint32_t timeout = 0xFFFF;

    /* Wait while busy */
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BUSY));

    /* Intiate start sequence */
    I2C_GenerateSTART(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send address and direction */
    I2C_Send7bitAddress(p_pI2Cx, p_SlaveAddress, I2C_Direction_Transmitter);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Write one byte */
    I2C_SendData(p_pI2Cx, p_Command);
    I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));

    /* Finish transmission */
    I2C_GenerateSTOP(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_STOPF));
}

/**
 * Writes the given data byte to the given address at the slave.
 */
void I2Cx_WriteByte(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Address, uint8_t p_Data)
{
    volatile uint32_t timeout = 0xFFFF;

    /* Wait while busy */
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BUSY));

    /* Intiate start sequence */
    I2C_GenerateSTART(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send address and direction */
    I2C_Send7bitAddress(p_pI2Cx, p_SlaveAddress, I2C_Direction_Transmitter);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Write address byte */
    I2C_SendData(p_pI2Cx, p_Address);
    I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));

    /* Write data byte */
    I2C_SendData(p_pI2Cx, p_Data);
    I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));

    /* Finish transmission */
    I2C_GenerateSTOP(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_STOPF));
}

/**
 * Sends the specified number of bytes starting at the given address to the slave.
 */
void I2Cx_WriteBuffer(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Address, uint8_t * p_pBuffer, uint8_t p_NumOfBytes)
{
    volatile uint32_t timeout = 0xFFFF;
    uint8_t i = 0;

    /* Wait while busy */
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BUSY));

    /* Intiate start sequence */
    I2C_GenerateSTART(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send address and direction */
    I2C_Send7bitAddress(p_pI2Cx, p_SlaveAddress, I2C_Direction_Transmitter);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Write the data bytes from the buffer */
    for (i = 0; i < p_NumOfBytes; i++) {
        I2C_SendData(p_pI2Cx, p_pBuffer[i]);
        I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));
    }

    /* Finish transmission */
    I2C_GenerateSTOP(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_STOPF));
}

/**
 * Reads one data byte from the given address of the slave to the given buffer.
 */
void I2Cx_ReadBuffer(I2C_TypeDef * p_pI2Cx, uint8_t p_SlaveAddress, uint8_t p_Address, uint8_t * p_pBuffer, uint8_t p_NumOfBytes)
{
    volatile uint32_t timeout = 0xFFFF;

    /* Wait while busy */
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BUSY));

    /* Intiate start sequence */
    I2C_GenerateSTART(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send address and direction */
    I2C_Send7bitAddress(p_pI2Cx, p_SlaveAddress, I2C_Direction_Transmitter);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Write address byte */
    I2C_SendData(p_pI2Cx, p_Address);
    I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));

    /* Enable acknowledgement, clear POS flag */
    I2C_AcknowledgeConfig(p_pI2Cx, ENABLE);
    I2C_NACKPositionConfig(p_pI2Cx, I2C_NACKPosition_Current);

    /* Intiate start sequence */
    I2C_GenerateSTART(p_pI2Cx, ENABLE);
    I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send address and direction */
    I2C_Send7bitAddress(p_pI2Cx, p_SlaveAddress, I2C_Direction_Receiver);
    I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_ADDR));

    if (p_NumOfBytes == 1) {

        /* Clear ACK bit */
        I2C_AcknowledgeConfig(p_pI2Cx, DISABLE);

        /* Clear ADDR and generate STOP -> must be atomic */
        __disable_irq();
        (void)p_pI2Cx->SR2;
        I2C_GenerateSTOP(p_pI2Cx, ENABLE);
        __enable_irq();

        /* Receive data */
        I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_RXNE));
        *p_pBuffer++ = I2C_ReceiveData(p_pI2Cx);
    }
    else if (p_NumOfBytes == 2) {

        /* Set POS flag */
        I2C_NACKPositionConfig(p_pI2Cx, I2C_NACKPosition_Next);

        /* Clear ADDR flag and ACK bit -> must be atomic and in this order */
        __disable_irq();
        (void)p_pI2Cx->SR2;
        I2C_AcknowledgeConfig(p_pI2Cx, DISABLE);
        __enable_irq();

        /* Wait for byte received */
        I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));
        __disable_irq();
        I2C_GenerateSTOP(p_pI2Cx, ENABLE);
        *p_pBuffer++ = p_pI2Cx->DR;
        __enable_irq();
        *p_pBuffer++ = p_pI2Cx->DR;
    }
    else {

        /* Clear ADDR flag */
        (void)p_pI2Cx->SR2;
        while (p_NumOfBytes-- != 3) {

            /* Wait for BTF instead of RXNE */
            I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));
            *p_pBuffer++ = I2C_ReceiveData(p_pI2Cx);
        }
        I2Cx_WaitEvent(!I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_BTF));

        /* Clear ACK bit */
        I2C_AcknowledgeConfig(p_pI2Cx, DISABLE);

        /* Receive byte N-2 and stop */
        __disable_irq();
        *p_pBuffer++ = I2C_ReceiveData(p_pI2Cx);
        I2C_GenerateSTOP(p_pI2Cx,ENABLE);
        __enable_irq();

        /* Receive byte N-1 and stop */
        *p_pBuffer++ = I2C_ReceiveData(p_pI2Cx);

        /* wait for byte N */
        I2Cx_WaitEvent(!I2C_CheckEvent(p_pI2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *p_pBuffer++ = I2C_ReceiveData(p_pI2Cx);

        p_NumOfBytes = 0;
    }

    /* Wait for stop */
    I2Cx_WaitEvent(I2C_GetFlagStatus(p_pI2Cx, I2C_FLAG_STOPF));
}
