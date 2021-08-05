/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Arduino.h"

#include "PCA963x.h"

/******************************* PUBLIC METHODS *******************************/

PCA963x::PCA963x(uint8_t _numChannels, uint8_t grpPWMRegisterAddr, uint8_t subAddrRegisterAddr) :
    _numChannels(_numChannels),
    _grpPWMRegisterAddr(grpPWMRegisterAddr),
    _subAddrRegisterAddr(subAddrRegisterAddr)
{

}

void PCA963x::begin(uint8_t deviceAddress, TwoWire *wire) {

    _deviceAddres = deviceAddress;

    _wire = wire;
    _wire->begin();

    // clear/ reset registers
    writeReg(REG_MODE1, 0x0);
    writeReg(REG_MODE2, 0x0);
}

void PCA963x::wakeUp() {

    uint8_t prevReg = readReg(REG_MODE1);
    uint8_t newReg = prevReg & ~(1 << BIT_SLEEP);

    writeReg(REG_MODE1, newReg);
}

void PCA963x::sleep() {

    uint8_t prevReg = readReg(REG_MODE1);
    uint8_t newReg = prevReg | (1 << BIT_SLEEP);

    writeReg(REG_MODE1, newReg);
}

void PCA963x::setAllPwm(uint8_t pwm) {

    for (uint8_t ch=0; ch < _numChannels; ch++)
    {
        writeReg(REG_PWM0+ch, pwm);
    }
}

void PCA963x::setPwm(uint8_t regPwm, uint8_t pwm) {

    writeReg(regPwm, pwm);
}

void PCA963x::setGrpPwm(uint8_t pwm) {

    writeReg(_grpPWMRegisterAddr, pwm);
}

void PCA963x::setBlinking(uint8_t blinkPeriod, float onOffRatio) {

    int16_t ratio = onOffRatio * 256;

    if (ratio < 0) {
        ratio = 0;
    }
    else if (ratio > 255) {
        ratio = 255;
    }

    writeReg(_grpPWMRegisterAddr+1, blinkPeriod);
    writeReg(_grpPWMRegisterAddr, (uint8_t) ratio);
}

void PCA963x::setDrvState(uint8_t state) {

    uint8_t prevReg = readReg(REG_MODE2);
    uint8_t newReg;

    // first clear the OUTDRV bit
    newReg = prevReg & ~(1 << BIT_OUTDRV);

    // second set new state to specified drv
    newReg |= (state << BIT_OUTDRV);

    writeReg(REG_MODE2, newReg);
}

void PCA963x::setInvertState(uint8_t invert) {

    uint8_t prevReg = readReg(REG_MODE2);
    uint8_t newReg;

    // first clear the INVRT bit
    newReg = prevReg & ~(1 << BIT_INVRT);

    // second set new state to specified invrt
    newReg |= (invert << BIT_INVRT);

    writeReg(REG_MODE2, newReg);
}

void PCA963x::setAutoIncrement(uint8_t option) {

    uint8_t newReg;

    switch (option) {

        case AI_ALL:
            newReg = (1 << BIT_AI2)
                   | (0 << BIT_AI1)
                   | (0 << BIT_AI0);
            break;

        case AI_IND:
            newReg = (1 << BIT_AI2)
                   | (1 << BIT_AI1)
                   | (0 << BIT_AI0);
            break;

        case AI_GBL:
            newReg = (1 << BIT_AI2)
                   | (0 << BIT_AI1)
                   | (1 << BIT_AI0);
            break;

        case AI_IND_GBL:
            newReg = (1 << BIT_AI2)
                   | (1 << BIT_AI1)
                   | (1 << BIT_AI0);
            break;

        case AI_DISABLED:
        default:
            newReg = (0 << BIT_AI2)
                   | (0 << BIT_AI1)
                   | (0 << BIT_AI0);
            break;
    }

    writeReg(REG_MODE1, newReg);
}

void PCA963x::setGroupControlMode(uint8_t mode) {

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

void PCA963x::writeReg(uint8_t registerAddress, uint8_t data) {

    _wire->beginTransmission(_deviceAddres);
    _wire->write(registerAddress);
    _wire->write(data);
    _wire->endTransmission();
}

uint8_t PCA963x::readReg(uint8_t registerAddress) {

    _wire->beginTransmission(_deviceAddres);
    _wire->write(registerAddress);
    _wire->endTransmission();

    _wire->requestFrom(_deviceAddres, (uint8_t) 1);

    if (_wire->available() == 1) {
        return _wire->read();
    }

    return -1;
}
