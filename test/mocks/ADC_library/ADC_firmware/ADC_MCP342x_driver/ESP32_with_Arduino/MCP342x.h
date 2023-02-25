//MOCK of MCP342x.h arduino library by Steve Marple
#pragma once
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

class MCP342x {
public:
  class Config;
  class Channel;
  class Mode;
  class Resolution;
  class Gain;
  
  static const Channel channel1;
  static const Channel channel2;
  static const Channel channel3;
  static const Channel channel4;

  static const Mode oneShot;
  static const Mode continuous;

  static const Resolution resolution12;
  static const Resolution resolution14;
  static const Resolution resolution16;
  static const Resolution resolution18;
  
  static const Gain gain1;
  static const Gain gain2;
  static const Gain gain4;
  static const Gain gain8;

  static const uint8_t notReadyMask = 0x80;
  static const uint8_t newConversionMask = 0x80;
  static const uint8_t numChannels = 4;
  static const uint8_t maxResolution = 18;
  static const uint8_t maxGain = 8;
  static const int writeTimeout_us = 250;

  enum error_t {
    errorNone,
    errorConvertFailed,
    errorReadFailed,
    errorReadTimeout,
    errorConversionNotReady,
    errorConfigureFailed,
  };

  static uint8_t generalCallReset(void);

  MCP342x(void);
  MCP342x(uint8_t address);

  bool autoprobe(const uint8_t *addressList, uint8_t len);
   
  error_t convert(Channel channel, Mode mode, Resolution resolution, Gain gain);

  error_t read(long &result, Config& config);

};

class MCP342x::Channel {
  friend class MCP342x;
  friend class MCP342x::Config;
public:
  inline operator int(void) const {
    return (val >> 5) + 1;
  }

private:
  inline Channel(uint8_t v) : val(v & 0x60) {
  };
  
  uint8_t val;
};

class MCP342x::Mode {
  friend class MCP342x;
  friend class MCP342x::Config;
public:
  //inline operator int(void) const {
  //return (val >> 1) + 12;
  //}
private:
  inline Mode(uint8_t v) : val(v & 0x10) {
  };

  uint8_t val;
};

class MCP342x::Resolution {
  friend class MCP342x;
  friend class MCP342x::Config;
public:
  inline operator int(void) const {
    return (val >> 1) + 12;
  }
  unsigned long getConversionTime(void) const;
  
private:
  inline Resolution(uint8_t v) : val(v & 0x0c) {
  };

  uint8_t val;
};


class MCP342x::Gain {
  friend class MCP342x;
  friend class MCP342x::Config;
public:
    inline operator int(void) const {
      return (1 << val);
    }
  inline uint8_t log2(void) const {
    return val;
  }
private:
  inline Gain(uint8_t v) : val(v & 0x03) {
  };

  uint8_t val;
};

class MCP342x::Config {
public:
  bool isReady(void) const {
    mock().actualCall("MCP342x::Config->isReady");
    return mock().returnBoolValueOrDefault(false);
  }
};