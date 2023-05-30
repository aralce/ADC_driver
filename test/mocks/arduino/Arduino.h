#pragma once
#include <cstdint>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <string>

#define LOW               0x0
#define HIGH              0x1

//GPIO FUNCTIONS
#define INPUT             0x01
#define OUTPUT            0x02
#define PULLUP            0x04
#define INPUT_PULLUP      0x05
#define PULLDOWN          0x08
#define INPUT_PULLDOWN    0x09

float min(float x, float y);
float max(float x, float y);

//PWM
double ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits);
void ledcWrite(uint8_t channel, uint32_t duty);
void ledcAttachPin(uint8_t pin, uint8_t channel);

uint16_t analogRead(uint8_t pin);
void analogReadResolution(uint8_t bits);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);

uint32_t micros();
void set_fake_micros(uint32_t microseconds);
uint32_t millis();
void set_fake_millis(uint32_t milliseconds);

void delay(unsigned int ms);

class Serial_class {
public:
    bool begin(uint32_t baudrate);
    void println(const char* msg);
};

class ESP_class {
public:
    void restart() {
        mock().actualCall("ESP_class->restart");
    }
};

// class Wire_class {
// public:
//     void begin();
// };

// extern Wire_class Wire;
extern Serial_class Serial;
extern ESP_class ESP;

