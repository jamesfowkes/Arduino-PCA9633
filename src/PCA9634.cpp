/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Arduino.h"

#include "PCA9634.h"

/******************************* PUBLIC METHODS *******************************/

PCA9634::PCA9634() : PCA963x(8, REG_GRPPWM_PCA9634, REG_SUBADR1_PCA9634) {}

void PCA9634::turnOn() {
    writeReg(REG_LEDOUT1_PCA9634, _storedRegLedout[0]);
    writeReg(REG_LEDOUT2_PCA9634, _storedRegLedout[1]);
}

void PCA9634::turnOff() {
    _storedRegLedout[0] = readReg(REG_LEDOUT1_PCA9634);
    _storedRegLedout[1] = readReg(REG_LEDOUT2_PCA9634);
    writeReg(REG_LEDOUT1_PCA9634, LDR_STATE_OFF);
    writeReg(REG_LEDOUT2_PCA9634, LDR_STATE_OFF);
}

void PCA9634::setLdr1State(uint8_t state, uint8_t ldrBit) {

    uint8_t prevReg = readReg(REG_LEDOUT1_PCA9634);
    uint8_t newReg;

    // first clear both bits of ldr
    newReg = prevReg & ~(0b11 << ldrBit);

    // second set new state to specified ldr
    newReg |= (state << ldrBit);

    writeReg(REG_LEDOUT1_PCA9634, newReg);
}

void PCA9634::setLdr2State(uint8_t state, uint8_t ldrBit) {

    uint8_t prevReg = readReg(REG_LEDOUT2_PCA9634);
    uint8_t newReg;

    // first clear both bits of ldr
    newReg = prevReg & ~(0b11 << ldrBit);

    // second set new state to specified ldr
    newReg |= (state << ldrBit);

    writeReg(REG_LEDOUT2_PCA9634, newReg);
}

void PCA9634::setChannelLdrState(uint8_t ch, uint8_t state) {

    const uint8_t reg = (ch < 4) ? REG_LEDOUT1_PCA9634 : REG_LEDOUT2_PCA9634;
    uint8_t prevReg = readReg(reg);
    uint8_t newReg;

    // first clear both bits of ldr
    newReg = prevReg & ~(0b11 << (ch*2));

    // second set new state to specified ldr
    newReg |= (state << (ch*2));

    writeReg(reg, newReg);
}

void PCA9634::setLdrStateAll(uint8_t state) {

    uint8_t newReg = ( state << BIT_LDR3
                       | state << BIT_LDR2
                       | state << BIT_LDR1
                       | state << BIT_LDR0 );

    writeReg(REG_LEDOUT1_PCA9634, newReg);
    writeReg(REG_LEDOUT2_PCA9634, newReg);
}
