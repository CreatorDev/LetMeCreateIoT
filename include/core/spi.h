#ifndef H_CORE_SPI
#define H_CORE_SPI

#include <stdint.h>


int spi_init(uint8_t mode);
int spi_release();
int spi_set_speed(uint32_t speed);

int spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint32_t len);

#endif
