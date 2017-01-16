/**
 * @file gpio_monitor.h
 * @author Francois Berder, Michal Tusnio
 * @date 2017
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_GPIO_MONITOR_H__
#define __LETMECREATE_CORE_GPIO_MONITOR_H__

#include <stdint.h>
#include <letmecreate/core/gpio.h>

/** Event on GPIO */
enum GPIO_EVENT {
    GPIO_RAISING   = 0x01,
    GPIO_FALLING   = 0x02,
    GPIO_EDGE      = 0x03
};


/**
 * @brief Initialise interrupt bits.
 *
 * This function must be called before adding/removing callbacks.
 *
 * @return 0 if successful, -1 otherwise
 */
int gpio_monitor_init(void);

/**
 * @brief Attach a callback to a GPIO. At the moment only one callback per pin is supported
 *
 * @param[in] gpio_pin GPIO to monitor (see #GPIO_PIN)
 * @param[in] event_mask Events which trigger callback (see #GPIO_EVENT)
 * @param[in] callback Function to call (uint8_t argument is the event type)
 * @return ID of the callback (non-negative integer) if successful, -1 otherwise
 */
int gpio_monitor_add_callback(uint8_t gpio_pin, uint8_t event_mask, void(*callback)(uint8_t));

/**
 * @brief Detach a callback from a GPIO
 *
 * @param[in] callback_ID ID of the callback (must not be negative)
 * @return 0 if successful, -1 otherwise
 */
int gpio_monitor_remove_callback(int callback_ID);

/**
 * @brief Remove all callbacks.
 *
 * @return 0 if successful, -1 otherwise
 */
int gpio_monitor_release(void);

#endif
