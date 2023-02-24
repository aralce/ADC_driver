#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#define ESP_LOGE(tag, message) error_log(TAG, message)

inline void error_log(char* tag, const char* message) {
    mock().actualCall("ESP_LOGE")
          .withStringParameter("tag", tag)
          .withStringParameter("message", message);
}