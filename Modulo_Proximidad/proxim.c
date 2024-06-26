/**
 * @file proxim.c
 * @brief Programa para medir la distancia utilizando un sensor de proximidad HC-SR04.
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define TRIG_PIN 15 ///< Pin de TRIG en la Raspberry Pi Pico.
#define ECHO_PIN 14 ///< Pin de ECHO en la Raspberry Pi Pico.

/**
 * @brief Espera un número específico de microsegundos.
 * @param us El número de microsegundos a esperar.
 */
void busy_wait_us_accurate(uint32_t us) {
    uint32_t start = time_us_32();
    while (time_us_32() - start < us);
}

/**
 * @brief Punto de entrada del programa.
 * @return 0 si el programa se ejecuta correctamente.
 */
int main() {
    stdio_init_all();
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    uint32_t comienzo, final, duracion;
    float distancia;

    while (1) {
        // Enviar pulso al TRIG
        gpio_put(TRIG_PIN, 1);
        busy_wait_us_accurate(10);
        gpio_put(TRIG_PIN, 0);

        // Esperar a que el ECHO comience
        while (gpio_get(ECHO_PIN) == 0);
        comienzo = time_us_32();

        // Esperar a que el ECHO termine
        while (gpio_get(ECHO_PIN) == 1);
        final = time_us_32();

        // Calcular la duración y la distancia
        duracion = final - comienzo;
        distancia = (duracion * 0.0343) / 2;

        printf("La distancia es: %.2f cm\n", distancia);

        // Esperar 3 segundos antes de la siguiente medición
        uint32_t wait_start = time_us_32();
        while (time_us_32() - wait_start < 3000000);
    }

    return 0;
}