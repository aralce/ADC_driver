#pragma once
#include "../../../ADC_driver_config.h"
#ifdef ESP32_WITH_ESP_IDF

#include <ADC_driver_api.h>
#include "adc_library/ads1115.h"

#define ADC_CHANNEL_1   ADS1115_MUX_0_GND
#define ADC_CHANNEL_2   ADS1115_MUX_1_GND

class ADC_ADS1115_driver : public ADC_driver_api {
public:
    void initialize() override;

    void start_new_single_reading() override;

    adc_channel get_input_channel() const override;
    void set_input_channel(adc_channel channel) override;

    bool is_conversion_complete() override;
    
    float get_measured_voltage() override;

private:

};

#endif // ESP32_WITH_ESP_IDF