//Mock of Adafruit_ADS1115 library by Adafruit

#pragma once
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#define GAIN_ONE 512

#define ADS1X15_REG_CONFIG_MUX_SINGLE_0 (0x4000) ///< Single-ended AIN0
#define ADS1X15_REG_CONFIG_MUX_SINGLE_1 (0x5000) ///< Single-ended AIN1
#define ADS1X15_REG_CONFIG_MUX_SINGLE_2 (0x6000) ///< Single-ended AIN2
#define ADS1X15_REG_CONFIG_MUX_SINGLE_3 (0x7000) ///< Single-ended AIN3

constexpr uint16_t MUX_BY_CHANNEL[] = {
    ADS1X15_REG_CONFIG_MUX_SINGLE_0, ///< Single-ended AIN0
    ADS1X15_REG_CONFIG_MUX_SINGLE_1, ///< Single-ended AIN1
    ADS1X15_REG_CONFIG_MUX_SINGLE_2, ///< Single-ended AIN2
    ADS1X15_REG_CONFIG_MUX_SINGLE_3  ///< Single-ended AIN3
};                                   ///< MUX config by channel

class Adafruit_ADS1115 {
public:
    int16_t readADC_SingleEnded(uint8_t channel) {
        mock().actualCall("Adafruit_ADS1115->readADC_SingleEnded")
              .withUnsignedIntParameter("channel", channel);
        return (int16_t)mock().unsignedIntReturnValue();
    }

    void setGain(uint32_t gain) {
        mock().actualCall("Adafruit_ADS1115->setGain")
              .withUnsignedIntParameter("gain", gain);
    }

    bool begin() {
        mock().actualCall("Adafruit_ADS1115->begin");
        return mock().returnBoolValueOrDefault(true);
    }

    float computeVolts(int16_t counts) {
        mock().actualCall("Adafruit_ADS1115->computeVolts")
              .withIntParameter("counts", counts);
        return (float)mock().returnDoubleValueOrDefault(0);
    }

    void startADCReading(uint16_t mux, bool continuous) {
        mock().actualCall("Adafruit_ADS1115->startADCReading")
              .withUnsignedIntParameter("mux", mux)
              .withBoolParameter("continuous", continuous);
    }

    bool conversionComplete() {
        mock().actualCall("Adafruit_ADS1115->conversionComplete");
        return mock().returnBoolValueOrDefault(false);
    }

    int16_t getLastConversionResults() {
        mock().actualCall("Adafruit_ADS1115->getLastConversionResults");
        return mock().returnIntValueOrDefault(0);
    }
};