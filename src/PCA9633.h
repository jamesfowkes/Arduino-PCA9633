/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef PCA9633_H
#define PCA9633_H

#include <Wire.h>

// Register definitions (page 11, table 7)

/**
 * Mode register 1
 */
#define REG_MODE1       0x00

/**
 * Mode register 2
 */
#define REG_MODE2       0x01

/**
 * Brightness control LED0
 */
#define REG_PWM0        0x02

/**
 * Brightness control LED1
 */
#define REG_PWM1        0x03

/**
 * Brightness control LED2
 */
#define REG_PWM2        0x04

/**
 * Brightness control LED3
 */
#define REG_PWM3        0x05

/**
 * Group duty cycle control
 */
#define REG_GRPPWM      0x06

/**
 * Group frequency
 */
#define REG_GRPFREQ     0x07

/**
 * LED output state
 */
#define REG_LEDOUT      0x08

/**
 * I2C-bus subaddress 1
 */
#define REG_SUBADR1     0x09

/**
 * I2C-bus subaddress 2
 */
#define REG_SUBADR2     0x0A

/**
 * I2C-bus subaddress 3
 */
#define REG_SUBADR3     0x0B

/**
 * LED All Call I2C-bus address
 */
#define REG_ALLCALLADR  0x0C



// Bits in REG_MODE1 (page 12, table 8)

/**
 * Bit for register Auto-Increment
 * 0 = disabled
 * 1 = enabled
 */
#define BIT_AI2     7

/**
 * Bit for Auto-Increment bit1
 */
#define BIT_AI1     6

/**
 * Bit for Auto-Increment bit0
 */
#define BIT_AI0     5

/**
 * 0 = Normal mode
 * 1 = Low power mode. Oscillator off
 */
#define BIT_SLEEP   4

/**
 * 0 = PCA9633 does not respond to I2C-bus subaddress 1
 * 1 = PCA9633 responds to I2C-bus subaddress 1
 */
#define BIT_SUB1    3

/**
 * 0 = PCA9633 does not respond to I2C-bus subaddress 2
 * 1 = PCA9633 responds to I2C-bus subaddress 2
 */
#define BIT_SUB2    2

/**
 * 0 = PCA9633 does not respond to I2C-bus subaddress 3
 * 1 = PCA9633 responds to I2C-bus subaddress 3
 */
#define BIT_SUB3    1

/**
 * 0 = PCA9633 does not respond to LED All Call I2C-bus address
 * 1 = PCA9633 responds to LED All Call I2C-bus address
 */
#define BIT_ALLCALL 0



// Bits in REG_MODE2 (page 12-13, table 9)

/**
 * Bit for group control; 0=dimming, 1=blinking
 */
#define BIT_DMBLNK  5

/**
 * 0 = Output logic state not inverted. Value to use when no external driver used
 * 1 = Output logic state inverted. Value to use when external driver used
 */
#define BIT_INVRT   4

/**
 * 0 = Outputs change on STOP command
 * 1 = Outputs change on ACK
 */
#define BIT_OCH     3

/**
 * 0 = The 4 LED outputs are configured with an open-drain structure
 * 1 = The 4 LED outputs are configured with a totem pole structure
 */
#define BIT_OUTDRV  2

/**
 * See BIT_OUTNE0
 */
#define BIT_OUTNE1  1

/**
 *  00 = When OE = 1 (output drivers not enabled), LEDn = 0.
 *  01* = When OE = 1 (output drivers not enabled):
 *      LEDn = 1 when OUTDRV = 1
 *      LEDn = high-impedance when OUTDRV = 0 (same as OUTNE[1:0] = 10)
 *  10 When OE = 1 (output drivers not enabled), LEDn = high-impedance.
 *  11 reserved
 */
#define BIT_OUTNE0  0



// Bits in REG_LEDOUT (page 14, table 13)

/**
 * Lower of two bits for LDR3
 */
#define BIT_LDR3    6

/**
 * Lower of two bits for LDR2
 */
#define BIT_LDR2    4

/**
 * Lower of two bits for LDR1
 */
#define BIT_LDR1    2

/**
 * Lower of two bits for LDR0
 */
#define BIT_LDR0    0



// LED driver output state, LEDOUT (page 14, below table 13)

/**
 * LED driver x is off
 */
#define LDR_STATE_OFF       0x00

/**
 * LED driver x is fully on (individual brightness and group dimming/ blinking
 * not controlled)
 */
#define LDR_STATE_ON        0x01

/**
 * LED driver x individual brightness can be controlled through its
 * PWMx register
 */
#define LDR_STATE_IND       0x02

/**
 * LED driver x individual brightness and group dimming/ blinking can be
 * controlled through its PWMx register and the GRPPWM registers. If using
 * LDR_STATE_IND_GRP the controller takes the minimum value of PWM* and
 * GRPPWM register
 */
#define LDR_STATE_IND_GRP   0x03



// Auto-Increment options (page 10, table 6)

/**
 * No Auto-Increment
 */
#define AI_DISABLED 0

/**
 * Auto-Increment for all registers. D3, D2, D1, D0 roll over to ‘0000’ after
 * the last register (1100) is accessed.
 */
#define AI_ALL      1

/**
 * Auto-Increment for individual brightness registers only. D3, D2, D1, D0
 * roll over to ‘0010’ after the last register (0101) is accessed.
 */
#define AI_IND      2

/**
 * Auto-Increment for global control registers only. D3, D2, D1, D0 roll over
 * to ‘0110’ after the last register (0111) is accessed.
 */
#define AI_GBL      3

/**
 * Auto-Increment for individual and global control registers only. D3, D2,
 * D1, D0 roll over to ‘0010’ after the last register (0111) is accessed.
 */
#define AI_IND_GBL  4



