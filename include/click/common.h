/**
 * @file include/click/common.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_COMMON_H__
#define __LETMECREATE_CLICK_COMMON_H__

#include <stdint.h>

/**
 * @brief Write one byte value to a register over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_address Address of the register on the slave
 * @param[in] value New value of the register
 * @return 0 if successful, otherwise it returns -1.
 */
int i2c_write_register(uint16_t address, uint8_t reg_address, uint8_t value);

/**
 * @brief Read a one-byte register from a slave over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_address Address of the register to read on the slave.
 * @param[out] data Pointer to a 8-bit variable to store the value of the register read from the slave
 * @return 0 if successful, otherwise it returns -1.
 */
int i2c_read_register(uint16_t address, uint8_t reg_address, uint8_t *data);

/**
 * @brief Read a 16bit register from a slave over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_low_address Address of the lower half of the register on the slave
 * @param[in] reg_high_address Address of the upper half of the register on the slave
 * @param[out] data Pointer to a 16-bit variable to store the value of the register read from the slave
 * @return 0 if successful, otherwise it returns -1.
 */
int i2c_read_16b_register(uint16_t address, uint8_t reg_low_address, uint8_t reg_high_address, uint16_t *data);

/**
 * @brief Write one byte to a register over SPI.
 *
 * @param[in] reg_address Address of the register
 * @param[in] data New value of the register
 * @return 0 if successful, -1 otherwise
 */
int spi_write_register(uint8_t reg_address, uint8_t data);

#endif
