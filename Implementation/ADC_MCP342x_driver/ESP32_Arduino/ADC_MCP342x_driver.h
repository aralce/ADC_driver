#pragma once
#include "../../../ADC_driver_config.h"

#if defined(ESP32_WITH_ARDUINO) || defined(IS_RUNNING_TESTS)

#include <cstdint>
#include <ADC_driver_api.h>
#ifdef IS_RUNNING_TESTS
    #include <MCP342x.h>
#else
    #include "MCP342x/MCP342x.h"
#endif

class ADC_MCP342x_driver : public ADC_driver_api {
public:
    void initialize() override;
    
    void start_new_single_reading() override;

    adc_channel get_input_channel() const override;
    void set_input_channel(adc_channel channel) override;

    bool is_conversion_complete() override;
    
    float get_measured_voltage() override;

private:
    MCP342x adc_driver{(uint8_t)0x69};

    MCP342x::Channel channel = MCP342x::channel1;
    MCP342x::Config reading_status;
    long reading_result = 0;
};

#endif // USE_ESP32_WITH_ESP_IDF