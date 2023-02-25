#pragma once
#include <stdint.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SMBUS_DEFAULT_TIMEOUT (1000 / portTICK_RATE_MS)  ///< Default transaction timeout in ticks
/**
 * @brief 7-bit or 10-bit I2C slave address.
 */
typedef uint16_t i2c_address_t;

/**
 * @brief Structure containing information related to the SMBus protocol.
 */
typedef struct
{
    int dummy;
} smbus_info_t;

inline int smbus_init(smbus_info_t * smbus_info, int i2c_port, i2c_address_t address) {
    mock().actualCall("smbus_init")
          .withPointerParameter("smbus_info", smbus_info)
          .withIntParameter("i2c_port", i2c_port)
          .withIntParameter("address", address);
    return mock().returnIntValueOrDefault(0);
}


#ifdef __cplusplus
}
#endif
