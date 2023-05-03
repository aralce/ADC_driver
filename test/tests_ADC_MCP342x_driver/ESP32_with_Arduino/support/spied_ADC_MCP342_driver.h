#pragma once
#include <Implementation/ADC_MCP342x_driver/ESP32_Arduino/ADC_MCP342x_driver.h>

class Spied_ADC_MCP342_driver : public ADC_MCP342x_driver {
public:
    int spy_resolution() {
        return (int)resolution;
    }
};