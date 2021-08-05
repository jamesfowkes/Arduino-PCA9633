/*
 * Copyright (C) 2020 Hendrik van Essen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "Arduino.h"
#include "PCA9634.h"

#define RGB_ADDRESS (0x16)

PCA9634 pca9634 = PCA9634();

void reset_pca9634()
{
    pca9634.setDrvState(OUTDRV_TOTEM_POLE);
    pca9634.setLdrStateAll(LDR_STATE_OFF);
    pca9634.setGrpPwm(0);
}

void setup() {
    Serial.begin(115200);

    Wire.setSCL(7);
    Wire.setSDA(8);
    Wire.begin();

    pca9634.begin(RGB_ADDRESS, &Wire);

    pca9634.wakeUp();
}

void loop() {

    reset_pca9634();

    delay(1000);

    // 1. turn on/off
    pca9634.setAllPwm(255);
    pca9634.setLdrStateAll(LDR_STATE_IND_GRP);

    pca9634.turnOn();
    delay(500);

    pca9634.turnOff();
    delay(500);

    pca9634.turnOn();
    delay(500);

    reset_pca9634();

    // 2. group dimming
    pca9634.setAllPwm(255);
    pca9634.setLdrStateAll(LDR_STATE_IND_GRP);

    for (int pwm = 255; pwm >= 0; pwm--) {
        pca9634.setGrpPwm(pwm);
        delay(20);
    }
    delay(1000);

    // 3. changing ldr state
    // 3a. individual on/off control (no PWM)
    pca9634.setGrpPwm(0);
    pca9634.setAllPwm(0);

    for (int ch = 0; ch < 8; ch++)
    {
        pca9634.setChannelLdrState(ch, LDR_STATE_ON);
        // LED on ch should be on
        delay(500);
        pca9634.setChannelLdrState(ch, LDR_STATE_OFF);
        // LED on ch should be off
    }

    reset_pca9634();

    // 3b. individual brightness control (no group control)
    for (int ch = 0; ch < 8; ch++)
    {
        pca9634.setChannelLdrState(ch, LDR_STATE_IND);
        pca9634.setPwm(ch, 32);
        // LED on ch should be on dim
        delay(125);
        pca9634.setPwm(ch, 255);
        // LED on ch should be on bright
        delay(125);
        pca9634.setPwm(ch, 0);
        pca9634.setChannelLdrState(ch, LDR_STATE_OFF);
        // LED on ch should be off
    }

    reset_pca9634();

    // 3c. group and individual brightness control
    pca9634.setGrpPwm(255);
    for (int ch = 0; ch < 8; ch++)
    {
        // Set LEDs to fade up from dim to bright across channels
        // LED0 = 31, LED1 = 63 ... LED7 = 255
        pca9634.setChannelLdrState(ch, LDR_STATE_IND_GRP);
        pca9634.setPwm(ch, (ch * 32) + 31);
    }

    delay(250);
    pca9634.setGrpPwm(128);
    // LEDs should all be half relative brightness

    delay(250);
    pca9634.setGrpPwm(64);
    // LEDs should all be 1/4 relative brightness

    reset_pca9634();

    // 5. test blinking
    pca9634.setGrpPwm(255);
    pca9634.setAllPwm(255);
    pca9634.setGroupControlMode(GROUP_CONTROL_MODE_BLINKING);
    pca9634.setBlinking(BLINKING_PERIOD_1_S, BLINKING_RATIO_BALANCED);
    delay(10000);
    pca9634.setGroupControlMode(GROUP_CONTROL_MODE_DIMMING);

    reset_pca9634();

    // 6. sleep mode
    pca9634.setGrpPwm(255);
    pca9634.setAllPwm(255);

    delay(500);

    pca9634.sleep();
    delay(2000);

    pca9634.wakeUp();
    delay(500);

    reset_pca9634();
}
