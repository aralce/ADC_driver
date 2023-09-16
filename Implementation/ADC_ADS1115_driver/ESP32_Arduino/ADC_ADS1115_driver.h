#pragma once
#include "../../../ADC_driver_config.h"
#if defined(ESP32_WITH_ARDUINO) || defined(IS_RUNNING_TESTS)

#include <ADC_driver_api.h>
#ifdef IS_RUNNING_TESTS
    #include <Adafruit_ADS1X15.h>
#else
    #include <Implementation/ADC_ADS1115_driver/ESP32_Arduino/Adafruit_ADS1115/Adafruit_ADS1X15.h>
#endif

#define ADC_CHANNEL_1   MUX_BY_CHANNEL[0]
#define ADC_CHANNEL_2   MUX_BY_CHANNEL[1]

class ADC_ADS1115_driver : public ADC_driver_api {
public:
    void initialize(int sda = -1, int scl = -1) override;

    void start_new_single_reading() override;

    adc_channel get_input_channel() const override;
    void set_input_channel(adc_channel channel) override;

    bool is_conversion_complete() override;
    
    double get_measured_voltage() override;

    void set_samples_per_second(uint16_t rate) override;

private:
    Adafruit_ADS1115 adc_driver;
    adc_channel input_channel = adc_channel::ADC_DRIVER_CHANNEL_1;
};

#endif // ESP32_WITH_ARDUINO