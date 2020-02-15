/**
 ******************************************************************************
 * @file    max6955.c
 * @author  Phillip Durdaut
 * @date    2014-04-09
 * @brief   MAX6955 driver.
 * @hw      STM32F103RET6 @ Back to the Future Clock board v1.0
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* MAX6955 registers */
#define MAX6955_REGISTER_NOOP                       0x00
#define MAX6955_REGISTER_DECODE_MODE                0x01
#define MAX6955_REGISTER_GLOBAL_INTENSITY           0x02
#define MAX6955_REGISTER_SCAN_LIMIT                 0x03
#define MAX6955_REGISTER_CONFIGURATION              0x04
#define MAX6955_REGISTER_GPIO_DATA                  0x05
#define MAX6955_REGISTER_PORT_CONFIGURATION         0x06
#define MAX6955_REGISTER_DISPLAY_TEST               0x07
#define MAX6955_REGISTER_DISPLAY_KEY_A_DEBOUNCE     0x08
#define MAX6955_REGISTER_DISPLAY_KEY_B_DEBOUNCE     0x09
#define MAX6955_REGISTER_DISPLAY_KEY_C_DEBOUNCE     0x0A
#define MAX6955_REGISTER_DISPLAY_KEY_D_DEBOUNCE     0x0B
#define MAX6955_REGISTER_DISPLAY_KEY_A_PRESSED      0x0C
#define MAX6955_REGISTER_DISPLAY_KEY_B_PRESSED      0x0D
#define MAX6955_REGISTER_DISPLAY_KEY_C_PRESSED      0x0E
#define MAX6955_REGISTER_DISPLAY_KEY_D_PRESSED      0x0F
#define MAX6955_REGISTER_INTENSITY_10               0x10
#define MAX6955_REGISTER_INTENSITY_32               0x11
#define MAX6955_REGISTER_INTENSITY_54               0x12
#define MAX6955_REGISTER_INTENSITY_76               0x13
#define MAX6955_REGISTER_INTENSITY_10A              0x14
#define MAX6955_REGISTER_INTENSITY_32A              0x15
#define MAX6955_REGISTER_INTENSITY_54A              0x16
#define MAX6955_REGISTER_INTENSITY_76A              0x17

#define MAX6955_REGISTER_DIGIT_0_PLANE_P0           0x20
#define MAX6955_REGISTER_DIGIT_1_PLANE_P0           0x21
#define MAX6955_REGISTER_DIGIT_2_PLANE_P0           0x22
#define MAX6955_REGISTER_DIGIT_3_PLANE_P0           0x23
#define MAX6955_REGISTER_DIGIT_4_PLANE_P0           0x24
#define MAX6955_REGISTER_DIGIT_5_PLANE_P0           0x25
#define MAX6955_REGISTER_DIGIT_6_PLANE_P0           0x26
#define MAX6955_REGISTER_DIGIT_7_PLANE_P0           0x27
#define MAX6955_REGISTER_DIGIT_0A_PLANE_P0          0x28
#define MAX6955_REGISTER_DIGIT_1A_PLANE_P0          0x29
#define MAX6955_REGISTER_DIGIT_2A_PLANE_P0          0x2A
#define MAX6955_REGISTER_DIGIT_3A_PLANE_P0          0x2B
#define MAX6955_REGISTER_DIGIT_4A_PLANE_P0          0x2C
#define MAX6955_REGISTER_DIGIT_5A_PLANE_P0          0x2D
#define MAX6955_REGISTER_DIGIT_6A_PLANE_P0          0x2E
#define MAX6955_REGISTER_DIGIT_7A_PLANE_P0          0x2F

#define MAX6955_REGISTER_DIGIT_0_PLANE_P1           0x40
#define MAX6955_REGISTER_DIGIT_1_PLANE_P1           0x41
#define MAX6955_REGISTER_DIGIT_2_PLANE_P1           0x42
#define MAX6955_REGISTER_DIGIT_3_PLANE_P1           0x43
#define MAX6955_REGISTER_DIGIT_4_PLANE_P1           0x44
#define MAX6955_REGISTER_DIGIT_5_PLANE_P1           0x45
#define MAX6955_REGISTER_DIGIT_6_PLANE_P1           0x46
#define MAX6955_REGISTER_DIGIT_7_PLANE_P1           0x47
#define MAX6955_REGISTER_DIGIT_0A_PLANE_P1          0x48
#define MAX6955_REGISTER_DIGIT_1A_PLANE_P1          0x49
#define MAX6955_REGISTER_DIGIT_2A_PLANE_P1          0x4A
#define MAX6955_REGISTER_DIGIT_3A_PLANE_P1          0x4B
#define MAX6955_REGISTER_DIGIT_4A_PLANE_P1          0x4C
#define MAX6955_REGISTER_DIGIT_5A_PLANE_P1          0x4D
#define MAX6955_REGISTER_DIGIT_6A_PLANE_P1          0x4E
#define MAX6955_REGISTER_DIGIT_7A_PLANE_P1          0x4F

