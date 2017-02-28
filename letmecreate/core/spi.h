/**
 * @file spi.h
 * @author Francois Berder, Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_SPI
#define H_CORE_SPI

#include <stdint.h>

enum SPI_MODE {
    SPI_MODE_0,
    SPI_MODE_1,
    SPI_MODE_2,
    SPI_MODE_3
};

enum SPI_SPEED {
    SPI_680K  = 680000,
    SPI_1M36  = 1360000,
    SPI_2M73  = 2730000,
    SPI_5M46  = 5460000,
    SPI_10M93 = 10930000,
    SPI_21M87 = 21870000,
    SPI_43M75 = 43750000
};

/**
 * @brief Initialises the SPI bus of the clicker
 *
 * Configure all SPI bus:
 *   - 8 bits per word
 *   - 2.73MHz
 *   - SPI_MODE_3
 *
 * @param[in] mode SPI mode (0-3)
 * @return 0 if successful, -1 otherwise
 */
int spi_init(void);

/**
 * @brief Set the mode of an SPI bus.
 *
 * @param[in] mikrobus_index Index of the bus to initialise (see #MIKROBUS_INDEX)
 * @param[in] mode Mode of the SPI bus (mode 0, 1, 2 or 3)
 * @return 0 if successful, -1 otherwise
 */
int spi_set_mode(uint8_t mikrobus_index, uint32_t mode);

/**
 * @brief Set the speed of the SPI bus.
 *
 * @param[in] speed Speed in Hz of the SPI bus
 * @return 0 if successful, -1 otherwise
 */
int spi_set_speed(uint32_t speed);

/**
 * @brief Select the bus to use.
 *
 * If the index given is invalid, the current bus will not change.
 *
 * @param[in] mikrobus_index Index of the bus to select (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int spi_select_bus(uint8_t mikrobus_index);

/**
 * @brief Get the current mikrobus index.
 *
 * @return Current mikrobus index used by SPI (see #MIKROBUS_INDEX)
 */
uint8_t spi_get_current_bus(void);

/**
 * @brief Get the transfer length limit of spidev driver
 *
 * This function reads the bufsiz parameter of the spidev driver at
 * /sys/module/spidev/parameters/bufsiz.
 *
 * @param[in] transfer_length_limit (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int spi_get_maximum_tranfer_length(uint32_t *transfer_length_limit);

/**
 * @brief Make a transfer of bytes over SPI.
 *
 * Make a transfer using the currently selected bus. @p tx_buffer and @p rx_buffer can be set to
 * NULL if no data has to be sent/received.
 *
 * @param[in] tx_buffer Address of the array of bytes to send
 * @param[out] rx_buffer Address of the array of bytes to receive from the bus
 * @param[in] len Number of bytes to read or write from the current bus.
 * @return @p len if successful, otherwise it returns -1.
 */
int spi_transfer(const uint8_t * tx_buffer, uint8_t * rx_buffer, uint32_t len);

/**
 * @brief Release the SPI bus.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int spi_release(void);
#endif
