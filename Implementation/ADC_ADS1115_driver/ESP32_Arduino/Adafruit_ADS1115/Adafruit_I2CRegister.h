#if defined(ESP32_WITH_ARDUINO) 

#ifndef _ADAFRUIT_I2C_REGISTER_H_
#define _ADAFRUIT_I2C_REGISTER_H_

#include <Adafruit_BusIO_Register.h>
#include <Arduino.h>

typedef Adafruit_BusIO_Register Adafruit_I2CRegister;
typedef Adafruit_BusIO_RegisterBits Adafruit_I2CRegisterBits;

#endif

#endif //ESP32_WITH_ARDUINO