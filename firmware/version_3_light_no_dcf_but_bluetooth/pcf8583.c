/**
 ******************************************************************************
 * @file    pcf8583.c
 * @author  Phillip Durdaut
 * @date    2014-04-11
 * @brief   PCF8583 driver.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* PCF8583 registers */
#define PCF8583_REGISTER_CONTROL_STATUS      0x00
#define PCF8583_REGISTER_HUNDRETH_SECONDS    0x01
#define PCF8583_REGISTER_SECONDS             0x02
#define PCF8583_REGISTER_MINUTES             0x03
#define PCF8583_REGISTER_HOURS               0x04
#define PCF8583_REGISTER_YEAR_DATE           0x05
#define PCF8583_REGISTER_WEEKDAY_MONTH       0x06

/* Use the first free RAM register for the year byte */
#define PCF8583_REGISTER_RAM_YEAR            0x10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t m_InterruptEnabled = 0;

/* Private function prototypes -----------------------------------------------*/
static uint8_t PCF8583_BCD2BIN(uint8_t p_BCD);
static uint8_t PCF8583_BIN2BCD(uint8_t p_BIN);

/* Public Function implementations -------------------------------------------*/

/**
 * Initializes the PCF8583 RTC.
 */
void PCF8583_Init(void)
{
    /* Start counting */
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_CONTROL_STATUS, 0x00);

    /* Enable RTC interrupt */
    PCF8583_InterruptEnable();
}

/**
 * Enables the RTC interrupt on falling edge.
 */
void PCF8583_InterruptEnable(void)
{
    /* PPM input signal pin can trigger an interrupt */
    EXTI_InitTypeDef  EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = PIN_RTC_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable Interrupt */
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = PIN_RTC_INT_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    m_InterruptEnabled = 1;
}

/**
 * Disables the RTC interrupt.
 */
void PCF8583_InterruptDisable(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = PIN_RTC_INT_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    m_InterruptEnabled = 0;
}

/**
 * Interrupt service routine.
 */
void PCF8583_ISR(void)
{
    if (EXTI_GetITStatus(PIN_RTC_INT_EXTI_LINE) != RESET) {
        EXTI_ClearITPendingBit(PIN_RTC_INT_EXTI_LINE);

        DATETIME_PCF8583_t dateTimeFromRTC = { 0, 0, 0, 0, 0, 0, 0 };
        PCF8583_GetDateTime(&dateTimeFromRTC);
        DATETIME_DISPLAY_t dateTimeForDisplay = DATETIME_ConvertPCF8583ToDisplay(&dateTimeFromRTC);
        DISPLAY_SetDateTime(DISPLAY_LINE_PRESENT, &dateTimeForDisplay);

        LED_Toggle(LED_SECONDS);
    }
}

/**
 * Gets the date/time from the PCF8583.
 */
void PCF8583_GetDateTime(DATETIME_PCF8583_t * p_pDateTime)
{
    uint8_t readBuf[5] = { 0 };
    uint8_t BCDSeconds;
    uint8_t BCDMinutes;
    uint8_t BCDHours;
    uint8_t BCDYearDate;
    uint8_t BCDWeekdayMonth;
    uint8_t RAMYear;
    uint8_t BITYear;
    uint8_t interruptEnabledBefore = m_InterruptEnabled;

    if (m_InterruptEnabled)
        PCF8583_InterruptDisable();

    /* Read from RTC */
    I2Cx_ReadBuffer(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_SECONDS, readBuf, 5);
    BCDSeconds = readBuf[0];
    BCDMinutes = readBuf[1];
    BCDHours = readBuf[2];
    BCDYearDate = readBuf[3];
    BCDWeekdayMonth = readBuf[4];

    /* Convert the masked values from BCD to binary */
    p_pDateTime->second = PCF8583_BCD2BIN(BCDSeconds);
    p_pDateTime->minute = PCF8583_BCD2BIN(BCDMinutes);
    p_pDateTime->hour = PCF8583_BCD2BIN(BCDHours & 0x3F);
    p_pDateTime->day = PCF8583_BCD2BIN(BCDYearDate & 0x3F);
    p_pDateTime->month = PCF8583_BCD2BIN(BCDWeekdayMonth & 0x1F);
    p_pDateTime->weekday = BCDWeekdayMonth >> 5;
    BITYear = BCDYearDate >> 6;

    /* Read year from RAM */
    I2Cx_ReadBuffer(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_RAM_YEAR, &RAMYear, 1);

    /* Check whether the two "year bits" were incremented by the RTC. */
    /* If so, increment the 8-bit year byte (read from NVRAM) */
    if (BITYear != 0)
		RAMYear++;
    p_pDateTime->year = RAMYear;

    /* Update year in RTC RAM */
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_RAM_YEAR, RAMYear);

    if (interruptEnabledBefore)
        PCF8583_InterruptEnable();
}

