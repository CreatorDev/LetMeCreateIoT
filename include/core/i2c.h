#ifndef H_CORE_I2C
#define H_CORE_I2C

#include <unistd.h>

uint8_t i2c_init();
uint8_t i2c_release();
uint8_t i2c_set_frequency(uint32_t frequency);
uint8_t i2c_write(uint8_t address, uint8_t reg_address, uint8_t * bytes, uint8_t length);
uint8_t i2c_read(uint8_t address, uint8_t reg_address, uint8_t * bytes, uint8_t length);

#endif
