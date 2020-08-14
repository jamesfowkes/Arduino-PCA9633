# PCA9633 driver for Arduino

This library was developed and tested on an esp32 with a PCA9633DP2 controller,
but should work for every other board as well.

## Installation

### Arduino IDE

#### Installation using the Library Manager (IDE 1.6.2 and later)
In the Arduino IDE, simply click on:\
`[Sketch] -> Include Library -> Manage Libraries`\
or\
`[Tools] -> Manage Libraries`\
Then search for `NXP PCA9633` to locate the library and install it.

#### Installation prior to IDE 1.6.2
Because the Library Manager is not available on versions prior to 1.6.2, you
would have to clone this repository or download the .zip file to install the
library manually. This is strongly discouraged as it can lead to several
problems. You should really do yourself a favour and update your Arduino IDE to
the latest version and install the library using the Library Manager instead.

For generic information about Arduino libraries and how to install them consult
the Arduino Libraries page: https://www.arduino.cc/en/Guide/Libraries

### Other platforms
There isn't a sophisticated installation support for other platforms yet (which
doesn't mean that the library won't work). As long as Arduino libraries are
supported, simply clone this repository according to the platforms guidelines.

## API overview:
```cpp
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
    void begin(uint16_t deviceAddress, TwoWire *wire);

    /**
     * Turn on all LEDs. Restores settings saved at turnOff().
     *
     * WARNING: If you call turnOff() twice, without calling turnOn() in between,
     *          then the restored state will be the turned off state!
     */
    void turnOn();

    /**
     * Turn off all LEDs. Saves current settings for turnOn().
     * For power saving, see sleep().
     *
     * WARNING: If you call turnOff() twice, without calling turnOn() in between,
     *          then the restored state will be the turned off state!
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
```

There is an application called `ApiExample` in `examples`, where you can have a
look how the API works and how it's intended to be used.
