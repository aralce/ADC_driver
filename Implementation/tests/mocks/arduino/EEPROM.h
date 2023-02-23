#pragma once
#include <cstdint>
#include <cstdio>

class EEPROM_class {
public:
    void write(int address, uint8_t value);
    void writeUShort(int address, uint16_t value);
    void writeULong(int address, uint32_t value);
    void writeFloat(int address, float value);
    
    bool commit();

    uint8_t read(int address);
    uint16_t readUShort(int address);
    uint32_t readULong(int address);
    float readFloat(int address);

    void begin(size_t size);
};

extern EEPROM_class EEPROM;

    // void HAL_EEPROM::write(int address, uint8_t value) {
    //     EEPROM.write(address, value);
    // }
    
    // void HAL_EEPROM::write(int address, uint16_t value) {
    //     EEPROM.writeUShort(address, value);
    // }
    
    // void HAL_EEPROM::write(int address, uint32_t value) {
    //     EEPROM.writeULong(address, value);
    // }
    
    // void HAL_EEPROM::write(int address, float value) {
    //     EEPROM.writeFloat(address, value);
    // };

    // bool HAL_EEPROM::commit() {
    //     return EEPROM.commit();
    // }

    // uint8_t HAL_EEPROM::read(int address) {
    //     return EEPROM.read(address);
    // }
    
    // uint16_t HAL_EEPROM::readUShort(int address) {
    //     return EEPROM.readUShort(address);
    // }
    
    // uint32_t HAL_EEPROM::readULong(int address) {
    //     return EEPROM.readULong(address);
    // }
    
    // float HAL_EEPROM::readFloat(int address) {
    //     return EEPROM.readFloat(address);
    // }

    // HAL_EEPROM::HAL_EEPROM() {
    //     // EEPROM.begin(EEPROM_SIZE_FOR_BATTERY_MODEL);
    //     EEPROM.begin(300); //TODO: link with EEPROM addresses
    // }