/**
 * Sets the given date/time to the PCF8583.
 */
void PCF8583_SetDateTime(DATETIME_PCF8583_t * p_pDateTime)
{
    uint8_t BCDSeconds;
    uint8_t BCDMinutes;
    uint8_t BCDHours;
    uint8_t BCDYearDate;
    uint8_t BCDWeekdayMonth;
    uint8_t interruptEnabledBefore = m_InterruptEnabled;

    /* Convert the input date/time into bcd values that are formatted for the rtc registers */
    BCDSeconds = PCF8583_BIN2BCD(p_pDateTime->second);
    BCDMinutes = PCF8583_BIN2BCD(p_pDateTime->minute);
    BCDHours = PCF8583_BIN2BCD(p_pDateTime->hour);
    BCDYearDate = PCF8583_BIN2BCD(p_pDateTime->day) | (0 << 6);
    BCDWeekdayMonth = PCF8583_BIN2BCD(p_pDateTime->month) | (p_pDateTime->weekday << 5);

    if (m_InterruptEnabled)
        PCF8583_InterruptDisable();

    /* Stop the rtc from counting, before writing to the registers */
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_CONTROL_STATUS, 0x80);

    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_SECONDS, BCDSeconds);
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_MINUTES, BCDMinutes);
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_HOURS, BCDHours);
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_YEAR_DATE, BCDYearDate);
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_WEEKDAY_MONTH, BCDWeekdayMonth);

    /* Write the year byte to RTC RAM */
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_RAM_YEAR, p_pDateTime->year);

    /* Enable counting again */
    I2Cx_WriteByte(PCF8583_I2CBUS, PCF8583_I2C_ADDRESS, PCF8583_REGISTER_CONTROL_STATUS, 0x00);

    if (interruptEnabledBefore)
        PCF8583_InterruptEnable();
}

/* Private Function implementations ------------------------------------------*/

/**
 * Converts the given BCD number to binary number.
 */
static uint8_t PCF8583_BCD2BIN(uint8_t p_BCD)
{
    uint8_t temp = p_BCD;

    /* Shifting the upper digit right by 1 is the same as multiplying it by 8 */
    temp >>= 1;

    /* Isolate the bits for the upper digit */
    temp &= 0x78;

    /* Now return: (tenth * 8) + (tenth * 2) + ones */
    return (temp + (temp >> 2) + (p_BCD & 0x0F));
}

/**
 * Converts the given binary number to BCD.
 */
static uint8_t PCF8583_BIN2BCD(uint8_t p_BIN)
{
    uint8_t bcd = 0;

    while(1) {

        /* Get the tenth digit by doing multiple subtraction 10 from the binary value */
        if (p_BIN >= 10) {
            p_BIN -= 10;
            bcd += 0x10;
        }

        /* Get the ones digit by adding the remainder */
        else {
            bcd += p_BIN;
            break;
        }
    }

    return bcd;
}
