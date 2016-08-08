/**
 * @file i2c.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_I2C
#define H_CORE_I2C

#include <stdint.h>

/**
 * @brief Initialise the I²C bus.
 *
 * Initialises the I2C registers of the PIC32MX
 *
 * @return Returns -1 if it fails, otherwise it returns 0.
 */
int i2c_init(void);

/**
 * @brief Send some data to a slave.
 *
 * This sends some data to the slave. The buffer must be non-null and the current bus selected must
 * be initialised before calling this function.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave
 * @param[in] buffer Array of bytes to send
 * @param[in] count Number of bytes to send
 * @return Returns @p count if successful, otherwise it returns -1.
 */
int i2c_write(uint16_t slave_address, const uint8_t *buffer, uint32_t count);

/**
 * @brief Read data from a slave.
 *
 * This function sends some data to the slave. The buffer must be non-null and the current bus
 * selected must be initialised before calling this function.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[out] buffer Allocated memory where data is stored (must be non-null)
 * @param[in] count Number of bytes to read from slave
 * @return Returns @p count if successful, otherwise it returns -1.
 */
int i2c_read(uint16_t slave_address, uint8_t *buffer, uint32_t count);

/**
 * @brief Send one byte to a slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave
 * @param data Byte to send to slave
 * @return Returns 1 if successful, otherwise it returns -1.
 */
int i2c_write_byte(uint16_t slave_address, uint8_t data);

/**
 * @brief Read one byte from slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[out] data Address to store byte read from slave (must not be null)
 * @return Returns 1 if successful, otherwise it returns -1.
 */
int i2c_read_byte(uint16_t slave_address, uint8_t *data);

/**
 * @brief Deinitialize PIC32MX registers
 *
 * @return Returns -1 if it fails, otherwise it returns 0.
 */
int i2c_release(void);

#endif