#define MAX6955_REGISTER_DIGIT_0_PLANE_P0_P1        0x60
#define MAX6955_REGISTER_DIGIT_1_PLANE_P0_P1        0x61
#define MAX6955_REGISTER_DIGIT_2_PLANE_P0_P1        0x62
#define MAX6955_REGISTER_DIGIT_3_PLANE_P0_P1        0x63
#define MAX6955_REGISTER_DIGIT_4_PLANE_P0_P1        0x64
#define MAX6955_REGISTER_DIGIT_5_PLANE_P0_P1        0x65
#define MAX6955_REGISTER_DIGIT_6_PLANE_P0_P1        0x66
#define MAX6955_REGISTER_DIGIT_7_PLANE_P0_P1        0x67
#define MAX6955_REGISTER_DIGIT_0A_PLANE_P0_P1       0x68
#define MAX6955_REGISTER_DIGIT_1A_PLANE_P0_P1       0x69
#define MAX6955_REGISTER_DIGIT_2A_PLANE_P0_P1       0x6A
#define MAX6955_REGISTER_DIGIT_3A_PLANE_P0_P1       0x6B
#define MAX6955_REGISTER_DIGIT_4A_PLANE_P0_P1       0x6C
#define MAX6955_REGISTER_DIGIT_5A_PLANE_P0_P1       0x6D
#define MAX6955_REGISTER_DIGIT_6A_PLANE_P0_P1       0x6E
#define MAX6955_REGISTER_DIGIT_7A_PLANE_P0_P1       0x6F

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void MAX6955_Clear(void);
static uint8_t MAX6955_GetDigitRegister(MAX6955_DEVICE_t p_Device, uint8_t p_Position, MAX6955_PLANE_t p_Plane);

/* Public Function implementations -------------------------------------------*/

/**
 * Initializes the MAX6955 driver ICs.
 */
void MAX6955_Init(void)
{
    MAX6955_Clear();

    /* Hexadecimal decode for all digits */
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_ALPHANUMERIC, MAX6955_REGISTER_DECODE_MODE, 0xFF);
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_NUMERIC_RED, MAX6955_REGISTER_DECODE_MODE, 0xFF);
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_NUMERIC_GREEN, MAX6955_REGISTER_DECODE_MODE, 0xFF);

    /* Intensity: 15/16 (max) */
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_ALPHANUMERIC, MAX6955_REGISTER_GLOBAL_INTENSITY, 0x0F);
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_NUMERIC_RED, MAX6955_REGISTER_GLOBAL_INTENSITY, 0x0F);
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_NUMERIC_GREEN, MAX6955_REGISTER_GLOBAL_INTENSITY, 0x0F);

    /* Enable normal operation */
    MAX6955_Enable(MAX6955_DEVICE_ALPHANUMERIC);
    MAX6955_Enable(MAX6955_DEVICE_NUMERIC_RED);
    MAX6955_Enable(MAX6955_DEVICE_NUMERIC_GREEN);
}

/**
 * Enables the given MAX6955.
 */
void MAX6955_Enable(MAX6955_DEVICE_t p_Device)
{
    uint8_t confReg = 0x00;
    I2Cx_ReadBuffer(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, &confReg, 1);
    I2Cx_WriteByte(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, confReg | 0x01);
}

/**
 * Disables the given MAX6955.
 */
void MAX6955_Disable(MAX6955_DEVICE_t p_Device)
{
    uint8_t confReg = 0x00;
    I2Cx_ReadBuffer(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, &confReg, 1);
    I2Cx_WriteByte(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, confReg & ~0x01);
}

/**
 * Sets a character (0-9 and A-Z) at the given position.
 */
