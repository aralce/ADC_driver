#include <Arduino.h>

float min(float x, float y) {
    return x < y ? x : y;
}

float max(float x, float y) {
    return x > y ? x : y;
}

//PWM
double ledcSetup(uint8_t channel, double freq, uint8_t resolution_bits) {
    mock().actualCall("ledcSetup")
          .withUnsignedIntParameter("channel", channel)
          .withDoubleParameter("freq", freq)
          .withUnsignedIntParameter("resolution_bits", resolution_bits);
    return mock().returnDoubleValueOrDefault(0);
}

void ledcWrite(uint8_t channel, uint32_t duty) {
    mock().actualCall("ledcWrite")
          .withUnsignedIntParameter("channel", channel)
          .withUnsignedLongIntParameter("duty", duty);
}

void ledcAttachPin(uint8_t pin, uint8_t channel) {
    mock().actualCall("ledcAttachPin")
          .withUnsignedIntParameter("pin", pin)
          .withUnsignedIntParameter("channel", channel);
}

uint16_t analogRead(uint8_t pin) {
    mock().actualCall("analogRead")
          .withUnsignedIntParameter("pin", pin);
    return mock().returnUnsignedIntValueOrDefault(0);
}

void analogReadResolution(uint8_t bits) {
    mock().actualCall("analogReadResolution")
          .withUnsignedIntParameter("bits", bits);
}

void pinMode(uint8_t pin, uint8_t mode) {
    mock().actualCall("pinMode")
          .withUnsignedIntParameter("pin", pin)
          .withUnsignedIntParameter("mode", mode);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    mock().actualCall("digitalWrite")
          .withUnsignedIntParameter("pin", pin)
          .withUnsignedIntParameter("val", val);
}

uint32_t microseconds_since_system_was_initialized = 0;

uint32_t micros() {return microseconds_since_system_was_initialized;}

void set_fake_micros(uint32_t microseconds) {
    microseconds_since_system_was_initialized = microseconds;
}

uint32_t milliseconds_since_system_was_initialized = 0;

uint32_t millis() {
    return milliseconds_since_system_was_initialized;
}

void set_fake_millis(uint32_t milliseconds) {
    milliseconds_since_system_was_initialized = milliseconds;
}

void delay(unsigned int ms) {
    mock().actualCall("delay")
          .withUnsignedIntParameter("ms", ms);
}

bool Serial_class::begin(uint32_t baudrate) {
    mock().actualCall("Serial->begin")
          .withUnsignedIntParameter("baudrate", baudrate);
    return mock().returnBoolValueOrDefault(true);
}

void Serial_class::println(const char* msg) {
    mock().actualCall("Serial->println")
          .withStringParameter("msg", msg);
}

void Wire_class::begin() {
    mock().actualCall("Wire->begin");
}

Wire_class Wire;
Serial_class Serial;
ESP_class ESP;