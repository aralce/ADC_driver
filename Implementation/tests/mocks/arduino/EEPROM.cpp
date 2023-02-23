#include "EEPROM.h"

EEPROM_class EEPROM;

void EEPROM_class::write(int address, uint8_t value) {

}

void EEPROM_class::writeUShort(int address, uint16_t value) {

}

void EEPROM_class::writeULong(int address, uint32_t value) {

}

void EEPROM_class::writeFloat(int address, float value) {

}

bool EEPROM_class::commit() {
    return true;
}

uint8_t EEPROM_class::read(int address) {
    return 0;
}

uint16_t EEPROM_class::readUShort(int address) {
    return 0;
}

uint32_t EEPROM_class::readULong(int address) {
    return 0;
}

float EEPROM_class::readFloat(int address) {
    return 0;
}

void EEPROM_class::begin(size_t size) {

}