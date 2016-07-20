#ifndef H_CORE_SPI
#define H_CORE_SPI

#include <stdint.h>


uint8_t spi_init(uint8_t mode);
uint8_t spi_release();
uint8_t spi_set_speed(uint32_t speed);

uint8_t spi_start_transfer();
uint8_t spi_end_transfer();

uint8_t spi_write_byte(uint8_t reg_address, uint8_t byte);
uint8_t spi_read_byte(uint8_t reg_address, uint8_t byte);

uint8_t spi_write_bytes(uint8_t reg_address, const uint8_t * buffer, uint8_t len);
uint8_t spi_read_bytes(uint8_t reg_address, uint8_t * buffer, uint8_t len);

#endif