void MAX6955_SetChar(MAX6955_DEVICE_t p_Device, uint8_t p_Position, uint8_t p_Char, MAX6955_PLANE_t p_Plane)
{
    I2Cx_WriteByte(MAX6955_I2CBUS, p_Device, MAX6955_GetDigitRegister(p_Device, p_Position, p_Plane), p_Char);
}

/**
 * Gets the current character (0-9 and A-Z) at the given position.
 */
uint8_t MAX6955_GetChar(MAX6955_DEVICE_t p_Device, uint8_t p_Position, MAX6955_PLANE_t p_Plane)
{
    uint8_t charInRegister = 0;
    I2Cx_ReadBuffer(MAX6955_I2CBUS, p_Device, MAX6955_GetDigitRegister(p_Device, p_Position, p_Plane), &charInRegister, 1);
    return charInRegister;
}

/**
 * Enables blinking with the given speed for the given MAX6955.
 */
void MAX6955_EnableBlinking(MAX6955_DEVICE_t p_Device, MAX6955_BLINKFREQ_t p_BlinkFreq)
{
    uint8_t confReg = 0x00;
    I2Cx_ReadBuffer(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, &confReg, 1);

    if (p_BlinkFreq == MAX6955_BLINKFREQ_SLOW)
        I2Cx_WriteByte(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, (confReg | 0x08) & ~0x04);

    if (p_BlinkFreq == MAX6955_BLINKFREQ_FAST)
        I2Cx_WriteByte(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, (confReg | 0x08) | 0x04);
}

/**
 * Disables blinking for the given MAX6955.
 */
void MAX6955_DisableBlinking(MAX6955_DEVICE_t p_Device)
{
    uint8_t confReg = 0x00;
    I2Cx_ReadBuffer(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, &confReg, 1);
    I2Cx_WriteByte(MAX6955_I2CBUS, p_Device, MAX6955_REGISTER_CONFIGURATION, confReg & ~0x08);
}

/* Private Function implementations ------------------------------------------*/

/**
 * Clears all MAX6955 driver ICs.
 */
static void MAX6955_Clear(void)
{
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_ALPHANUMERIC, MAX6955_REGISTER_CONFIGURATION, 0b00100000);
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_NUMERIC_RED, MAX6955_REGISTER_CONFIGURATION, 0b00100000);
    I2Cx_WriteByte(MAX6955_I2CBUS, MAX6955_DEVICE_NUMERIC_GREEN, MAX6955_REGISTER_CONFIGURATION, 0b00100000);
}

/**
 * Returns the corresponding digit register for the given MAX6955 and the given position.
 */
