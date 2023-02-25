#include "ADC_MCP342x_driver.h"

#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#define ADC_I2C_PORT 0
const uint8_t default_address = 0x69;

void ADC_MCP342x_driver::initialize() {
    smbus_init(&smbus_info, ADC_I2C_PORT, default_address);

    mcp342x_config.channel = MCP342X_CHANNEL_1;
    mcp342x_config.conversion_mode = MCP342X_MODE_ONESHOT;
    mcp342x_config.sample_rate = MCP342X_SRATE_14BIT;
    mcp342x_config.gain = MCP342X_GAIN_1X;
    mcp342x_init(&mcp342x_info, &smbus_info, mcp342x_config);
}

void ADC_MCP342x_driver::start_new_single_reading() {
    mcp342x_start_new_conversion(&mcp342x_info);
}

adc_channel ADC_MCP342x_driver::get_input_channel() const {
    return _adc_channel;
}

void ADC_MCP342x_driver::set_input_channel(adc_channel channel) {
    _adc_channel = channel;
    switch (channel) {
        case adc_channel::ADC_DRIVER_CHANNEL_1:
            mcp342x_config.channel = MCP342X_CHANNEL_1;
            break;
        case adc_channel::ADC_DRIVER_CHANNEL_2:
            mcp342x_config.channel = MCP342X_CHANNEL_2;
    }
    mcp342x_set_config(&mcp342x_info, mcp342x_config);
}

bool ADC_MCP342x_driver::is_conversion_complete() {
    return is_mcp342x_conversion_complete(&mcp342x_info);
}

#define LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION 250e-6
float ADC_MCP342x_driver::get_measured_voltage() {
    double voltage_measured;
    mcp342x_read_voltage(&mcp342x_info, &voltage_measured);
    return voltage_measured;
}

#endif // ESP32_WITH_IDF