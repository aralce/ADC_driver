#pragma once
#include <ADC_driver_api.h>
#include <Adafruit_ADS1X15.h>

#define ADC_CHANNEL_1   MUX_BY_CHANNEL[0]
#define ADC_CHANNEL_2   MUX_BY_CHANNEL[1]

class ADC_ADS1115_driver : public ADC_driver_api {
public:
    void initialize() override;

    void start_new_single_reading() override;

    adc_channel get_input_channel() const override;
    void set_input_channel(adc_channel channel) override;

    bool is_conversion_complete() override;
    
    float get_measured_voltage() override;

private:
    Adafruit_ADS1115 adc_driver;
    adc_channel input_channel = adc_channel::ADC_DRIVER_CHANNEL_1;
};