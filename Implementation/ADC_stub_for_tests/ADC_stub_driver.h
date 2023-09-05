#pragma once
#include <cstdint>
#include <ADC_driver_api.h>

extern float measured_voltage;

class ADC_stub : public ADC_driver_api {
public:
    void initialize(int sda = -1, int scl = -1) override {}
    
    void start_new_single_reading() override {}

    adc_channel get_input_channel() const override {return _channel;}
    void set_input_channel(adc_channel channel) override {_channel = channel;}

    bool is_conversion_complete() override {return true;}
    
    double get_measured_voltage() override {return measured_voltage;}

    void set_samples_per_second(uint16_t rate) override {}

private:
    adc_channel _channel = adc_channel::ADC_DRIVER_CHANNEL_1;
};