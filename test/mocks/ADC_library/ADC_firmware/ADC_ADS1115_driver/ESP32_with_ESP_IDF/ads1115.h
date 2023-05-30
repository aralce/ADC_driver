#pragma once
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef ADS1115_H
#define ADS1115_H

#include <stdio.h>

typedef enum {
    GPIO_NUM_NC = -1,    /*!< Use to signal not connected to S/W */
    GPIO_NUM_0 = 0,     /*!< GPIO0, input and output */
    GPIO_NUM_1 = 1,     /*!< GPIO1, input and output */
    GPIO_NUM_2 = 2,     /*!< GPIO2, input and output */
    GPIO_NUM_3 = 3,     /*!< GPIO3, input and output */
    GPIO_NUM_4 = 4,     /*!< GPIO4, input and output */
    GPIO_NUM_5 = 5,     /*!< GPIO5, input and output */
    GPIO_NUM_6 = 6,     /*!< GPIO6, input and output */
    GPIO_NUM_7 = 7,     /*!< GPIO7, input and output */
    GPIO_NUM_8 = 8,     /*!< GPIO8, input and output */
    GPIO_NUM_9 = 9,     /*!< GPIO9, input and output */
    GPIO_NUM_10 = 10,   /*!< GPIO10, input and output */
    GPIO_NUM_11 = 11,   /*!< GPIO11, input and output */
    GPIO_NUM_12 = 12,   /*!< GPIO12, input and output */
    GPIO_NUM_13 = 13,   /*!< GPIO13, input and output */
    GPIO_NUM_14 = 14,   /*!< GPIO14, input and output */
    GPIO_NUM_15 = 15,   /*!< GPIO15, input and output */
    GPIO_NUM_16 = 16,   /*!< GPIO16, input and output */
    GPIO_NUM_17 = 17,   /*!< GPIO17, input and output */
    GPIO_NUM_18 = 18,   /*!< GPIO18, input and output */
    GPIO_NUM_19 = 19,   /*!< GPIO19, input and output */
    GPIO_NUM_20 = 20,   /*!< GPIO20, input and output */
    GPIO_NUM_21 = 21,   /*!< GPIO21, input and output */
    GPIO_NUM_22 = 22,   /*!< GPIO22, input and output */
    GPIO_NUM_23 = 23,   /*!< GPIO23, input and output */
    GPIO_NUM_25 = 25,   /*!< GPIO25, input and output */
    GPIO_NUM_26 = 26,   /*!< GPIO26, input and output */
    GPIO_NUM_27 = 27,   /*!< GPIO27, input and output */
    GPIO_NUM_28 = 28,   /*!< GPIO28, input and output */
    GPIO_NUM_29 = 29,   /*!< GPIO29, input and output */
    GPIO_NUM_30 = 30,   /*!< GPIO30, input and output */
    GPIO_NUM_31 = 31,   /*!< GPIO31, input and output */
    GPIO_NUM_32 = 32,   /*!< GPIO32, input and output */
    GPIO_NUM_33 = 33,   /*!< GPIO33, input and output */
    GPIO_NUM_34 = 34,   /*!< GPIO34, input mode only */
    GPIO_NUM_35 = 35,   /*!< GPIO35, input mode only */
    GPIO_NUM_36 = 36,   /*!< GPIO36, input mode only */
    GPIO_NUM_37 = 37,   /*!< GPIO37, input mode only */
    GPIO_NUM_38 = 38,   /*!< GPIO38, input mode only */
    GPIO_NUM_39 = 39,   /*!< GPIO39, input mode only */
    GPIO_NUM_MAX,
} gpio_num_t;

typedef enum { // multiplex options
  ADS1115_MUX_0_1 = 0,
  ADS1115_MUX_0_3,
  ADS1115_MUX_1_3,
  ADS1115_MUX_2_3,
  ADS1115_MUX_0_GND,
  ADS1115_MUX_1_GND,
  ADS1115_MUX_2_GND,
  ADS1115_MUX_3_GND,
} ads1115_mux_t;

typedef enum { // full-scale resolution options
  ADS1115_FSR_6_144 = 0,
  ADS1115_FSR_4_096,
  ADS1115_FSR_2_048,
  ADS1115_FSR_1_024,
  ADS1115_FSR_0_512,
  ADS1115_FSR_0_256,
} ads1115_fsr_t;

