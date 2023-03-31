#include "ADC_MCP342x_driver.h"

#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#define ADC_I2C_PORT 0
const uint8_t default_address = 0x69;

void ADC_MCP342x_driver::initialize() {
    esp_err_t error = smbus_init(&smbus_info, ADC_I2C_PORT, default_address);
    if (error != ESP_OK)
        printf("ERROR: FAILED TO INITIALIZE SMBUS_INFO -- %s\r\n", esp_err_to_name(error));

    mcp342x_config.channel = MCP342X_CHANNEL_1;
    mcp342x_config.conversion_mode = MCP342X_MODE_ONESHOT;
    mcp342x_config.sample_rate = MCP342X_SRATE_14BIT;
    mcp342x_config.gain = MCP342X_GAIN_1X;
    
    error = mcp342x_init(&mcp342x_info, &smbus_info, mcp342x_config);
    if (error != ESP_OK)
        printf("ERROR: FAILED TO INITIALIZE MCP342x -- %s\r\n", esp_err_to_name(error));
}

void ADC_MCP342x_driver::start_new_single_reading() {
    esp_err_t error = mcp342x_start_new_conversion(&mcp342x_info);
    if (error != ESP_OK)
        printf("ERROR:  FAILED TO START NEW CONVERSION -- %s", esp_err_to_name(error));
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
const char read_status[6][27] {"MCP342X_STATUS_OK", "MCP342X_STATUS_UNDERFLOW", "MCP342X_STATUS_OVERFLOW", "MCP342X_STATUS_I2C", "MCP342X_STATUS_IN_PROGRESS", "MCP342X_STATUS_TIMEOUT"};

float ADC_MCP342x_driver::get_measured_voltage() {
    double voltage_measured;
    mcp342x_conversion_status_t status = mcp342x_read_voltage(&mcp342x_info, &voltage_measured);
    printf("Conversion status: %s\r\n", read_status[status]);
    return voltage_measured;
}

#endif // ESP32_WITH_IDF