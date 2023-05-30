#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class TwoWire {
public:
    TwoWire(uint8_t bus_num) {}
    ~TwoWire() {}

    void begin() {
        mock().actualCall("Wire->begin");
    }

    bool setPins(int sda, int scl) {
        mock().actualCall("TwoWire->setPins")
              .withIntParameter("sda", sda)
              .withIntParameter("scl", scl);
        return mock().returnBoolValueOrDefault(true);
    }
};

extern TwoWire Wire;
extern TwoWire Wire1;