typedef enum { // samples per second
  ADS1115_SPS_8 = 0,
  ADS1115_SPS_16,
  ADS1115_SPS_32,
  ADS1115_SPS_64,
  ADS1115_SPS_128,
  ADS1115_SPS_250,
  ADS1115_SPS_475,
  ADS1115_SPS_860,
  ADS1115_MAX_SPS
} ads1115_sps_t;

typedef enum {
  ADS1115_MODE_CONTINUOUS = 0,
  ADS1115_MODE_SINGLE
} ads1115_mode_t;

typedef union { // configuration register
  struct {
    uint16_t COMP_QUE:2;  // bits 0..  1  Comparator queue and disable
    uint16_t COMP_LAT:1;  // bit  2       Latching Comparator
    uint16_t COMP_POL:1;  // bit  3       Comparator Polarity
    uint16_t COMP_MODE:1; // bit  4       Comparator Mode
    uint16_t DR:3;        // bits 5..  7  Data rate
    uint16_t MODE:1;      // bit  8       Device operating mode
    uint16_t PGA:3;       // bits 9..  11 Programmable gain amplifier configuration
    uint16_t MUX:3;       // bits 12.. 14 Input multiplexer configuration
    uint16_t OS:1;        // bit  15      Operational status or single-shot conversion start
  } bit;
  uint16_t reg;
} ADS1115_CONFIG_REGISTER_Type;

typedef struct {
    int dummy;
} ads1115_t;

typedef uint32_t TickType_t;

// initialize device
inline ads1115_t ads1115_config(int i2c_port, int sda, int scl, uint8_t address) // set up configuration
{
    mock().actualCall("ads1115_config")
          .withIntParameter("i2c_port", i2c_port)
          .withIntParameter("sda", sda)
          .withIntParameter("scl", scl)
          .withUnsignedIntParameter("address", address);
    ads1115_t return_value;
    return return_value;
}

// set configuration
inline void ads1115_set_mux(ads1115_t* ads, ads1115_mux_t mux) // set multiplexer
{
    mock().actualCall("ads1115_set_mux")
          .withPointerParameter("ads", ads)
          .withIntParameter("mux", (int)mux);
}

inline void ads1115_set_pga(ads1115_t* ads, ads1115_fsr_t fsr) // set fsr
{
    mock().actualCall("ads1115_set_pga")
          .withPointerParameter("ads", ads)
          .withIntParameter("fsr", fsr);
}

inline void ads1115_set_mode(ads1115_t* ads, ads1115_mode_t mode) // set read mode
{
    mock().actualCall("ads1115_set_mode")
          .withPointerParameter("ads", ads)
          .withIntParameter("mode", mode);
}

inline void ads1115_set_sps(ads1115_t* ads, ads1115_sps_t sps) // set sampling speed
{
    mock().actualCall("ads1115_set_sps")
          .withPointerParameter("ads", ads)
          .withIntParameter("sps", sps);
}

inline void ads1115_set_max_ticks(ads1115_t* ads, TickType_t max_ticks) // maximum wait ticks for i2c bus
{
    mock().actualCall("ads1115_set_max_ticks")
          .withPointerParameter("ads", ads)
          .withUnsignedLongIntParameter("max_ticks", max_ticks);
}

inline void ads1115_startConversion(ads1115_t* ads) {
    mock().actualCall("ads1115_startConversion")
          .withPointerParameter("ads", ads);
}

inline int16_t ads1115_get_raw(ads1115_t* ads) // get voltage in bits
{
    mock().actualCall("ads1115_get_raw")
          .withPointerParameter("ads", ads);
    return mock().returnIntValueOrDefault(0);
}

inline double ads1115_get_voltage(ads1115_t* ads) // get voltage in volts
{
    mock().actualCall("ads1115_get_voltage")
          .withPointerParameter("ads", ads);
    return mock().returnDoubleValueOrDefault(0);
}

inline bool ads1115_conversionComplete(ads1115_t* ads)
{
    mock().actualCall("ads1115_conversionComplete")
          .withPointerParameter("ads", ads);
    return mock().returnBoolValueOrDefault(false);
}

#endif // ifdef ADS1115_H

#ifdef __cplusplus
}
#endif
