#include "ADC_ADS1115_driver.h"

#if defined(ESP32_WITH_ARDUINO) || defined(IS_RUNNING_TESTS)

#include <Arduino.h>

#define SERIAL_PORT_BAUDRATE    115200UL

void ADC_ADS1115_driver::initialize() {
    Serial.begin(SERIAL_PORT_BAUDRATE);

    adc_driver.setGain(GAIN_ONE);
    
    if (adc_driver.begin() == false) {
        Serial.println("--Sensor_logger-- Failed to initialize external ADC ads1x15. Please, check the I2C connections and restart micro-controller.");
        #ifndef IS_RUNNING_TESTS
            while(1);
        #endif
    }
}

void ADC_ADS1115_driver::start_new_single_reading() {
    switch (input_channel) {
    case adc_channel::ADC_DRIVER_CHANNEL_1:
        adc_driver.startADCReading(ADC_CHANNEL_2, false);
        break;
    case adc_channel::ADC_DRIVER_CHANNEL_2:
        adc_driver.startADCReading(ADC_CHANNEL_1, false);
        break;
    }
}

adc_channel ADC_ADS1115_driver::get_input_channel() const {
    return input_channel;
}

void ADC_ADS1115_driver::set_input_channel(adc_channel channel) {
    input_channel = channel;
}

bool ADC_ADS1115_driver::is_conversion_complete() {
    return adc_driver.conversionComplete();
}

float ADC_ADS1115_driver::get_measured_voltage() {
    int16_t raw_value = adc_driver.getLastConversionResults();
    return adc_driver.computeVolts(raw_value);
}

static bool is_sps_set_valid(uint16_t rate_to_check);
static uint16_t get_register_mask_from_rate(uint16_t rate);

void ADC_ADS1115_driver::set_samples_per_second(uint16_t rate) {
    // if (is_sps_set_valid(rate))
    if (is_sps_set_valid(rate)) {
        uint16_t config_register_mask = get_register_mask_from_rate(rate);
        adc_driver.setDataRate(config_register_mask);
    }
    else
        Serial.println("Error -- ADC ads1115 samples per second to set is invalid. Please use a valid rate");
}

static bool is_sps_set_valid(uint16_t rate_to_check) {
    uint16_t valid_rates[] = {8, 16, 32, 64, 128, 250, 475, 860};
    for (uint8_t rate_index = 0; rate_index < sizeof(valid_rates)/sizeof(valid_rates[0]); rate_index++)
        if (valid_rates[rate_index] == rate_to_check)
            return true;
    return false;
}

static uint16_t get_register_mask_from_rate(uint16_t rate) {
    switch(rate) {
        case 8:
            return RATE_ADS1115_8SPS;
        case 16:
            return RATE_ADS1115_16SPS;
        case 32:
            return RATE_ADS1115_32SPS;
        case 64:
            return RATE_ADS1115_64SPS;
        case 128:
            return RATE_ADS1115_128SPS;
        case 250:
            return RATE_ADS1115_250SPS;
        case 475:
            return RATE_ADS1115_475SPS;
        default:
            return RATE_ADS1115_860SPS;
    }
}

#endif // ESP32_WITH_ARDUINO