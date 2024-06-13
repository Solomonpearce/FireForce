// /**
//  * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  */

// #include <stdio.h>

// #include "pico/stdlib.h"
// #include "hardware/dma.h"
// #include "hardware/adc.h"
// #include "hardware/pwm.h"
// #include "hardware/sync.h"
// #include "hardware/resets.h"
// #include <math.h>

// #define ADC_DMA_CH 0
// #define ADC_PIN 26

// int16_t buffer[100];

// volatile bool gFlagDMA = false;

// /// @brief 
// /// @param  
// void isrDMA_IRQ0(void){
//     //dma_channel_acknowledge_irq0(5);
//     dma_irqn_acknowledge_channel(0,ADC_DMA_CH);
//     adc_run(false);
//     gFlagDMA = true;
// }

// void pwmIRQ(void){
//     switch (pwm_get_irq_status_mask())
//     {
//     case 0x01UL:                                              ///< PWM slice 0 ISR used as a PIT to generate row sequence
//         pwm_clear_irq(0);                           ///< Acknowledge slice 0 PWM IRQ
//         if(!dma_channel_is_busy(ADC_DMA_CH)){
//             dma_hw->ch[ADC_DMA_CH].al2_transfer_count = 100;
//             dma_hw->ch[ADC_DMA_CH].al2_write_addr_trig = (uint32_t)buffer;
//             //dma_hw->ch[5].al2_transfer_count = 100;            
//             //dma_start_channel_mask(1);
//             adc_fifo_drain();
//             adc_run(true);
//             //printf("Si_%d\n",adc_fifo_get_level());
//         }
//         else{
//             printf("Channel Busy %d\n",adc_fifo_get_level());
//         }
//         break;
//     default:
//         printf("Paso lo que no debería pasar en PWM IRQ\n");
//         break;
//     }
//  }
 
// /// @brief 
// /// @param channel 
// void initDMAxADC(uint8_t channel){
//     dma_channel_config myDMAADCCH;
//     // Verify if DMA channel is being use by something else
//     if(!dma_channel_is_claimed(channel)){
//         // If channel available then claim the channel (register that channel is being used)
//         dma_channel_claim(channel);
//         // Create default DMA configuration
//         myDMAADCCH = dma_channel_get_default_config(channel);
//         // And now configurate the channel for ADC (Pheripheral to memory scenario)
//         // Read address do not increment
//         channel_config_set_read_increment (&myDMAADCCH, false);
//         // Write address do increment
//         channel_config_set_write_increment (&myDMAADCCH, true);
//         // Link DMA-channel to ADC
//         channel_config_set_dreq (&myDMAADCCH, DREQ_ADC);
//         // We will use ADC with 12 bits then transfers must be 16bits
//         channel_config_set_transfer_data_size (&myDMAADCCH, DMA_SIZE_16);
//         // We don't use ring feature
//         channel_config_set_ring (&myDMAADCCH, false, 0);
//         // We don't use bsap feature
//         channel_config_set_bswap (&myDMAADCCH, false);
//         // We want IRQ at the end of a transfer block
//         channel_config_set_irq_quiet (&myDMAADCCH, false);
//         // No need for preferencial treatment
//         channel_config_set_high_priority (&myDMAADCCH, false);
//         // Enable channel
//         channel_config_set_enable (&myDMAADCCH, true);
//         // Configurate the channel with the DMA configuration structure, and set read and write addresses
//         dma_channel_configure (channel, &myDMAADCCH, buffer, (const volatile void *)(ADC_BASE+ADC_FIFO_OFFSET), 100, false);
//         // Enable  IRQs on IRQ0 signal
//         dma_channel_set_irq0_enabled(channel,true);
//         // Set ISR for IRQ0 interrupt
//         irq_set_exclusive_handler(DMA_IRQ_0,isrDMA_IRQ0);
//         // Enable the DMA_IRQ_0 interrupt
//         irq_set_enabled(DMA_IRQ_0,true);
//         // Set DMA_IRQ_0 priority
//         irq_set_priority(DMA_IRQ_0,0);
//     }
//     else{
//         printf("Ay!!! juemadre, el canal no esta disponible\n");
//         while(1);
//     }
// }

// /// @brief 
// /// @param fsample 
// void initADCxTEMPSENSE_DMA(uint32_t fsample){
//     // Unreset ADC, enable it and wait to be ready
//     adc_init();
//     // Setup ADC channel
//     adc_select_input(4);
//     // Set round robin policy
//     adc_set_round_robin(0x00010UL);
//     // Enable temperature sensor
//     adc_set_temp_sensor_enabled(true);
//     uint32_t divINT = 48000000UL/fsample;
//     // Set
//     adc_set_clkdiv(divINT);
//     adc_fifo_setup (true, true, 1, true, false);
//     adc_fifo_drain();
//     //adc_run (true);
// }

// // Initialize ADC
// void initADC(uint32_t fsample) {
//     adc_init();
//     adc_gpio_init(ADC_PIN);
//     adc_select_input(0);

