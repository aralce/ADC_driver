#pragma once

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <stdint.h>
#include "smbus.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*-----------------------------------------------------------
* MACROS & ENUMS
*----------------------------------------------------------*/

/** I2C Address of device
 * MCP3421, MCP3425 & MCP3426 are factory programed for any of 0x68 thru 0x6F
 * MCP3422, MCP3423, MCP3424, MCP3427 & MCP3428 addresses are controlled by address lines A0 and A1
 * each address line can be low (GND), high (VCC) or floating (FLT)
 */
typedef enum MCP342xAddress
{
    MCP342X_A0GND_A1GND = 0x68,
    MCP342X_A0GND_A1FLT = 0x69,
    MCP342X_A0GND_A1VCC = 0x6A,
    MCP342X_A0FLT_A1GND = 0x6B,
    MCP342X_A0VCC_A1GND = 0x6C,
    MCP342X_A0VCC_A1FLT = 0x6D,
    MCP342X_A0VCC_A1VCC = 0x6E,
    MCP342X_A0FLT_A1VCC = 0x6F,
} mcp342x_address_t;

/** Ready Bit definitions (bit 7)
 */
typedef enum MCP342xControl
{
    // Write in OneShot Mode
    MCP342X_CNTRL_NO_EFFECT = 0x00,
    MCP342X_CNTRL_TRIGGER_CONVERSION = 0x80,
    // Reading from register
    MCP342X_CNTRL_RESULT_NOT_UPDATED = 0x80,
    MCP342X_CNTRL_RESULT_UPDATED = 0x00,
    // Mask to get Bit 7
    MCP342X_CNTRL_MASK = 0x80,
} mcp342x_control_t;

/** Channel definitions (bit 6-5)
 * MCP3421 & MCP3425 have only the one channel and ignore this param
 * MCP3422, MCP3423, MCP3426 & MCP3427 have two channels and treat 3 & 4 as repeats of 1 & 2 respectively
 * MCP3424 & MCP3428 have all four channels
 */
typedef enum MCP342xChannel
{
    MCP342X_CHANNEL_1 = 0x00, // 0b 0000 0000
    MCP342X_CHANNEL_2 = 0x20, // 0b 0010 0000
    MCP342X_CHANNEL_3 = 0x40, // 0b 0100 0000
    MCP342X_CHANNEL_4 = 0x60, // 0b 0110 0000
    MCP342X_CHANNEL_MASK = 0x60,
} mcp342x_channel_t;

/** Conversion mode definitions (bit 4)
 */
typedef enum MCP342xConversionMode
{
    MCP342X_MODE_ONESHOT = 0x00,    // 0b 0000 0000
    MCP342X_MODE_CONTINUOUS = 0x10, // 0b 0001 0000
    MCP342X_MODE_MASK = 0x10,
} mcp342x_conversion_mode_t;

/** Sample size definitions (bit 3-2)
 * these also affect the sampling rate
 * 12-bit has a max sample rate of 240sps
 * 14-bit has a max sample rate of  60sps
 * 16-bit has a max sample rate of  15sps
 * 18-bit has a max sample rate of   3.75sps (MCP3421, MCP3422, MCP3423, MCP3424 only)
 */
typedef enum MCP342xSampleRate
{
    MCP342X_SRATE_12BIT = 0x00, // 0b 0000 0000
    MCP342X_SRATE_14BIT = 0x04, // 0b 0000 0100
    MCP342X_SRATE_16BIT = 0x08, // 0b 0000 1000
    MCP342X_SRATE_18BIT = 0x0C, // 0b 0000 1100
    MCP342X_SRATE_MASK = 0x0C,
} mcp342x_sample_rate_t;

/** Programmable Gain PGA definitions (bit 1-0)
 */
typedef enum MCP342xGain
{
    MCP342X_GAIN_1X = 0x00, // 0b 0000 0000
    MCP342X_GAIN_2X = 0x01, // 0b 0000 0001
    MCP342X_GAIN_4X = 0x02, // 0b 0000 0010
    MCP342X_GAIN_8X = 0x03, // 0b 0000 0011
    MCP342X_GAIN_MASK = 0x03,
} mcp342x_gain_t;

