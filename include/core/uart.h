/**
 * @file uart.h
 * @author Francois Berder, Bartlomiej Zarnowski, Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef H_CORE_UART
#define H_CORE_UART

#include <stdint.h>

/** UART baud rates  */
enum UART_BAUDRATE {
    UART_BD_1200    = 1200,
    UART_BD_2400    = 2400,
    UART_BD_4800    = 4800,
    UART_BD_9600    = 9600,
    UART_BD_19200   = 19200,
    UART_BD_38400   = 38400,
    UART_BD_57600   = 57600,
    UART_BD_115200  = 115200
};

/** Special timeout value to disable it. Make uart_read blocking */
#define UART_TIMEOUT_NEVER      (0)

/**
 * @brief Initialise all UART devices.
 *
 * Set the baud rate to 9600. Set current UART device to MIKROBUS_1. Disable
 * timeout (set it to 0).
 *
 * @return 0 if successful, -1 otherwise
 */
int uart_init(void);

/**
 * @brief Select the current UART device.
 *
 * If mikrobus_index is not valid, it does not change the current UART device.
 *
 * @param[in] mikrobus_index Index of the device (see #MIKROBUS_INDEX)
 */
void uart_select_bus(uint8_t mikrobus_index);

/**
 * @brief Get the current mikrobus index.
 *
 * @return Current selected UART device (see #MIKROBUS_INDEX)
 */
uint8_t uart_get_current_bus(void);

/**
 * @brief Set the baud rate of the current UART device.
 *
 * The device must be initialised first.
 *
 * @param[in] baudrate Set the new baud rate of the UART device (see #UART_BAUDRATE for valid baud rates)
 * @return 0 if successful, -1 otherwise
 */
int uart_set_baudrate(uint32_t baudrate);

/**
 * @brief Get the speed of the current UART device.
 *
 * The device must be initialised first.
 * For compatibility reasons with LetMeCreate, this function returns the same
 * baudrate than the one set using uart_set_baudrate, which might be an
 * approximation of the actual baudrate.
 *
 * @param[out] baudrate Current baud rate of the UART device (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int uart_get_baudrate(uint32_t *baudrate);

/**
 * @brief Get the real speed of the current UART device.
 *
 * The device must be initialised first.
 * This function differs from uart_get_baudrate since it computes the baudrate
 * from UART3 registers. This gives the exact baudrate used by the UART device.
 *
 * @param[out] baudrate Actual baud rate of the UART device (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int uart_get_real_baudrate(uint32_t *baudrate);

/**
 * @brief Send some data using current UART device.
 *
 * @param[in] buffer Array of bytes
 * @param[in] count Number of bytes to send
 * @return @p count if successful, -1 otherwise
 */
int uart_send(const uint8_t *buffer, uint32_t count);

/**
 * @brief Receive some data using current UART device.
 *
 * @param[out] buffer Array of bytes
 * @param[in] count Number of bytes to receive
 * @return @p count if successful, -1 otherwise
 */
int uart_receive(uint8_t *buffer, uint32_t count);

/**
 * @brief Get timeout of currently selected UART bus.
 *
 * @return timeout Timeout in ms after which uart_receive will return, even if it does not read the request amount of bytes.
 */
uint32_t uart_get_timeout(void);

/**
 * @brief Set timeout of currently selected UART bus.
 *
 * Pass UART_TIMEOUT_NEVER to disable timeout functionality.
 *
 * @param[in] timeout Timeout in ms after which uart_receive will return, even if it does not read the request amount of bytes.
 */
void uart_set_timeout(uint32_t timeout);

/**
 * @brief Release all UART devices.
 *
 * Close device file and restore old parameters.
 *
 * @return 0 if successful, -1 otherwise
 */
int uart_release(void);

#endif
