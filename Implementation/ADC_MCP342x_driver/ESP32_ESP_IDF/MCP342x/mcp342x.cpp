/*
    Craft Metrics

    This product includes software developed by
    Craft Metrics (https://craftmetrics.ca/).

    MIT License
    Copyright (c) 2018 Craft Metrics

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "mcp342x.h"

#include <string.h>
#include <esp_log.h>
#include "smbus.h"

static const char *TAG = "mcp342x";

/*-----------------------------------------------------------
* I2C C API
*----------------------------------------------------------*/
static bool _is_init(const mcp342x_info_t *mcp342x_info_ptr)
{
    bool ok = false;
    if (mcp342x_info_ptr != NULL)
    {
        if (mcp342x_info_ptr->init)
        {
            ok = true;
        }
        else
        {
            ESP_LOGE(TAG, "mcp342x_info is not initialised");
        }
    }
    else
    {
        ESP_LOGE(TAG, "mcp342x_info is NULL");
    }
    return ok;
}

/*-----------------------------------------------------------
* PUBLIC C API
*----------------------------------------------------------*/
mcp342x_info_t *mcp342x_malloc(void)
{
    mcp342x_info_t *mcp342x_info = (mcp342x_info_t *)malloc(sizeof(*mcp342x_info));
    if (mcp342x_info != NULL)
    {
        memset(mcp342x_info, 0, sizeof(*mcp342x_info));
        ESP_LOGD(TAG, "malloc mcp342x_info_t %p", mcp342x_info);
    }
    else
    {
        ESP_LOGE(TAG, "malloc mcp342x_info failed");
    }
    return mcp342x_info;
}

void mcp342x_free(mcp342x_info_t **mcp342x_info_ptr_ptr)
{
    if (mcp342x_info_ptr_ptr != NULL && (*mcp342x_info_ptr_ptr != NULL))
    {
        (*mcp342x_info_ptr_ptr)->init = false;

        ESP_LOGD(TAG, "free mcp342x_info_t %p", *mcp342x_info_ptr_ptr);
        free(*mcp342x_info_ptr_ptr);
        *mcp342x_info_ptr_ptr = NULL;
    }
    else
    {
        ESP_LOGE(TAG, "free mcp342x_info_t failed");
    }
}

esp_err_t mcp342x_init(mcp342x_info_t *mcp342x_info_ptr, smbus_info_t *smbus_info_ptr, mcp342x_config_t in_config)
{
    esp_err_t err = ESP_FAIL;
    if (mcp342x_info_ptr != NULL)
    {
        mcp342x_info_ptr->init = true;

        ESP_LOGD(TAG, "init mcp342x_info_t %p", mcp342x_info_ptr);
        ESP_LOGD(TAG, "config.conversion_mode = 0x%02x", in_config.conversion_mode);
        ESP_LOGD(TAG, "config.channel = 0x%02x", in_config.channel);
        ESP_LOGD(TAG, "config.gain = 0x%02x", in_config.gain);
        ESP_LOGD(TAG, "config.sample_rate = 0x%02x", in_config.sample_rate);
        mcp342x_info_ptr->smbus_info = smbus_info_ptr;
        mcp342x_info_ptr->config = ((in_config.conversion_mode & MCP342X_MODE_MASK) |
                                    (in_config.channel & MCP342X_CHANNEL_MASK) |
                                    (in_config.gain & MCP342X_GAIN_MASK) |
                                    (in_config.sample_rate & MCP342X_SRATE_MASK));
        // Test connection
        ESP_LOGD(TAG, "send mcp342x_info config 0x%02x", mcp342x_info_ptr->config);
        err = smbus_send_byte(smbus_info_ptr, mcp342x_info_ptr->config);
    }
    else
    {
        ESP_LOGE(TAG, "mcp342x_info is NULL");
    }
    return err;
}

void mcp342x_set_config(mcp342x_info_t *mcp342x_info_ptr, mcp342x_config_t in_config)
{
    mcp342x_info_ptr->config = (in_config.channel |
                                in_config.conversion_mode |
                                in_config.gain |
                                in_config.sample_rate);
    return;
}

