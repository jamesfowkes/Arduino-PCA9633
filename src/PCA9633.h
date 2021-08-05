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

#include "PCA963x.h"

/**
 * Group duty cycle control
 */
#define REG_GRPPWM_PCA9633  0x6

/**
 * Group frequency
 */
#define REG_GRPFREQ_PCA9633  0x7

/**
 * LED output state
 */
#define REG_LEDOUT  0x8

/**
 * I2C-bus subaddress 1
 */
#define REG_SUBADR1 0x9

/**
 * I2C-bus subaddress 2
 */
#define REG_SUBADR2 0x0A

/**
 * I2C-bus subaddress 3
 */
#define REG_SUBADR3 0x0B

/**
 * LED All Call I2C-bus address
 */
 #define REG_ALLCALLADR 0x0C

class PCA9633 : public PCA963x {

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
     * Set PWM values for RGB.
     *
     * @param r  Value for red color channel
     * @param g  Value for green color channel
     * @param b  Value for blue color channel
    */
    void setRGB(uint8_t r, uint8_t g, uint8_t b);

    /**
     * Set PWM values for RGBW. Only available when PCA963x object was created
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
    * Set the LED driver output state for a given channel. There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param channel  Channel to set (0 to 3 for PCA9633, 0 to 7 for PCA9634)
    * @param state  Lower bit of LDR* (see BIT_LDR*)
    */
    void setChannelLdrState(uint8_t ch, uint8_t state);

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

private:
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

};

#endif //PCA9633_H
