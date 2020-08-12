/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Arduino.h"
#include "PCA9633.h"

#define RGB_ADDRESS (0xc0 >> 1)

PCA9633 pca9633 = PCA9633(REG_PWM2, REG_PWM1, REG_PWM0);

void setup() {
    Serial.begin(115200);
    pca9633.begin(RGB_ADDRESS, &Wire);

    pca9633.setLdrStateAll(LDR_STATE_IND_GRP);
    pca9633.setGroupControlMode(GROUP_CONTROL_MODE_DIMMING);
    pca9633.setRGB(255, 255, 255);
}

void loop() {

    delay(1000);

    // 1. turn on/off
    pca9633.turnOff();
    delay(500);

    pca9633.turnOn();
    delay(500);

    // 2. individual dimming (setRGB() uses setPwm() internally)
    pca9633.setRGB(255, 255, 255);
    delay(500);

    pca9633.setRGB(255, 0, 0);
    delay(500);

    pca9633.setRGB(0, 255, 0);
    delay(500);

    pca9633.setRGB(0, 0, 255);
    delay(500);

    // 3. group dimming
    pca9633.setRGB(255, 255, 255);
    pca9633.setLdrStateAll(LDR_STATE_IND_GRP);

    for (int pwm = 255; pwm >= 0; pwm--) {
        pca9633.setGrpPwm(pwm);
        delay(20);
    }
    delay(1000);

    // 4. changing ldr state
    pca9633.setGrpPwm(255);
    pca9633.setRGB(255, 255, 255);
    pca9633.setLdrState(LDR_STATE_OFF, BIT_LDR1);
    // color should be magenta
    delay(500);

    pca9633.setGrpPwm(0);
    pca9633.setRGB(0, 0, 0);
    pca9633.setLdrState(LDR_STATE_ON, BIT_LDR1);
    // color should be green
    delay(500);

    pca9633.setGrpPwm(255);
    pca9633.setRGB(255, 128, 0);
    pca9633.setLdrState(LDR_STATE_IND, BIT_LDR1);
    // color should be orange
    delay(500);

    pca9633.setGrpPwm(0);
    pca9633.setRGB(255, 255, 255);
    pca9633.setLdrState(LDR_STATE_IND_GRP, BIT_LDR1);
    // should be no color at all
    delay(500);

    // 5. test blinking
    pca9633.setGrpPwm(255);
    pca9633.setRGB(255, 255, 255);
    pca9633.setGroupControlMode(GROUP_CONTROL_MODE_BLINKING);
    pca9633.setBlinking(BLINKING_PERIOD_1_S, BLINKING_RATIO_BALANCED);
    delay(10000);
    pca9633.setGroupControlMode(GROUP_CONTROL_MODE_DIMMING);

    // 6. sleep mode
    pca9633.setRGB(0, 255, 255);
    delay(500);

    pca9633.sleep();
    delay(2000);

    pca9633.wakeUp();
    delay(500);
}
