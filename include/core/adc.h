/**
 * @file adc.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef H_CORE_ADC
#define H_CORE_ADC

#include <stdint.h>

/**
 * @brief Get the reading of an ADC in Volt, in range 0-5V.
 *
 * @param[in] mikrobus_index Index of the ADC (see #MIKROBUS_INDEX)
 * @param[out] value Pointer to a floating point variable (must be non-null)
 * @return 0 if successful, -1 otherwise
 */
int adc_get_value(uint8_t mikrobus_index, float * value);

#endif

