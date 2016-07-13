#ifndef H_CORE_I2C
#define H_CORE_I2C

#include <stdint.h>


uint8_t spi_init();
uint8_t spi_release();
uint8_t spi_set_speed(uint32_t speed);
uint8_t spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint8_t len);

#endif