static uint8_t MAX6955_GetDigitRegister(MAX6955_DEVICE_t p_Device, uint8_t p_Position, MAX6955_PLANE_t p_Plane)
{
    uint8_t reg = 0;

    switch (p_Plane) {

        case MAX6955_PLANE_0:
            if (p_Device == MAX6955_DEVICE_ALPHANUMERIC) {

                switch (p_Position) {
                    case 0: reg = MAX6955_REGISTER_DIGIT_0_PLANE_P0;        break;
                    case 1: reg = MAX6955_REGISTER_DIGIT_1_PLANE_P0;        break;
                    case 2: reg = MAX6955_REGISTER_DIGIT_2_PLANE_P0;        break;
                    case 3: reg = MAX6955_REGISTER_DIGIT_3_PLANE_P0;        break;
                    case 4: reg = MAX6955_REGISTER_DIGIT_4_PLANE_P0;        break;
                    case 5: reg = MAX6955_REGISTER_DIGIT_5_PLANE_P0;        break;
                }
            }
            else {

                switch (p_Position) {
                    case 0: reg = MAX6955_REGISTER_DIGIT_0_PLANE_P0;        break;
                    case 1: reg = MAX6955_REGISTER_DIGIT_0A_PLANE_P0;       break;
                    case 2: reg = MAX6955_REGISTER_DIGIT_1_PLANE_P0;        break;
                    case 3: reg = MAX6955_REGISTER_DIGIT_1A_PLANE_P0;       break;
                    case 4: reg = MAX6955_REGISTER_DIGIT_2_PLANE_P0;        break;
                    case 5: reg = MAX6955_REGISTER_DIGIT_2A_PLANE_P0;       break;
                    case 6: reg = MAX6955_REGISTER_DIGIT_3_PLANE_P0;        break;
                    case 7: reg = MAX6955_REGISTER_DIGIT_3A_PLANE_P0;       break;
                    case 8: reg = MAX6955_REGISTER_DIGIT_4_PLANE_P0;        break;
                    case 9: reg = MAX6955_REGISTER_DIGIT_4A_PLANE_P0;       break;
                }
            }
            break;

        case MAX6955_PLANE_1:
            if (p_Device == MAX6955_DEVICE_ALPHANUMERIC) {

                switch (p_Position) {
                    case 0: reg = MAX6955_REGISTER_DIGIT_0_PLANE_P1;        break;
                    case 1: reg = MAX6955_REGISTER_DIGIT_1_PLANE_P1;        break;
                    case 2: reg = MAX6955_REGISTER_DIGIT_2_PLANE_P1;        break;
                    case 3: reg = MAX6955_REGISTER_DIGIT_3_PLANE_P1;        break;
                    case 4: reg = MAX6955_REGISTER_DIGIT_4_PLANE_P1;        break;
                    case 5: reg = MAX6955_REGISTER_DIGIT_5_PLANE_P1;        break;
                }
            }
            else {

                switch (p_Position) {
                    case 0: reg = MAX6955_REGISTER_DIGIT_0_PLANE_P1;        break;
                    case 1: reg = MAX6955_REGISTER_DIGIT_0A_PLANE_P1;       break;
                    case 2: reg = MAX6955_REGISTER_DIGIT_1_PLANE_P1;        break;
                    case 3: reg = MAX6955_REGISTER_DIGIT_1A_PLANE_P1;       break;
                    case 4: reg = MAX6955_REGISTER_DIGIT_2_PLANE_P1;        break;
                    case 5: reg = MAX6955_REGISTER_DIGIT_2A_PLANE_P1;       break;
                    case 6: reg = MAX6955_REGISTER_DIGIT_3_PLANE_P1;        break;
                    case 7: reg = MAX6955_REGISTER_DIGIT_3A_PLANE_P1;       break;
                    case 8: reg = MAX6955_REGISTER_DIGIT_4_PLANE_P1;        break;
                    case 9: reg = MAX6955_REGISTER_DIGIT_4A_PLANE_P1;       break;
                }
            }
            break;

        case MAX6955_PLANE_BOTH:
            if (p_Device == MAX6955_DEVICE_ALPHANUMERIC) {

                switch (p_Position) {
                    case 0: reg = MAX6955_REGISTER_DIGIT_0_PLANE_P0_P1;     break;
                    case 1: reg = MAX6955_REGISTER_DIGIT_1_PLANE_P0_P1;     break;
                    case 2: reg = MAX6955_REGISTER_DIGIT_2_PLANE_P0_P1;     break;
                    case 3: reg = MAX6955_REGISTER_DIGIT_3_PLANE_P0_P1;     break;
                    case 4: reg = MAX6955_REGISTER_DIGIT_4_PLANE_P0_P1;     break;
                    case 5: reg = MAX6955_REGISTER_DIGIT_5_PLANE_P0_P1;     break;
                }
            }
            else {

                switch (p_Position) {
                    case 0: reg = MAX6955_REGISTER_DIGIT_0_PLANE_P0_P1;     break;
                    case 1: reg = MAX6955_REGISTER_DIGIT_0A_PLANE_P0_P1;    break;
                    case 2: reg = MAX6955_REGISTER_DIGIT_1_PLANE_P0_P1;     break;
                    case 3: reg = MAX6955_REGISTER_DIGIT_1A_PLANE_P0_P1;    break;
                    case 4: reg = MAX6955_REGISTER_DIGIT_2_PLANE_P0_P1;     break;
                    case 5: reg = MAX6955_REGISTER_DIGIT_2A_PLANE_P0_P1;    break;
                    case 6: reg = MAX6955_REGISTER_DIGIT_3_PLANE_P0_P1;     break;
                    case 7: reg = MAX6955_REGISTER_DIGIT_3A_PLANE_P0_P1;    break;
                    case 8: reg = MAX6955_REGISTER_DIGIT_4_PLANE_P0_P1;     break;
                    case 9: reg = MAX6955_REGISTER_DIGIT_4A_PLANE_P0_P1;    break;
                }
            }
            break;
    }

    return reg;
}
