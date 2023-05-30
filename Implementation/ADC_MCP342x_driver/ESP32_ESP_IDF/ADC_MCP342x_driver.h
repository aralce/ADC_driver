#pragma once
#include "../../../ADC_driver_config.h"
#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#include <cstdint>
#include <ADC_driver_api.h>
#ifdef IS_RUNNING_TESTS
    #include <mcp342x.h>
#else
    #include "MCP342x/mcp342x.h"
#endif

class ADC_MCP342x_driver : public ADC_driver_api {
public:
    void initialize(int sda = 21, int scl = 22) override;
    
    void start_new_single_reading() override;

    adc_channel get_input_channel() const override;
    void set_input_channel(adc_channel channel) override;

    bool is_conversion_complete() override;
    
    float get_measured_voltage() override;

    void set_samples_per_second(uint16_t rate) override;

private:
    smbus_info_t smbus_info = {};
    mcp342x_info_t mcp342x_info;
    mcp342x_config_t mcp342x_config;

    adc_channel _adc_channel = adc_channel::ADC_DRIVER_CHANNEL_1;
};

#endif // ESP32_WITH_ESP_IDF