typedef enum MCP342xGeneralCall
{
    MCP342X_GC_START = 0x00,
    MCP342X_GC_LATCH = 0x04,
    MCP342X_GC_RESET = 0x06,
    MCP342X_GC_CONVERSION = 0x08
} mcp342x_general_call_t;

typedef enum MCP342xConvStatus
{
    MCP342X_STATUS_OK,
    MCP342X_STATUS_UNDERFLOW,
    MCP342X_STATUS_OVERFLOW,
    MCP342X_STATUS_I2C,
    MCP342X_STATUS_IN_PROGRESS,
    MCP342X_STATUS_TIMEOUT
} mcp342x_conversion_status_t;

/** Configuration Register values of the MCP342x device
 * Initialized with default settings partially according to
 * datasheet Section 4.1
 */
typedef struct MCP342xConfig
{
    mcp342x_channel_t channel;
    mcp342x_conversion_mode_t conversion_mode;
    mcp342x_sample_rate_t sample_rate;
    mcp342x_gain_t gain;
} mcp342x_config_t;

/** Struct for controlling a MCP342x device
 * smbus_info contains the i2c address of the device
 */
typedef struct MCP342xInfo_t
{
    bool init : 1;
    smbus_info_t *smbus_info;
    uint8_t config;
} mcp342x_info_t;

/*-----------------------------------------------------------
* DEFINITIONS
*----------------------------------------------------------*/

inline int mcp342x_init(mcp342x_info_t *mcp342x_info, smbus_info_t *smbus_info, mcp342x_config_t in_config) {
    mock().actualCall("mcp342x_init")
          .withPointerParameter("mcp342x_info", (void*)mcp342x_info)
          .withPointerParameter("smbus_info", smbus_info)
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)&in_config);
    return mock().returnIntValueOrDefault(0);
}

inline void mcp342x_set_config(mcp342x_info_t *mcp342x_info_ptr, mcp342x_config_t in_config) {
    mock().actualCall("mcp342x_set_config")
          .withPointerParameter("mcp342x_info_ptr", (void*)mcp342x_info_ptr)
          .withParameterOfType("mcp342x_config_t", "in_config", (const void*)&in_config);
}

inline int mcp342x_start_new_conversion(const mcp342x_info_t *mcp342x_info_ptr) {
    mock().actualCall("mcp342x_start_new_conversion")
          .withPointerParameter("mcp342x_info_t", (void*)mcp342x_info_ptr);
    return 0;
}

inline mcp342x_conversion_status_t mcp342x_read_voltage(const mcp342x_info_t *mcp342x_info_ptr, double  *result) {
    mock().actualCall("mcp342x_read_voltage")
          .withPointerParameter("mcp342x_info_ptr", (void*)mcp342x_info_ptr)
          .withOutputParameter("result", (void*)result);
    return (mcp342x_conversion_status_t)mock().returnIntValueOrDefault(0);
}

inline bool is_mcp342x_conversion_complete(const mcp342x_info_t *mcp342x_info_ptr) {
    mock().actualCall("is_mcp342x_conversion_complete")
          .withPointerParameter("mcp342x_info_ptr", (void*)mcp342x_info_ptr);
    return mock().returnBoolValueOrDefault(false);
}

class Mcp342x_config_t_comparator : public MockNamedValueComparator
{
public:
    virtual bool isEqual(const void* object1, const void* object2)
    {
        const mcp342x_config_t* config1 = static_cast<const mcp342x_config_t*>(object1);
        const mcp342x_config_t* config2 = static_cast<const mcp342x_config_t*>(object2);

        if (config1->channel != config2->channel) {
            printf("channel mismatch: %d  vs %d\r\n", config1->channel, config2->channel);
            return false;
        }
        if (config1->conversion_mode != config2->conversion_mode) {
            printf("conversion mode mismatch: %d  vs %d\r\n", config1->conversion_mode, config2->conversion_mode);
            return false;
        }
        if (config1->gain != config2->gain) {
            printf("gain mismatch: %d vs %d\r\n", config1->gain, config2->gain);
            return false;
        }
        if (config1->sample_rate != config2->sample_rate) {
            printf("sample_rate mismatch: %d vs %d\r\n", config1->sample_rate, config2->sample_rate);
            return false;
        }
        
        return true;
    }
    virtual SimpleString valueToString(const void* object)
    {
        return StringFrom(object);
    }
};


#ifdef __cplusplus
}
#endif