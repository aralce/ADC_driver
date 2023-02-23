#pragma once
#include "ADC_driver_API.h"

class ADC_driver_factory {
public:
    static HAL_ADC_api* get_new_default_adc_driver_instance() {
        return new HAL_ADC_api;
    }
};