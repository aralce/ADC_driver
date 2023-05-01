#include "ADC_driver_factory.h"
#include <Implementation/ADC_implementation.h>

ADC_driver_api* ADC_driver_factory::get_new_default_adc_driver_instance() {
    return ADC_driver_factory::get_new_adc_driver_instance(ADC_DEFAULT_MODEL);
}

ADC_driver_api* ADC_driver_factory::get_new_adc_driver_instance(adc_model instance_type) {
    switch (instance_type) {
        case adc_model::ADC_ADS1115_driver:
            return new ADC_ADS1115_driver;

        case adc_model::ADC_MCP342x_driver:
            return new ADC_MCP342x_driver;

        case adc_model::ADC_STUB:
            return new ADC_stub;
            
        default:
            return new ADC_ADS1115_driver;
    }
}

void set_adc_stub_measured_voltage(float value) {
    measured_voltage = value;
}