#include "ADC_ADS1115_driver.h"

#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#define ADS1X15_ADDRESS (0x48) ///< 1001 000 (ADDR = GND)
#define I2C_PORT 0
void ADC_ADS1115_driver::initialize() {
    adc_driver = ads1115_config(I2C_PORT, ADS1X15_ADDRESS);
    ads1115_set_mux(&adc_driver, ADS1115_MUX_0_GND);
    ads1115_set_pga(&adc_driver, ADS1115_FSR_4_096);
    ads1115_set_mode(&adc_driver, ADS1115_MODE_SINGLE);
    ads1115_set_sps(&adc_driver, ADS1115_SPS_128);
}

void ADC_ADS1115_driver::start_new_single_reading() {
    ads1115_startConversion(&adc_driver);
}

adc_channel ADC_ADS1115_driver::get_input_channel() const {
    return _adc_channel;
}

void ADC_ADS1115_driver::set_input_channel(adc_channel channel) {
    switch(channel) {
        case adc_channel::ADC_DRIVER_CHANNEL_1:
            ads1115_set_mux(&adc_driver, ADS1115_MUX_0_GND);
            break;
        case adc_channel::ADC_DRIVER_CHANNEL_2:
            ads1115_set_mux(&adc_driver, ADS1115_MUX_1_GND);
            break;
    }
    _adc_channel = channel;
}

bool ADC_ADS1115_driver::is_conversion_complete() {
    return ads1115_conversionComplete(&adc_driver);
}

#define DO_NOT_WAIT_FOR_CONVERSION  false
float ADC_ADS1115_driver::get_measured_voltage() {
    return ads1115_get_voltage(&adc_driver);
}

#endif // ESP32_WITH_ESP_IDF