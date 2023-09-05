#include "ADC_ADS1115_driver.h"
#include <esp_log.h>

#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#define ADS1X15_ADDRESS (0x48) ///< 1001 000 (ADDR = GND)
#define I2C_PORT 0
void ADC_ADS1115_driver::initialize(int sda, int scl) {
    int _sda = sda != -1 ? sda : GPIO_NUM_21;
    int _scl = scl != -1 ? scl : GPIO_NUM_22;
    adc_driver = ads1115_config(I2C_PORT, _sda, _scl, ADS1X15_ADDRESS);
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
double ADC_ADS1115_driver::get_measured_voltage() {
    return ads1115_get_voltage(&adc_driver);
}

static char tag[] = "ADS1115 esp-idf";
static bool is_rate_valid(uint16_t rate);
static ads1115_sps_t get_sps_enum_value_from_rate(uint16_t rate);

void ADC_ADS1115_driver::set_samples_per_second(uint16_t rate) {
    if (is_rate_valid(rate)) {
        ads1115_sps_t enum_value = get_sps_enum_value_from_rate(rate);
        ads1115_set_sps(&adc_driver, enum_value);
    }
    else
        ESP_LOGE(tag, "The value to set samples per second is invalid.");
}

static bool is_rate_valid(uint16_t rate) {
    uint16_t valid_rates[] = {8, 16, 32, 64, 128, 250, 475, 860};
    for(uint8_t rate_index = 0; rate_index < sizeof(valid_rates)/sizeof(valid_rates[0]); ++rate_index)
        if (rate == valid_rates[rate_index])
            return true;
    return false;
}

static ads1115_sps_t get_sps_enum_value_from_rate(uint16_t rate) {
    switch (rate) {
        case 8:
            return ADS1115_SPS_8;
        case 16:
            return ADS1115_SPS_16;
        case 32:
            return ADS1115_SPS_32;
        case 64:
            return ADS1115_SPS_64;
        case 128:
            return ADS1115_SPS_128;
        case 250:
            return ADS1115_SPS_250;
        case 475:
            return ADS1115_SPS_475;
        default:
            return ADS1115_SPS_860;
    }
}

#endif // ESP32_WITH_ESP_IDF