esp_err_t mcp342x_general_call(const mcp342x_info_t *mcp342x_info_ptr, mcp342x_general_call_t call)
{
    return smbus_write_byte(mcp342x_info_ptr->smbus_info, MCP342X_GC_START, call);
}

esp_err_t mcp342x_start_new_conversion(const mcp342x_info_t *mcp342x_info_ptr)
{
    esp_err_t err = ESP_FAIL;
    if (_is_init(mcp342x_info_ptr))
    {
        err = smbus_send_byte(mcp342x_info_ptr->smbus_info, mcp342x_info_ptr->config | MCP342X_CNTRL_TRIGGER_CONVERSION);
    }
    return err;
}

mcp342x_conversion_status_t mcp342x_read_voltage(const mcp342x_info_t *mcp342x_info_ptr, double *result)
{
    int32_t i32;
    uint8_t buffer[5] = {};

    smbus_i2c_read_block(mcp342x_info_ptr->smbus_info, mcp342x_info_ptr->config, buffer, 5);
    ESP_LOGV(TAG, "%02x %02x %02x %02x", buffer[0], buffer[1], buffer[2], buffer[3]);

    ESP_LOGI(TAG, "Conversion done");
    ESP_LOGD(TAG, "Buffer: %02x %02x %02x | %02x %02x",buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);

    /**
     * Choose the LSB voltage value and discard repeated MSB
     */
    double LSB = 0;
    uint8_t MSB = 0x80 & buffer[0];
    switch ((mcp342x_info_ptr->config & MCP342X_SRATE_MASK))
    {
    case MCP342X_SRATE_12BIT:
    {
        LSB = 0.001;
        buffer[0] &= 0x0F;
        break;
    }
    case MCP342X_SRATE_14BIT:
    {
        LSB = 0.000250;
        buffer[0] &= 0x3F;
        break;
    }
    case MCP342X_SRATE_16BIT:
    {
        LSB = 0.0000625;
        buffer[0] &= 0xFF;
        break;
    }
    case MCP342X_SRATE_18BIT:
    {
        LSB = 0.000015625;
        buffer[0] &= 0x3F;
        break;
    }
    }

    /**
     * Arrage bytes to form output code
     * MSB <| Byte1 | Byte2 |[ Byte3 ]|> LSB
     */
    if ((mcp342x_info_ptr->config & MCP342X_SRATE_MASK) == MCP342X_SRATE_18BIT)
    {
        i32 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    }
    else
    {
        i32 = (buffer[0] << 8) | buffer[1]; // MSB / byte1 | byte2 / LSB
    }

    ESP_LOGD(TAG, "i32: %08x | %d", i32, i32);

    /**
     * Based on the sign, calculate the input voltage signal
     * MSB = 0 (Positive) 
     * MSB = 1 (Negative)
     */
    if (MSB == 0)
    {
        *result = i32 * (LSB / (1 << (mcp342x_info_ptr->config & MCP342X_GAIN_MASK)));
    }
    else
    {
        *result = (~(i32) + 1) * (LSB / (1 << (mcp342x_info_ptr->config & MCP342X_GAIN_MASK)));
    }

    if (*result > 2.048)
    {
        return MCP342X_STATUS_OVERFLOW;
    }
    if (*result < -2.048)
    {
        return MCP342X_STATUS_UNDERFLOW;
    }

    return MCP342X_STATUS_OK;
}

bool is_mcp342x_conversion_complete(const mcp342x_info_t *mcp342x_info_ptr) {
    uint8_t buffer[5] = {};

    esp_err_t err = smbus_i2c_read_block(mcp342x_info_ptr->smbus_info, mcp342x_info_ptr->config, buffer, 5);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error in is_mcp342x_conversion_complete: %s\r\n", esp_err_to_name(err));
        return false;
    }
    ESP_LOGV(TAG, "%02x %02x %02x %02x", buffer[0], buffer[1], buffer[2], buffer[3]);
    
    return (buffer[3] & MCP342X_CNTRL_MASK) == MCP342X_CNTRL_RESULT_UPDATED;
}