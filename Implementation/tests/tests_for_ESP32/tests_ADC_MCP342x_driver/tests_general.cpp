#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <HAL_implementation/HAL_for_ESP32/ADC_driver/ADC_MCP342x_driver/ADC_MCP342x_driver.h>

ADC_MCP342x_driver* adc_driver = nullptr;

TEST_GROUP(ADC_MCP342x_driver)
{
    void setup()
    {
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
    }
};

#define SERIAL_PORT_BAUDRATE    115200UL
#define DEFAULT_ADDRESS 0x69
TEST(ADC_MCP342x_driver, initialize_ADC_driver_with_no_issues_and_INITIALIZE_SERIAL_PORT_to_communicate_errors)
{
    mock().expectOneCall("MCP342x->construct(address)")
          .withUnsignedIntParameter("address", DEFAULT_ADDRESS);
    ADC_MCP342x_driver adc_driver;

    mock().expectOneCall("Serial->begin")
          .withUnsignedIntParameter("baudrate", SERIAL_PORT_BAUDRATE)
          .andReturnValue(true);

    mock().expectOneCall("Wire->begin");

    mock().expectOneCall("MCP342x::generalCallReset");
    mock().expectOneCall("delay")
          .withUnsignedIntParameter("ms", 1);

    const uint8_t address_to_check = DEFAULT_ADDRESS;
    mock().expectOneCall("MCP342x->autoprobe")
          .withMemoryBufferParameter("addressList", (const unsigned char*)&address_to_check, 1)
          .withUnsignedIntParameter("len", 1)
          .andReturnValue(true);

//channel 1;
//resolution 14_bits
//one_shot_mode
//gain 1
    adc_driver.initialize();
}

TEST(ADC_MCP342x_driver, if_begin_fails_then_shows_a_message_and_hangs_the_micro_controller)
{
    mock().expectOneCall("MCP342x->autoprobe")
          .ignoreOtherParameters()
          .andReturnValue(false);
    
    mock().expectOneCall("Serial->println")
          .withStringParameter("msg", "--Sensor_logger-- Failed to initialize external ADC MCP342x. Please, check the I2C connections and restart micro-controller");
    
    mock().ignoreOtherCalls();

    adc_driver->initialize();
}

#define DEFAULT_INPUT_CHANNEL (uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1
TEST(ADC_MCP342x_driver, default_input_channel_is_ADC_DRIVER_CHANNEL_1)
{
    CHECK_EQUAL(DEFAULT_INPUT_CHANNEL, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_MCP342x_driver, set_input_channel)
{
    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_2);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_2, (uint32_t)adc_driver->get_input_channel());

    adc_driver->set_input_channel(adc_channel::ADC_DRIVER_CHANNEL_1);
    CHECK_EQUAL((uint32_t)adc_channel::ADC_DRIVER_CHANNEL_1, (uint32_t)adc_driver->get_input_channel());
}

TEST(ADC_MCP342x_driver, is_conversion_complete)
{
    mock().expectOneCall("MCP342x->read")
          .ignoreOtherParameters();
    mock().expectOneCall("MCP342x::Config->isReady")
          .andReturnValue(true);

    CHECK_TRUE(adc_driver->is_conversion_complete());
    mock().checkExpectations();
    mock().clear();

    mock().expectOneCall("MCP342x->read")
          .ignoreOtherParameters();
    mock().expectOneCall("MCP342x::Config->isReady")
          .andReturnValue(false);

    CHECK_FALSE(adc_driver->is_conversion_complete());
}

#define LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION 250e-6
TEST(ADC_MCP342x_driver, get_measured_voltage)
{
    long READ_VALUE = 75;
    mock().expectOneCall("MCP342x->read")
          .withOutputParameterReturning("result", (void*)&READ_VALUE, sizeof(READ_VALUE))
          .ignoreOtherParameters();
    
    float expected_voltage = READ_VALUE*LEAST_SIGNIFICANT_BIT_VALUE_ON_14_BIT_RESOLUTION;
    DOUBLES_EQUAL(expected_voltage, adc_driver->get_measured_voltage(), 0.00001f);
}

TEST(ADC_MCP342x_driver, start_new_single_reading)
{
    mock().expectOneCall("MCP342x->convert")
          .withMemoryBufferParameter("channel", (const unsigned char*)&MCP342x::channel1, sizeof(MCP342x::Channel))
          .withMemoryBufferParameter("mode", (const unsigned char*)&MCP342x::oneShot, sizeof(MCP342x::Mode))
          .withMemoryBufferParameter("resolution", (const unsigned char*)&MCP342x::resolution14, sizeof(MCP342x::Resolution))
          .withMemoryBufferParameter("gain", (const unsigned char*)&MCP342x::gain1, sizeof(MCP342x::Gain));
    
    adc_driver->start_new_single_reading();
}