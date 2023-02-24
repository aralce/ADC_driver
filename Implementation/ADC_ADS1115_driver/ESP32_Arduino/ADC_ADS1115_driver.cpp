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

#endif // ESP32_WITH_ARDUINO