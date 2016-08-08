/**
 * @file interrupts.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_INTERRUPTS
#define H_CORE_INTERRUPTS

#include <stdint.h>

/** Event on GPIO */
enum GPIO_EVENT {
    GPIO_RAISING   = 0x01,
    GPIO_FALLING   = 0x02,
    GPIO_EDGE      = 0x03
};

/**
 * @brief Configure an interrupt on the D0 Mikrobus pin
 *
 * @param[in] callback Callback function to be triggered on interrupt
 * @return 0 if successful, otherwise -1.
 */
int interrupt_configure(void (*callback)(uint8_t));

#endif