/**
 * Control mode for blinking
 */
#define GROUP_CONTROL_MODE_BLINKING 0

/**
 * Control mode for dimming
 */
#define GROUP_CONTROL_MODE_DIMMING  1



// Frequency of 24 Hz is used
#define BLINKING_PERIOD_125_MS  3   // ((1 / 24 Hz) * 3 cycles)
#define BLINKING_PERIOD_250_MS  6   // ((1 / 24 Hz) * 6 cycles)
#define BLINKING_PERIOD_500_MS  12  // ((1 / 24 Hz) * 12 cycles)
#define BLINKING_PERIOD_1_S     24  // ((1 / 24 Hz) * 24 cycles)
#define BLINKING_PERIOD_MAX     255 // 10.73 s

#define BLINKING_RATIO_BALANCED 0.5

class PCA9633 {

public:

    /**
     * Constructor for PCA9633 with RGB.
     *
     * @param regRedPwm     Register address for red color channel
     * @param regGreenPwm   Register address for green color channel
     * @param regBluePwm    Register address for blue color channel
     */
    PCA9633(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm);

    /**
     * Constructor for PCA9633 with RGBW.
     *
     * @param regRedPwm     Register address for red color channel
     * @param regGreenPwm   Register address for green color channel
     * @param regBluePwm    Register address for blue color channel
     * @param regWhitePwm   Register address for white color channel
     */
    PCA9633(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm,
            uint8_t regWhitePwm);

    /**
     * Initialization.
     *
     * @param deviceAddress I2C address of the pca9633
     * @param wire          Reference to TwoWire for I2C communication
     */
    void begin(uint8_t deviceAddress, TwoWire *wire);

    /**
     * Turn on all LEDs. Restores settings saved at turnOff().
     *
     * WARNING: If you call turnOff() twice, without calling turnOn() in between,
     *          then the restored state will be LDR_STATE_OFF!
     */
    void turnOn();

    /**
     * Turn off all LEDs. Saves current settings for turnOn().
     * For power saving, see sleep().
     *
     * WARNING: If you call turnOff() twice, without calling turnOn() in between,
     *          then the restored state will be LDR_STATE_OFF!
     */
    void turnOff();

    /**
     * Switch to normal mode.
     */
    void wakeUp();

    /**
     * Switch to low power mode.
     */
    void sleep();

    /**
     * Set individual PWM signal for a given channel.
     *
     * @param regPwm    Register address for PWM channel
     * @param pwm       PWM value
     */
    void setPwm(uint8_t regPwm, uint8_t pwm);

    /**
     * Set global PWM signal.
     *
     * @param pwm   PWM value
     */
    void setGrpPwm(uint8_t pwm);

    /**
     * Set up values for blinking mode. Blinking mode needs to be activated
     * manually by calling setGroupControlMode(GROUP_CONTROL_MODE_BLINKING).
     *
     * @param blinkPeriod   Period for one blink (turning off and on)
     * @param onOffRatio    Value between 0.0 and 1.0, where e.g. a value of
     *                      0.25 means 1/4 of the time the LEDs are on and
     *                      3/4 of the time the LEDs are off
     */
    void setBlinking(uint8_t blinkPeriod, float onOffRatio);

    /**
    * Set PWM values for RGB.
    *
    * @param r  Value for red color channel
    * @param g  Value for green color channel
    * @param b  Value for blue color channel
    */
    void setRGB(uint8_t r, uint8_t g, uint8_t b);

    /**
    * Set PWM values for RGBW. Only available when PCA9633 object was created
    * with the RGBW constructor.
    *
    * @param r  Value for red color channel
    * @param g  Value for green color channel
    * @param b  Value for blue color channel
    * @param w  Value for white color channel
    */
    void setRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);

    /**
    * Set the LED driver output state for a given channel. There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state  One of the four possible states
    * @param ldrBit Lower bit of LDR* (see BIT_LDR*)
    */
    void setLdrState(uint8_t state, uint8_t ldrBit);

    /**
    * Set the LED driver output state for all channels. There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state  One of the four possible states
    */
    void setLdrStateAll(uint8_t state);

    /**
    * Set an option for auto increment. There are five options:
    *   - AI_DISABLED
    *   - AI_ALL
    *   - AI_IND
    *   - AI_GBL
    *   - AI_IND_GBL
    *
    * @param option One of the possible five options
    */
    void setAutoIncrement(uint8_t option);

    /**
    * Set the group control mode. There are two modes:
    *   - GROUP_CONTROL_MODE_BLINKING
    *   - GROUP_CONTROL_MODE_DIMMING
    *
    * @param mode   One of the two possible modes
    */
    void setGroupControlMode(uint8_t mode);


private:

    /**
    * Write data to a register.
    *
    * @param registerAddress    Register address to write to
    * @param data               Data to write
    */
    void writeReg(uint8_t registerAddress, uint8_t data);

    /**
    * Read data from a register.
    *
    * @param registerAddress    Register address to read from
     *
     * @return  byte read from given registerAddress
     * @return  -1 if no byte was available to be read
    */
    uint8_t readReg(uint8_t registerAddress);

    /**
     * I2C address of device.
     */
    uint8_t _deviceAddres;

    /**
     * Mapping of different PWM channels to colors.
     */
    uint8_t _regRedPwm, _regGreenPwm, _regBluePwm, _regWhitePwm;

    /**
     * Indicates whether PCA9633 was created with the RGBW constructor.
     */
     bool _hasWhiteChannel;

    /**
     * Stored register content of LEDOUT when writing LDR_STATE_OFF to all LDRs
     * when calling turnOff().
     */
    uint8_t _storedRegLedout;

    /**
     * Object for I2C communication.
     */
    TwoWire *_wire;
};

#endif //PCA9633_H
