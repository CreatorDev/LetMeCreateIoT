#ifndef H_CLICK_ACCEL
#define H_CLICK_ACCEL


uint8_t accel_click_enable();
uint8_t accel_click_get_measure(float * accelX, float * accelY, float * accelZ);
uint8_t accel_click_disable();


#endif
