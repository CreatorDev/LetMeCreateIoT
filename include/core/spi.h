/**
 * @file spi.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_SPI
#define H_CORE_SPI

#include <stdint.h>

/**
 * @brief Initialises the SPI bus of the clicker
 *
 * Configure all SPI bus:
 *   - 8 bits per word
 *   - 1MHz
 *
 * @param[in] mode SPI mode (0-3)
 * @return 0 if successful, -1 otherwise
 */
int spi_init(uint8_t mode);

/**
 * @brief Set the speed of the SPI bus.
 *
 * @param[in] speed Speed in Hz of the SPI bus
 * @return 0 if successful, -1 otherwise
 */
int spi_set_speed(uint32_t speed);

/**
 * @brief Make a transfer of bytes over SPI.
 *
 * Make a transfer using the currently selected bus. @p tx_buffer and @p rx_buffer can be set to
 * NULL if no data has to be sent/received.
 *
 * This function is compatible with the CI40 LMC spi_transfer, however it DOES NOT support
 * full duplex mode. It can only be used in three cases:
 * 1) To transfer data, receive none
 * 2) To receive data, transfer none
 * 3) To send ONE byte of data and receive the response. To maintain consistency, however,
 * data will be received starting with the SECOND byte of rx_buffer. First byte will be
 * untouched
 *
 * @param[in] tx_buffer Address of the array of bytes to send
 * @param[out] rx_buffer Address of the array of bytes to receive from the bus
 * @param[in] count Number of bytes to read or write from the current bus.
 * @return @p count if successful, otherwise it returns -1.
 */
int spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint32_t len);

/**
 * @brief Release the SPI bus.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int spi_release();
#endif
