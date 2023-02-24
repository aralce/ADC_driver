#include <Implementation/ADC_ADS1115_driver/ESP32_ESP_IDF/ADC_ADS1115_driver.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#define ADS1X15_ADDRESS (0x48) ///< 1001 000 (ADDR = GND)
ADC_driver_api* adc_driver = nullptr;
#define DEFAULT_INPUT_CHANNEL   (uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1

TEST_GROUP(ADC_ADS1115_driver_esp32_esp_idf)
{
    void setup()
    {
        adc_driver = new ADC_ADS1115_driver;
    }
    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
        if (adc_driver != nullptr) {
            delete adc_driver;
            adc_driver = nullptr;
        }
    }
};

TEST(ADC_ADS1115_driver_esp32_esp_idf, initialize_ADC_driver) {
    const int I2C_PORT = 0;
    mock().expectOneCall("ads1115_config")
          .withIntParameter("i2c_port", I2C_PORT)
          .withUnsignedIntParameter("address", ADS1X15_ADDRESS);
    
    mock().expectOneCall("ads1115_set_mux")
          .withIntParameter("mux", ADS1115_MUX_0_GND)
          .ignoreOtherParameters();

    mock().expectOneCall("ads1115_set_pga")
          .withIntParameter("fsr", ADS1115_FSR_4_096)
          .ignoreOtherParameters();

    mock().expectOneCall("ads1115_set_mode")
          .withIntParameter("mode", ADS1115_MODE_SINGLE)
          .ignoreOtherParameters();

    mock().expectOneCall("ads1115_set_sps")
          .withIntParameter("sps", ADS1115_SPS_128)
          .ignoreOtherParameters();

    adc_driver->initialize();
}

TEST(ADC_ADS1115_driver_esp32_esp_idf, default_input_channel_is_ADC_DRIVER_CHANNEL_1)
{
    CHECK_EQUAL(DEFAULT_INPUT_CHANNEL, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_ADS1115_driver_esp32_esp_idf, set_input_channel){
    mock().expectOneCall("ads1115_set_mux")
          .withIntParameter("mux", ADS1115_MUX_0_GND)
          .ignoreOtherParameters();
    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_1);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1, (uint32_t)adc_driver->get_input_channel());

    mock().expectOneCall("ads1115_set_mux")
          .withIntParameter("mux", ADS1115_MUX_1_GND)
          .ignoreOtherParameters();
    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_2);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_2, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_ADS1115_driver_esp32_esp_idf, is_conversion_complete)
{
    mock().expectOneCall("ads1115_conversionComplete")
          .ignoreOtherParameters()
          .andReturnValue(true);
    CHECK_TRUE(adc_driver->is_conversion_complete());

    mock().expectOneCall("ads1115_conversionComplete")
          .ignoreOtherParameters()
          .andReturnValue(false);
    CHECK_FALSE(adc_driver->is_conversion_complete());
}

TEST(ADC_ADS1115_driver_esp32_esp_idf, get_measured_voltage)
{
    const float EXPECTED_VOLTAGE = 22.5;
    mock().expectOneCall("ads1115_get_voltage")
          .ignoreOtherParameters()
          .andReturnValue(EXPECTED_VOLTAGE);
    
    CHECK_EQUAL(EXPECTED_VOLTAGE, adc_driver->get_measured_voltage());
}

TEST(ADC_ADS1115_driver_esp32_esp_idf, start_new_single_reading_when_channel_is_ADC_DRIVER_CHANNEL_1)
{
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1, (uint32_t)adc_driver->get_input_channel());

    mock().expectOneCall("ads1115_startConversion")
          .ignoreOtherParameters();
    
    adc_driver->start_new_single_reading();
}