//     uint32_t div = 48000000 / fsample;
//     adc_set_clkdiv((float)div);
//     adc_fifo_setup(true, true, 1, false, false);
//     adc_fifo_drain();
// }

// void initPWMasPIT(uint8_t slice, uint16_t milis, bool enable){
//     assert(milis<=262);                                             ///< PWM can manage interrupt periods greater than 262 milis
//     float prescaler = (float)SYS_CLK_KHZ/500;
//     assert(prescaler<256); ///< the integer part of the clock divider can be greater than 255 
//     uint32_t wrap = 500000*milis/2000;
//     assert(wrap<((1UL<<16)-1));
//     pwm_config cfg =  pwm_get_default_config();
//     pwm_config_set_phase_correct(&cfg,true);
//     pwm_config_set_clkdiv(&cfg,prescaler);
//     pwm_config_set_clkdiv_mode(&cfg,PWM_DIV_FREE_RUNNING);
//     pwm_config_set_wrap(&cfg,wrap);
//     pwm_set_irq_enabled(slice,true);
//     irq_set_enabled(PWM_IRQ_WRAP,true);
//     irq_set_priority(PWM_IRQ_WRAP,255);
//     irq_set_exclusive_handler(PWM_IRQ_WRAP,pwmIRQ);
//     pwm_init(slice,&cfg,enable);
//  }

// int main() {
// 	stdio_init_all();
//     sleep_ms(2000);
//     //unreset_block_wait(RESETS_RESET_DMA_BITS);
// 	printf("Esto es una prueba del DMA\n\n");
//     initDMAxADC(ADC_DMA_CH);
//     //initADCxTEMPSENSE_DMA(100000);
//     initADC(100000);
//     initPWMasPIT(0,250,true);
//     //dma_start_channel_mask(1);
//     while (true) {
//         //printf("No estoy muerto%d\n",adc_fifo_get_level());
//         while(gFlagDMA){
//             uint32_t promedio=0, maxim=0, minim=INT32_MAX;
//             gFlagDMA=false;
//             uint16_t cnt=0;
//             for(int i=0; i<100; i++){
//                 if(!(buffer[i]&(1<<15))){
//                     cnt++;
//                     promedio+=buffer[i];
//                     if(buffer[i]>maxim){
//                         maxim=buffer[i];
//                     }
//                     if(buffer[i]<minim){
//                         minim=buffer[i];
//                     }
//                 }
//             }
//             promedio/=cnt;
//             float ADC_voltage_p = (float)(3.3*promedio)/4096.0;
//             float ADC_voltage_min = (float)(3.3*minim)/4096.0;
//             float ADC_voltage_max = (float)(3.3*maxim)/4096.0;
//             float T_p = 27 - (ADC_voltage_p - 0.706)*581.058;
//             float T_max = 27 - (ADC_voltage_min - 0.706)*581.058;
//             float T_min = 27 - (ADC_voltage_max - 0.706)*581.058;
//             printf("AvgTemp:%f\tvoltage:%f\tMinTemp:%f\tErrors:%d\n",T_p,ADC_voltage_p,T_min,100-cnt);
//         }

//         __wfi;
//     }
// }
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <math.h>

// Pin GPIO donde está conectado el sensor DS18B20
#define DS18B20_PIN 15

void ds18b20_reset() {
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);
    gpio_put(DS18B20_PIN, 0);
    sleep_us(480);
    gpio_set_dir(DS18B20_PIN, GPIO_IN);
    sleep_us(70);
    sleep_us(410);
}

void ds18b20_write_bit(bool bit) {
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);
    gpio_put(DS18B20_PIN, 0);
    sleep_us(bit ? 1 : 60);
    gpio_put(DS18B20_PIN, 1);
    sleep_us(bit ? 60 : 1);
}

bool ds18b20_read_bit() {
    bool bit;
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);
    gpio_put(DS18B20_PIN, 0);
    sleep_us(1);
    gpio_set_dir(DS18B20_PIN, GPIO_IN);
    sleep_us(14);
    bit = gpio_get(DS18B20_PIN);
    sleep_us(45);
    return bit;
}

void ds18b20_write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(byte & 1);
        byte >>= 1;
    }
}

uint8_t ds18b20_read_byte() {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        byte |= (ds18b20_read_bit() << i);
    }
    return byte;
}

float ds18b20_get_temperature() {
    ds18b20_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Convert T

    sleep_ms(750); // Wait for conversion

    ds18b20_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0xBE); // Read Scratchpad

    uint8_t temp_lsb = ds18b20_read_byte();
    uint8_t temp_msb = ds18b20_read_byte();
    int16_t temp = (temp_msb << 8) | temp_lsb;

    return temp / 16.0;
}

int main() {
    stdio_init_all();
    gpio_init(DS18B20_PIN);
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);

    while (1) {
        float temperature = ds18b20_get_temperature();
        printf("Temperature: %.2f°C\n", temperature);
        sleep_ms(500); // Leer cada 2 segundos
    }

    return 0;
}
