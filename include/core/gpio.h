/**
 * @file gpio.h
 * @author Francois Berder, Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef H_CORE_GPIO
#define H_CORE_GPIO

#include <stdint.h>


/** GPIO pin number */
enum GPIO_PIN {
    GPIO_AN  = 22,
    GPIO_RST = 23,
    GPIO_PWM = 73,
    GPIO_INT = 21,
    GPIO_CS  = 24, // Temporary

    /** For CI40 LMC compatibility */
    MIKROBUS_1_AN  = GPIO_AN,
    MIKROBUS_1_RST = GPIO_RST,
    MIKROBUS_1_PWM = GPIO_PWM,
    MIKROBUS_1_INT = GPIO_INT,
    MIKROBUS_1_CS  = GPIO_CS,
};

/** GPIO direction */
enum GPIO_DIR {
    GPIO_OUTPUT,
    GPIO_INPUT
};

/**
 * @brief Initialise a GPIO.
 *
 * Configures a pin as output. Does the same as gpio_set_direction, but is
 * needed for compatbility with CI40 LMC
 *
 * @param[in] gpio_pin Index of the GPIO
 * @return 0 if successful, -1 otherwise
 */
int gpio_init(uint8_t gpio_pin);

/**
 * @brief Configure GPIO as input or output.
 *
 * @param[in] gpio_pin Index of the GPIO
 * @param[in] dir Direction of the gpio (must be GPIO_OUTPUT or GPIO_INPUT)
 * @return 0 if successful, -1 otherwise
 */
int gpio_set_direction(uint8_t gpio_pin, uint8_t dir);

/**
 * @brief Get the direction of a GPIO.
 *
 * @param[in] gpio_pin Index of the GPIO
 * @param[out] dir Pointer to a variable (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int gpio_get_direction(uint8_t gpio_pin, uint8_t *dir);

/**
 * @brief Set the output state (high or low) of a GPIO
 *
 * The GPIO must be configured as an output.
 *
 * @param[in] gpio_pin Index of the GPIO
 * @param[in] value (0: low, any other value: high)
 * @return 0 if successful, -1 otherwise
 */
int gpio_set_value(uint8_t gpio_pin, uint8_t value);

/**
 * @brief Get the output state of a GPIO.
 *
 * @param[in] gpio_pin Index of the GPIO
 * @param[out] value Pointer to a variable (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int gpio_get_value(uint8_t gpio_pin, uint8_t *value);

/**
 * @brief Release a GPIO.
 *
 * Call once you're done using a GPIO, kept for compatbility with
 * CI40 LMC
 *
 * @param gpio_pin Index of the GPIO
 * @return 0 if successful, -1 otherwise
 */
int gpio_release(uint8_t gpio_pin);



#endif
