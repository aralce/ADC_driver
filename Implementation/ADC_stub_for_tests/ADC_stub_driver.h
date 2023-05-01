#pragma once
#include <cstdint>
#include <ADC_driver_api.h>

extern float measured_voltage;

class ADC_stub : public ADC_driver_api {
public:
    void initialize() override {}
    
    void start_new_single_reading() override {}

    adc_channel get_input_channel() const override {return _channel;}
    void set_input_channel(adc_channel channel) override {_channel = channel;}

    bool is_conversion_complete() override {return true;}
    
    float get_measured_voltage() override {return measured_voltage;}

private:
    adc_channel _channel = adc_channel::ADC_DRIVER_CHANNEL_1;
};