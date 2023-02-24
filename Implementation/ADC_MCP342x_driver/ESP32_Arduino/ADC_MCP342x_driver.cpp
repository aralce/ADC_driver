#include "ADC_MCP342x_driver.h"
#include <Arduino.h>
#include <Wire.h>

const uint8_t default_address = 0x69;

void ADC_MCP342x_driver::initialize() {
    Serial.begin(115200);
    Wire.begin();

    MCP342x::generalCallReset();
    delay(1); //wait for adc to reset

    if (adc_driver.autoprobe(&default_address, 1) == false) {
        Serial.println("--Sensor_logger-- Failed to initialize external ADC MCP342x. Please, check the I2C connections and restart micro-controller");
        #ifndef IS_RUNNING_TESTS
            while(1);
        #endif
    }
}

void ADC_MCP342x_driver::start_new_single_reading() {
    adc_driver.convert(this->channel, MCP342x::oneShot, MCP342x::resolution14, MCP342x::gain1);
}

adc_channel ADC_MCP342x_driver::get_input_channel() const {
    if (this->channel == MCP342x::channel1)
        return adc_channel::ADC_DRIVER_CHANNEL_1;
    else
        return adc_channel::ADC_DRIVER_CHANNEL_2;
}

void ADC_MCP342x_driver::set_input_channel(adc_channel channel) {
    switch (channel) {
        case adc_channel::ADC_DRIVER_CHANNEL_1:
            this->channel = MCP342x::channel1;
            break;
        case adc_channel::ADC_DRIVER_CHANNEL_2:
            this->channel = MCP342x::channel2;
            break;
    }
}

bool ADC_MCP342x_driver::is_conversion_complete() {
    adc_driver.read(reading_result, reading_status);
    return reading_status.isReady();
}

#define LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION 250e-6
float ADC_MCP342x_driver::get_measured_voltage() {
    adc_driver.read(reading_result, reading_status);
    return reading_result*LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION;
}