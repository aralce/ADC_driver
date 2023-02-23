#include "MCP342x.h"

const MCP342x::Channel MCP342x::channel1 = Channel(0x00);
const MCP342x::Channel MCP342x::channel2 = Channel(0x20);
const MCP342x::Channel MCP342x::channel3 = Channel(0x40);
const MCP342x::Channel MCP342x::channel4 = Channel(0x60);

const MCP342x::Mode MCP342x::oneShot = Mode(0x00);
const MCP342x::Mode MCP342x::continuous = Mode(0x10);

const MCP342x::Resolution MCP342x::resolution12 = Resolution(0x00);
const MCP342x::Resolution MCP342x::resolution14 = Resolution(0x04);
const MCP342x::Resolution MCP342x::resolution16 = Resolution(0x08);
const MCP342x::Resolution MCP342x::resolution18 = Resolution(0x0c);

const MCP342x::Gain MCP342x::gain1 = Gain(0x00);
const MCP342x::Gain MCP342x::gain2 = Gain(0x01);
const MCP342x::Gain MCP342x::gain4 = Gain(0x02);
const MCP342x::Gain MCP342x::gain8 = Gain(0x03);

uint8_t MCP342x::generalCallReset(void) {
    mock().actualCall("MCP342x::generalCallReset");
    return 0;
}

MCP342x::MCP342x(void) {
    mock().actualCall("MCP342x->construct()");
}


MCP342x::MCP342x(uint8_t address) {
    mock().actualCall("MCP342x->construct(address)")
          .withIntParameter("address", address);
}

bool MCP342x::autoprobe(const uint8_t *addressList, uint8_t len) {
    mock().actualCall("MCP342x->autoprobe")
          .withMemoryBufferParameter("addressList", (const unsigned char*)addressList, len)
          .withUnsignedIntParameter("len", len);
    return mock().returnBoolValueOrDefault(true);
}
   
MCP342x::error_t MCP342x::convert(Channel channel, Mode mode, Resolution resolution, Gain gain) {
    mock().actualCall("MCP342x->convert")
          .withMemoryBufferParameter("channel", (const unsigned char*)&channel, sizeof(MCP342x::Channel))
          .withMemoryBufferParameter("mode", (const unsigned char*)&mode, sizeof(MCP342x::Mode))
          .withMemoryBufferParameter("resolution", (const unsigned char*)&resolution, sizeof(MCP342x::Resolution))
          .withMemoryBufferParameter("gain", (const unsigned char*)&gain, sizeof(MCP342x::Gain));
    return errorNone;
  }

MCP342x::error_t MCP342x::read(long &result, Config& config) {
    mock().actualCall("MCP342x->read")
          .withOutputParameter("result", (void*)&result)
          .withMemoryBufferParameter("config", (const unsigned char*)&config, sizeof(MCP342x::Config));
    return errorNone;
}