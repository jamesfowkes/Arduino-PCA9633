/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Arduino.h"

#include "PCA9633.h"

/******************************* PUBLIC METHODS *******************************/
PCA9633::PCA9633(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm) : PCA963x(4, REG_GRPPWM_PCA9633, REG_SUBADR1) {

    _regRedPwm = regRedPwm;
    _regGreenPwm = regGreenPwm;
    _regBluePwm = regBluePwm;

    _hasWhiteChannel = false;
}

PCA9633::PCA9633(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm,
                 uint8_t regWhitePwm)
                 : PCA9633(regRedPwm, regGreenPwm, regBluePwm) {

    _regWhitePwm = regWhitePwm;

    _hasWhiteChannel = true;
}

void PCA9633::turnOn() {

    writeReg(REG_LEDOUT, _storedRegLedout);
}

void PCA9633::turnOff() {

    _storedRegLedout = readReg(REG_LEDOUT);
    writeReg(REG_LEDOUT, LDR_STATE_OFF);
}

void PCA9633::setRGB(uint8_t r, uint8_t g, uint8_t b) {

    setPwm(_regRedPwm, r);
    setPwm(_regGreenPwm, g);
    setPwm(_regBluePwm, b);
}

void PCA9633::setRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {

    setRGB(r, g, b);

    if (_hasWhiteChannel) {
        setPwm(_regWhitePwm, w);
    }
}

void PCA9633::setLdrState(uint8_t state, uint8_t ldrBit) {

    uint8_t prevReg = readReg(REG_LEDOUT);
    uint8_t newReg;

    // first clear both bits of ldr
    newReg = prevReg & ~(0b11 << ldrBit);

    // second set new state to specified ldr
    newReg |= (state << ldrBit);

    writeReg(REG_LEDOUT, newReg);
}

void PCA9633::setChannelLdrState(uint8_t state, uint8_t ldrBit) {

    uint8_t prevReg = readReg(REG_LEDOUT);
    uint8_t newReg;

    // first clear both bits of ldr
    newReg = prevReg & ~(0b11 << ldrBit);

    // second set new state to specified ldr
    newReg |= (state << ldrBit);

    writeReg(REG_LEDOUT, newReg);
}

void PCA9633::setLdrStateAll(uint8_t state) {

    uint8_t newReg = ( state << BIT_LDR3
                       | state << BIT_LDR2
                       | state << BIT_LDR1
                       | state << BIT_LDR0 );

    writeReg(REG_LEDOUT, newReg);
}
