#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <Implementation/ADC_MCP342x_driver/ESP32_ESP_IDF/ADC_MCP342x_driver.h>
#include <mocks/ADC_library/ADC_firmware/ADC_MCP342x_driver/ESP32_with_ESP_IDF/mcp342x.h>

#include <esp_log.h>

ADC_driver_api* adc_driver = nullptr;
// ADC_MCP342x_driver* adc_driver = nullptr;

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
    .sample_rate = MCP342X_SRATE_12BIT,
    .gain = MCP342X_GAIN_1X
};

static void CHECK_adc_driver_initialization_functions_are_called(int smbus_return_value, int mcp342x_init_return_value);

TEST(ADC_MCP342x_driver_ESP_IDF, initialize_ADC_driver)
{
    CHECK_adc_driver_initialization_functions_are_called(ESP_OK, ESP_OK);
    adc_driver->initialize();
}

TEST(ADC_MCP342x_driver_ESP_IDF, GIVEN_system_is_not_initialized_WHEN_initialize_system_THEN_set_sda_and_scl_for_I2C)
{
    const int SCL = 2;
    const int SDA = 15;
    mock().expectOneCall("smbus_init")
      .withIntParameter("sda", SDA)
      .withIntParameter("scl", SCL)
      .withIntParameter("i2c_port", 0)
      .withIntParameter("address", ADC_ADDRESS)
      .ignoreOtherParameters()
      .andReturnValue(ESP_OK);
    mock().ignoreOtherCalls();

    adc_driver->initialize(SDA, SCL);
}

TEST(ADC_MCP342x_driver_ESP_IDF, GIVEN_initialize_ADC_driver_WHEN_init_has_issues_THEN_reinit_driver_and_log_message)
{
    mock().expectNCalls(2, "ESP_LOGE")
          .withStringParameter("tag", "ADC_MCP342x_driver_ESP_IDF")
          .withStringParameter("message", "error_on_initialization");
    
    CHECK_adc_driver_initialization_functions_are_called(ESP_FAIL, ESP_OK);
    CHECK_adc_driver_initialization_functions_are_called(ESP_OK, ESP_FAIL);
    CHECK_adc_driver_initialization_functions_are_called(ESP_OK, ESP_OK);

    adc_driver->initialize();
}

static void CHECK_adc_driver_initialization_functions_are_called(int smbus_return_value, int mcp342x_init_return_value)
{
    mock().expectOneCall("smbus_init")
          .withIntParameter("sda", 21)
          .withIntParameter("scl", 22)
          .withIntParameter("i2c_port", 0)
          .withIntParameter("address", ADC_ADDRESS)
          .ignoreOtherParameters()
          .andReturnValue(smbus_return_value);

    mock().expectOneCall("vTaskDelay")
          .withUnsignedIntParameter("ticks", 1);

    mock().expectOneCall("mcp342x_init")
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)&default_config)
          .ignoreOtherParameters()
          .andReturnValue(mcp342x_init_return_value);
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

static void CHECK_mcp342x_set_config(mcp342x_config_t* config);

TEST(ADC_MCP342x_driver_ESP_IDF, set_samples_per_second)
{
    mcp342x_config_t config = default_config;

    config.sample_rate = MCP342X_SRATE_12BIT;
    CHECK_mcp342x_set_config(&config);
    adc_driver->set_samples_per_second(240);

    config.sample_rate = MCP342X_SRATE_14BIT;
    CHECK_mcp342x_set_config(&config);
    adc_driver->set_samples_per_second(60);

    config.sample_rate = MCP342X_SRATE_16BIT;
    CHECK_mcp342x_set_config(&config);
    adc_driver->set_samples_per_second(15);
}

TEST(ADC_MCP342x_driver_ESP_IDF, GIVEN_set_samples_per_second_WHEN_value_to_set_is_invalid_THEN_log_a_message_and_does_not_set_value)
{
    mock().expectNoCall("mcp342x_set_config");

    mock().expectOneCall("ESP_LOGE")
          .withStringParameter("tag", "ADC MCP342x esp-idf")
          .withStringParameter("message", "Error -- samples per second to set is invalid and won't take effect. Please use a valid value.");
    adc_driver->set_samples_per_second(14);
}

static void CHECK_mcp342x_set_config(mcp342x_config_t* config)
{
    mock().expectOneCall("mcp342x_set_config")
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)config)
          .ignoreOtherParameters();
}