#pragma once
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

enum class adc_channel{ADC_DRIVER_CHANNEL_1, ADC_DRIVER_CHANNEL_2};

class HAL_ADC_api {
public:
    void initialize() {
        mock().actualCall("HAL_ADC_api->initialize");
    }

    void start_new_single_reading() {
        mock().actualCall("HAL_ADC_api->start_new_single_reading");
    }
    
    void set_input_channel(adc_channel channel) {
        mock().actualCall("HAL_ADC_api->set_input_channel")
              .withUnsignedIntParameter("channel", (uint32_t)channel);
    }

    bool is_conversion_complete() {
        mock().actualCall("HAL_ADC_api->is_conversion_complete");
        return mock().returnBoolValueOrDefault(false);
    }
    
    float get_measured_voltage() {
        mock().actualCall("HAL_ADC_api->get_measured_voltage");
        return mock().returnDoubleValueOrDefault(0);
    }

};