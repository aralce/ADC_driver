#pragma once
#include <cstdint>

enum class adc_channel{ADC_DRIVER_CHANNEL_1, ADC_DRIVER_CHANNEL_2};
class ADC_driver_api {
public:
    virtual ~ADC_driver_api() {}

    virtual void initialize(int sda = -1, int scl = -1) = 0;
    
    virtual void start_new_single_reading() = 0;

    virtual adc_channel get_input_channel() const = 0;
    virtual void set_input_channel(adc_channel channel) = 0;

    virtual bool is_conversion_complete() = 0;
    
    virtual double get_measured_voltage() = 0;

    virtual void set_samples_per_second(uint16_t rate) = 0;
};