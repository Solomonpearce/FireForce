/**
 * @file adc_dma.c
 * @brief Programa para leer la temperatura de un sensor DS18B20. Posteriormente un LM35
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <math.h>

#define DS18B20_PIN 15 ///< Pin GPIO donde está conectado el sensor DS18B20, Posteriormente el LM35

/**
 * @brief Realiza un reset al sensor DS18B20.
 */
void ds18b20_reset() {
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);
    gpio_put(DS18B20_PIN, 0);
    sleep_us(480);
    gpio_set_dir(DS18B20_PIN, GPIO_IN);
    sleep_us(70);
    sleep_us(410);
}

/**
 * @brief Escribe un bit al sensor DS18B20.
 * @param bit El bit a escribir.
 */
void ds18b20_write_bit(bool bit) {
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);
    gpio_put(DS18B20_PIN, 0);
    sleep_us(bit ? 1 : 60);
    gpio_put(DS18B20_PIN, 1);
    sleep_us(bit ? 60 : 1);
}

/**
 * @brief Lee un bit del sensor DS18B20.
 * @return El bit leído.
 */
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

/**
 * @brief Escribe un byte al sensor DS18B20.
 * @param byte El byte a escribir.
 */
void ds18b20_write_byte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(byte & 1);
        byte >>= 1;
    }
}

/**
 * @brief Lee un byte del sensor DS18B20.
 * @return El byte leído.
 */
uint8_t ds18b20_read_byte() {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        byte |= (ds18b20_read_bit() << i);
    }
    return byte;
}

/**
 * @brief Obtiene la temperatura actual del sensor DS18B20.
 * @return La temperatura en grados Celsius.
 */
float ds18b20_get_temperature() {
    ds18b20_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Convert T

    sleep_ms(750); ///<Wait for conversion

    ds18b20_reset();
    ds18b20_write_byte(0xCC); ///<Skip ROM
    ds18b20_write_byte(0xBE); ///<Read Scratchpad

    uint8_t temp_lsb = ds18b20_read_byte();
    uint8_t temp_msb = ds18b20_read_byte();
    int16_t temp = (temp_msb << 8) | temp_lsb;

    return temp / 16.0;
}

/**
 * @brief Punto de entrada del programa.
 * @return 0 si el programa se ejecuta correctamente.
 */
int main() {
    stdio_init_all();
    gpio_init(DS18B20_PIN);
    gpio_set_dir(DS18B20_PIN, GPIO_OUT);

    while (1) {
        float temperature = ds18b20_get_temperature();
        printf("Temperature: %.2f°C\n", temperature);
        sleep_ms(500); ///<Leer cada 2 segundos
    }

    return 0;
}