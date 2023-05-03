#pragma once
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

inline void vTaskDelay(uint32_t ticks) {
    mock().actualCall("vTaskDelay")
          .withUnsignedLongIntParameter("ticks", ticks);
}