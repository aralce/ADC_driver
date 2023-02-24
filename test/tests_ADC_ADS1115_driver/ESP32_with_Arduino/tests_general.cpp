#include <Implementation/ADC_ADS1115_driver/ESP32_Arduino/ADC_ADS1115_driver.h>
#include <Arduino.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

ADC_driver_api* adc_driver = nullptr;
#define DEFAULT_INPUT_CHANNEL   (uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1

TEST_GROUP(ADC_ADS1115_driver_esp32_arduino)
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

#define SERIAL_PORT_BAUDRATE    115200UL
TEST(ADC_ADS1115_driver_esp32_arduino, initialize_ADC_driver_with_no_issues_and_INITIALIZE_SERIAL_PORT_to_communicate_errors)
{
    mock().expectOneCall("Serial->begin")
          .withUnsignedIntParameter("baudrate", SERIAL_PORT_BAUDRATE)
          .andReturnValue(true);
    
    mock().expectOneCall("Adafruit_ADS1115->setGain")
          .withUnsignedIntParameter("gain", GAIN_ONE);
    
    mock().expectOneCall("Adafruit_ADS1115->begin");

    adc_driver->initialize();
}

TEST(ADC_ADS1115_driver_esp32_arduino, if_begin_fails_then_shows_a_message_and_hangs_the_micro_controller)
{
    mock().expectOneCall("Adafruit_ADS1115->begin")
          .andReturnValue(false);
    
    mock().expectOneCall("Serial->println")
          .withStringParameter("msg", "--Sensor_logger-- Failed to initialize external ADC ads1x15. Please, check the I2C connections and restart micro-controller.");
    
    mock().ignoreOtherCalls();

    adc_driver->initialize();
}

TEST(ADC_ADS1115_driver_esp32_arduino, default_input_channel_is_ADC_DRIVER_CHANNEL_1)
{
    CHECK_EQUAL(DEFAULT_INPUT_CHANNEL, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_ADS1115_driver_esp32_arduino, set_input_channel){
    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_1);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1, (uint32_t)adc_driver->get_input_channel());

    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_2);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_2, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_ADS1115_driver_esp32_arduino, is_conversion_complete)
{
    mock().expectOneCall("Adafruit_ADS1115->conversionComplete")
          .andReturnValue(true);
    CHECK_TRUE(adc_driver->is_conversion_complete());

    mock().expectOneCall("Adafruit_ADS1115->conversionComplete")
          .andReturnValue(false);
    CHECK_FALSE(adc_driver->is_conversion_complete());
}

TEST(ADC_ADS1115_driver_esp32_arduino, get_measured_voltage)
{
    const int16_t EXPECTED_RAW_VALUE = 1024;
    const float EXPECTED_VOLTAGE = 22.5;
    mock().expectOneCall("Adafruit_ADS1115->getLastConversionResults")
          .andReturnValue(EXPECTED_RAW_VALUE);
    mock().expectOneCall("Adafruit_ADS1115->computeVolts")
          .withIntParameter("counts", EXPECTED_RAW_VALUE)
          .andReturnValue(EXPECTED_VOLTAGE);
    
    CHECK_EQUAL(EXPECTED_VOLTAGE, adc_driver->get_measured_voltage());
}

TEST(ADC_ADS1115_driver_esp32_arduino, start_new_single_reading_when_channel_is_ADC_DRIVER_CHANNEL_1)
{
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1, (uint32_t)adc_driver->get_input_channel());

    mock().expectOneCall("Adafruit_ADS1115->startADCReading")
          .withUnsignedIntParameter("mux", ADC_CHANNEL_2)
          .withBoolParameter("continuous", false);
    
    adc_driver->start_new_single_reading();
}

TEST(ADC_ADS1115_driver_esp32_arduino, start_new_single_reading_when_channel_is_ADC_DRIVER_CHANNEL_2)
{
    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_2);

    mock().expectOneCall("Adafruit_ADS1115->startADCReading")
          .withUnsignedIntParameter("mux", ADC_CHANNEL_1)
          .withBoolParameter("continuous", false);
    
    adc_driver->start_new_single_reading();
}