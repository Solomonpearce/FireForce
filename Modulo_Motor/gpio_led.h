/**
 * \file
 * \brief
 * \details
 * \author      Ricardo Andres Velasquez Velez
 * \version     0.0.1
 * \date        05/10/2023
 * \copyright   Unlicensed
 */

#ifndef __GPIO_LED_H__
#define __GPIO_LED_H__

#include <stdint.h>
#include "hardware/gpio.h"

static inline void Motor_init(uint8_t gpio_num){
    gpio_init( gpio_num); 
    gpio_set_dir(gpio_num,true); 
    gpio_put(gpio_num,false);
}

static inline void motor_on(uint8_t gpio_num){
    gpio_put(gpio_num,true);
}

static inline void motor_off(uint8_t gpio_num){
    gpio_put(gpio_num,false);
}

static inline void motor_toggle(uint8_t gpio_num){
    gpio_xor_mask(0x00000001 << gpio_num);
}

#endif