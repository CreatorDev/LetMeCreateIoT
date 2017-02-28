/**
 * @file pwm.h
 * @author Francois Berder
 * @date 2017
 * @copyright 3-clause BSD
 */


#ifndef H_CORE_PWM
#define H_CORE_PWM

#include <stdint.h>

/**
 * @brief Initialise a PWM pin.
 *
 * Set the duty cycle to 50%, the frequency to 3kHz and disable the pin.
 *
 * @param[in] mikrobus_index Index of the pin to initialise (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int pwm_init(uint8_t mikrobus_index);

/**
 * @brief Enable PWM on a pin.
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int pwm_enable(uint8_t mikrobus_index);

/**
 * @brief Set the duty cycle.
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @param[in] percentage Percentage of the period when pin is high (must in range [0, 100])
 * @return 0 if successful, -1 otherwise
 */
int pwm_set_duty_cycle(uint8_t mikrobus_index, float percentage);

/**
 * @brief Get the duty cycle.
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @param[out] percentage (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int pwm_get_duty_cycle(uint8_t mikrobus_index, float *percentage);

/**
 * @brief Set the frequency.
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @param[in] frequency Frequency of the PWM output in Hertz
 * @return 0 if successful, -1 otherwise
 */
int pwm_set_frequency(uint8_t mikrobus_index, uint32_t frequency);

/**
 * @brief Get the frequency in Hz
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @param[out] frequency (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int pwm_get_frequency(uint8_t mikrobus_index, uint32_t *frequency);

/**
 * @brief Set the period
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @param[in] period Period of PWM output in nanoseconds
 * @return 0 if successful, -1 otherwise
 */
int pwm_set_period(uint8_t mikrobus_index, uint32_t period);

/**
 * @brief Get the Period in nanoseconds
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @param[out] period Pointer to an integer (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int pwm_get_period(uint8_t mikrobus_index, uint32_t *period);

/**
 * @brief Disable PWM output on a pin.
 *
 * @param[in] mikrobus_index Index of the pin (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int pwm_disable(uint8_t mikrobus_index);

/**
 * @brief Release a PWM pin.
 *
 * @param[in] mikrobus_index Index of the pin to release (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int pwm_release(uint8_t mikrobus_index);


#endif
