#pragma once
#include <ADC_driver_api.h>

enum class adc_model{ADC_ADS1115_driver, ADC_MCP342x_driver};
#define ADC_DEFAULT_MODEL   adc_model::ADC_MCP342x_driver

class ADC_driver_factory {
public:
    static ADC_driver_api* get_new_default_adc_driver_instance();
    static ADC_driver_api* get_new_adc_driver_instance(adc_model instance_type);

    ADC_driver_factory() = delete;
    ADC_driver_factory(const ADC_driver_factory&) = delete;
    ADC_driver_factory(ADC_driver_factory&&) = delete;
    ADC_driver_factory& operator=(const ADC_driver_factory&) = delete;
    ADC_driver_factory& operator=(ADC_driver_factory&&) = delete;
};