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
PCA9633::PCA9633(uint8_t regRedPwm, uint8_t regGreenPwm, uint8_t regBluePwm) {

    _regRedPwm = regRedPwm;
    _regGreenPwm = regGreenPwm;
    _regBluePwm = regBluePwm;
}

void PCA9633::begin(uint16_t deviceAddress, TwoWire *wire) {

    _deviceAddres = deviceAddress;

    _wire = wire;
    _wire->begin();

    // clear/ reset registers
    writeReg(REG_MODE1, 0x0);
    writeReg(REG_MODE2, 0x0);

    setLdrStateAll(LDR_STATE_IND_GRP);
    setGroupControlMode(GROUP_CONTROL_MODE_DIMMING);
    setRGB(255, 255, 255);
}

void PCA9633::wakeUp() {

    uint8_t prevReg = readReg(REG_MODE1);
    uint8_t newReg = prevReg & ~(1 << BIT_SLEEP);

    writeReg(REG_MODE1, newReg);
}

void PCA9633::sleep() {

    uint8_t prevReg = readReg(REG_MODE1);
    uint8_t newReg = prevReg | (1 << BIT_SLEEP);

    writeReg(REG_MODE1, newReg);
}

void PCA9633::turnOn() {

    writeReg(REG_LEDOUT, _storedRegLedout);
}

void PCA9633::turnOff() {

    _storedRegLedout = readReg(REG_LEDOUT);
    writeReg(REG_LEDOUT, LDR_STATE_OFF);
}

void PCA9633::setPwm(uint8_t regPwm, uint8_t pwm) {

    writeReg(regPwm, pwm);
}

void PCA9633::setGrpPwm(uint8_t pwm) {

    writeReg(REG_GRPPWM, pwm);
}

void PCA9633::setBlinking(uint8_t blinkPeriod, float onOffRatio) {

    int16_t ratio = onOffRatio * 256;

    if (ratio < 0) {
        ratio = 0;
    }
    else if (ratio > 255) {
        ratio = 255;
    }

    writeReg(REG_GRPFREQ, blinkPeriod);
    writeReg(REG_GRPPWM, (uint8_t) ratio);
}

void PCA9633::setRGB(uint8_t r, uint8_t g, uint8_t b) {

    setPwm(_regRedPwm, r);
    setPwm(_regGreenPwm, g);
    setPwm(_regBluePwm, b);
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

void PCA9633::setLdrStateAll(uint8_t state) {

    uint8_t newReg = ( state << BIT_LDR3
                       | state << BIT_LDR2
                       | state << BIT_LDR1
                       | state << BIT_LDR0 );

    writeReg(REG_LEDOUT, newReg);
}

void PCA9633::setAutoIncrement(uint8_t option) {

    bool enabled, bit0, bit1;

    switch (option) {

        case AI_ALL:
            enabled = true;
            bit0 = false;
            bit1 = false;
            break;

        case AI_IND:
            enabled = true;
            bit0 = false;
            bit1 = true;
            break;

        case AI_GBL:
            enabled = true;
            bit0 = true;
            bit1 = false;
            break;

        case AI_IND_GBL:
            enabled = true;
            bit0 = true;
            bit1 = true;
            break;

        case AI_DISABLED:
        default:
            enabled = false;
            bit0 = false;
            bit1 = false;
            break;
    }

    uint8_t newReg = ( enabled << BIT_AI2
                       | bit1 << BIT_AI1
                       | bit0 << BIT_AI0 );

    writeReg(REG_MODE1, newReg);
}

void PCA9633::setGroupControlMode(uint8_t mode) {

    uint8_t prevReg = readReg(REG_MODE2);

    switch (mode) {

        case GROUP_CONTROL_MODE_BLINKING:
            writeReg(REG_MODE2, prevReg | (1 << BIT_DMBLNK));
            break;

        case GROUP_CONTROL_MODE_DIMMING:
        default:
            writeReg(REG_MODE2, prevReg & ~(1 << BIT_DMBLNK));
            break;
    }
}

/****************************** PRIVATE METHODS *******************************/

void PCA9633::writeReg(uint8_t registerAddress, uint8_t data) {

    _wire->beginTransmission(_deviceAddres);
    _wire->write(registerAddress);
    _wire->write(data);
    _wire->endTransmission();
}

uint8_t PCA9633::readReg(uint8_t registerAddress) {

    _wire->beginTransmission(_deviceAddres);
    _wire->write(registerAddress);
    _wire->endTransmission();

    _wire->requestFrom(_deviceAddres, (uint8_t) 1, true);

    if (_wire->available() == 1) {
        return _wire->read();
    }

    return -1;
}
