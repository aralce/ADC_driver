#pragma once
#include "../../../ADC_driver_config.h"

#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#include <ADC_driver_api.h>
#ifdef IS_RUNNING_TESTS
    #include <mocks/ADC_library/ADC_firmware/ADC_ADS1115_driver/ESP32_with_ESP_IDF/ads1115.h>
#else
    #include "adc_library/ads1115.h"
#endif

#define ADC_CHANNEL_1   ADS1115_MUX_0_GND
#define ADC_CHANNEL_2   ADS1115_MUX_1_GND

class ADC_ADS1115_driver : public ADC_driver_api {
public:
    void initialize(int sda = GPIO_NUM_21, int scl = GPIO_NUM_22) override;

    void start_new_single_reading() override;

    adc_channel get_input_channel() const override;
    void set_input_channel(adc_channel channel) override;

    bool is_conversion_complete() override;
    
    float get_measured_voltage() override;

    void set_samples_per_second(uint16_t rate) override;

private:
    ads1115_t adc_driver;
    adc_channel _adc_channel = adc_channel::ADC_DRIVER_CHANNEL_1;
};

#endif // ESP32_WITH_ESP_IDF