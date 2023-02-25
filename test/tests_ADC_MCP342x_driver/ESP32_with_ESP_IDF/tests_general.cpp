#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <Implementation/ADC_MCP342x_driver/ESP32_ESP_IDF/ADC_MCP342x_driver.h>
#include <mocks/ADC_library/ADC_firmware/ADC_MCP342x_driver/ESP32_with_ESP_IDF/mcp342x.h>

ADC_MCP342x_driver* adc_driver = nullptr;

TEST_GROUP(ADC_MCP342x_driver_ESP_IDF)
{
    Mcp342x_config_t_comparator comp;
    void setup()
    {
        mock().installComparator("mcp342x_config_t", comp);
        mock().disable();
        adc_driver = new ADC_MCP342x_driver();
        mock().enable();
    }
    void teardown()
    {
        if (adc_driver != nullptr)
        {
            delete adc_driver;
            adc_driver = nullptr;
        }
        mock().checkExpectations();
        mock().clear();
        mock().removeAllComparatorsAndCopiers();
    }
};

#define ADC_ADDRESS 0x69
mcp342x_config_t default_config = {
    .channel = MCP342X_CHANNEL_1,
    .conversion_mode = MCP342X_MODE_ONESHOT,
    .sample_rate = MCP342X_SRATE_14BIT,
    .gain = MCP342X_GAIN_1X
};

TEST(ADC_MCP342x_driver_ESP_IDF, initialize_ADC_driver)
{
    mock().expectOneCall("smbus_init")
          .withIntParameter("i2c_port", 0)
          .withIntParameter("address", ADC_ADDRESS)
          .ignoreOtherParameters();

    mock().expectOneCall("mcp342x_init")
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)&default_config)
          .ignoreOtherParameters();

    adc_driver->initialize();
}

#define DEFAULT_INPUT_CHANNEL (uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1
TEST(ADC_MCP342x_driver_ESP_IDF, default_input_channel_is_ADC_DRIVER_CHANNEL_1)
{
    CHECK_EQUAL(DEFAULT_INPUT_CHANNEL, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_MCP342x_driver_ESP_IDF, set_input_channel)
{
    mock().disable();
    adc_driver->initialize();
    mock().enable();
    
    mcp342x_config_t config = default_config;
    config.channel = MCP342X_CHANNEL_2;
    mock().expectOneCall("mcp342x_set_config")
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)&config)
          .ignoreOtherParameters();
    
    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_2);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_2, (uint32_t)adc_driver->get_input_channel());

    config.channel = MCP342X_CHANNEL_1;
    mock().expectOneCall("mcp342x_set_config")
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)&config)
          .ignoreOtherParameters();

    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_1);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_MCP342x_driver_ESP_IDF, is_conversion_complete)
{
    mock().disable();
    adc_driver->initialize();
    mock().enable();

    mock().expectOneCall("is_mcp342x_conversion_complete")
          .ignoreOtherParameters()
          .andReturnValue(false);
    CHECK_FALSE(adc_driver->is_conversion_complete());

    mock().expectOneCall("is_mcp342x_conversion_complete")
          .ignoreOtherParameters()
          .andReturnValue(true);
    CHECK_TRUE(adc_driver->is_conversion_complete());
}

TEST(ADC_MCP342x_driver_ESP_IDF, get_measured_voltage)
{
    mock().disable();
    adc_driver->initialize();
    mock().enable();
    
    double EXPECTED_VOLTAGE = 2.5;
    mock().expectOneCall("mcp342x_read_voltage")
          .withOutputParameterReturning("result", (const void*)&EXPECTED_VOLTAGE, sizeof(EXPECTED_VOLTAGE))
          .ignoreOtherParameters();
    DOUBLES_EQUAL(EXPECTED_VOLTAGE, adc_driver->get_measured_voltage(), 0.00001f);
}

TEST(ADC_MCP342x_driver_ESP_IDF, start_new_single_reading)
{
    mock().disable();
    adc_driver->initialize();
    mock().enable();

    mock().expectOneCall("mcp342x_start_new_conversion")
          .ignoreOtherParameters();
    adc_driver->start_new_single_reading();
}