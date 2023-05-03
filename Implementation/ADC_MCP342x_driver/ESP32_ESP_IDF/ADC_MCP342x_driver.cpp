#include "ADC_MCP342x_driver.h"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#if defined(ESP32_WITH_ESP_IDF) || defined(IS_RUNNING_TESTS)

#define ADC_I2C_PORT 0
const uint8_t default_address = 0x69;

void ADC_MCP342x_driver::initialize() {
    char tag[] = "ADC_MCP342x_driver_ESP_IDF";
    esp_err_t err = ESP_OK;
    do {
        err = smbus_init(&smbus_info, ADC_I2C_PORT, default_address);
        vTaskDelay(1);

        mcp342x_config.channel = MCP342X_CHANNEL_1;
        mcp342x_config.conversion_mode = MCP342X_MODE_ONESHOT;
        mcp342x_config.sample_rate = MCP342X_SRATE_14BIT;
        mcp342x_config.gain = MCP342X_GAIN_1X;
    
        err |= mcp342x_init(&mcp342x_info, &smbus_info, mcp342x_config);
        if (err != ESP_OK)
            ESP_LOGE(tag, "error_on_initialization");
    }while (err != ESP_OK);
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
const char read_status[6][27] {"MCP342X_STATUS_OK", "MCP342X_STATUS_UNDERFLOW", "MCP342X_STATUS_OVERFLOW", "MCP342X_STATUS_I2C", "MCP342X_STATUS_IN_PROGRESS", "MCP342X_STATUS_TIMEOUT"};

float ADC_MCP342x_driver::get_measured_voltage() {
    double voltage_measured;
    mcp342x_read_voltage(&mcp342x_info, &voltage_measured);
    // mcp342x_conversion_status_t status = mcp342x_read_voltage(&mcp342x_info, &voltage_measured);
    // printf("Conversion status: %s\r\n", read_status[status]);
    return voltage_measured;
}

char tag[] = "ADC MCP342x esp-idf";
static bool is_rate_valid(uint16_t rate);
static void set_config_with_sample_rate(mcp342x_config_t& config, uint16_t rate);

void ADC_MCP342x_driver::set_samples_per_second(uint16_t rate) {
    if (is_rate_valid(rate)) {
        set_config_with_sample_rate(mcp342x_config, rate);
        mcp342x_set_config(&mcp342x_info, mcp342x_config);
    }else
        ESP_LOGE(tag, "Error -- samples per second to set is invalid and won't take effect. Please use a valid value.");
}

static bool is_rate_valid(uint16_t rate) {
    switch(rate) {
        case 240:
        case 60:
        case 15:
            return true;
        default:
            return false;
    }
}

static void set_config_with_sample_rate(mcp342x_config_t& config, uint16_t rate) {
    switch(rate) {
        case 240:
            config.sample_rate = MCP342X_SRATE_12BIT;
            break;
        case 15:
            config.sample_rate = MCP342X_SRATE_16BIT;
            break;
        default:
            config.sample_rate = MCP342X_SRATE_14BIT;
    }
    
}

#endif // ESP32_WITH_IDF