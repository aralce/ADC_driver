#include "ADC_MCP342x_driver.h"

#if defined(ESP32_WITH_ARDUINO) || defined(IS_RUNNING_TESTS)
#include <Arduino.h>
#include <Wire.h>

const uint8_t default_address = 0x69;

void ADC_MCP342x_driver::initialize(int sda, int scl) {
    Serial.begin(115200);
    if (sda != -1 && scl != -1)
        Wire.setPins(sda, scl);
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
    adc_driver.convert(this->channel, MCP342x::oneShot, resolution, MCP342x::gain1);
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
    MCP342x::error_t  err = adc_driver.read(reading_result, reading_status);
    if (err != MCP342x::error_t::errorNone)
        return false;
    return reading_status.isReady();
}

#define LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION 250e-6
float ADC_MCP342x_driver::get_measured_voltage() {
    adc_driver.read(reading_result, reading_status);
    return reading_result*LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION;
}

static bool is_valid_rate(uint16_t rate);
char tag[] = "ADC MCP342x arduino";

void ADC_MCP342x_driver::set_samples_per_second(uint16_t rate) {
    if (is_valid_rate(rate))
        resolution = get_resolution_from_rate(rate);
    else
        Serial.println( "Error --ADC MCP342x arduino: Invalid Samples per second to set. The operation will do not take place. Please, use a valid value.");
}

static bool is_valid_rate(uint16_t rate) {
    switch (rate) {
        case 240:
        case 60:
        case 15:
            return true;
        default:
            return false;
    }
}

MCP342x::Resolution ADC_MCP342x_driver::get_resolution_from_rate(uint16_t rate) {
    switch(rate) {
        case 240:
            return MCP342x::resolution12;
        case 15:
            return MCP342x::resolution16;
        default:
            return MCP342x::resolution14;
    }
}

#endif // ESP32_WITH_ARDUINO