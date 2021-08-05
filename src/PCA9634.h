/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef PCA9634_H
#define PCA9634_H

#include <Wire.h>

#include "PCA963x.h"

/**
 * Brightness control LED4
 */
#define REG_PWM4        0x06

/**
 * Brightness control LED5
 */
#define REG_PWM5        0x07

/**
 * Brightness control LED6
 */
#define REG_PWM6        0x08

/**
 * Brightness control LED7
 */
#define REG_PWM7        0x09

/**
 * Group duty cycle control
 */
#define REG_GRPPWM_PCA9634  0x0A

/**
 * Group frequency
 */
#define REG_GRPFREQ_PCA9634  0x0B

/**
 * LED output state
 */
#define REG_LEDOUT1_PCA9634 0x0C

/**
 * LED output state
 */
#define REG_LEDOUT2_PCA9634 0x0D

/**
 * I2C-bus subaddress 1
 */
#define REG_SUBADR1_PCA9634 0x0E

/**
 * I2C-bus subaddress 2
 */
#define REG_SUBADR2_PCA9634 0x0F

/**
 * I2C-bus subaddress 3
 */
#define REG_SUBADR3_PCA9634 0x10

/**
 * LED All Call I2C-bus address
 */
#define REG_ALLCALLADR_PCA9634 0x11

class PCA9634 : public PCA963x {

public:

    PCA9634();

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
    * Set the LED driver output state for a given channel (0 to 3). There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state  One of the four possible states
    * @param ldrBit Lower bit of LDR* (see BIT_LDR*)
    */
    void setLdr1State(uint8_t state, uint8_t ldrBit);

    /**
    * Set the LED driver output state for a given channel (4 to 7). There are four states:
    *   - LDR_STATE_OFF
    *   - LDR_STATE_ON
    *   - LDR_STATE_IND
    *   - LDR_STATE_IND_GRP
    *
    * @param state  One of the four possible states
    * @param ldrBit Lower bit of LDR* (see BIT_LDR*)
    */
    void setLdr2State(uint8_t state, uint8_t ldrBit);

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
      * Stored register content of LEDOUT when writing LDR_STATE_OFF to all LDRs
      * when calling turnOff().
      */
     uint8_t _storedRegLedout[2];
};

#endif //PCA9634_H
