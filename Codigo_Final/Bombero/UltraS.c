#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Espera un número específico de microsegundos.
 * @param us El número de microsegundos a esperar.
 */
void busy_wait_us_accurate(uint32_t us) {
    uint32_t start = time_us_32();
    while (time_us_32() - start < us);
}

/**
 * @brief Inicializa los pines del sensor ultrasonido.
 * @param TRIG_PIN El pin de TRIG en la Raspberry Pi Pico.
 * @param ECHO_PIN El pin de ECHO en la Raspberry Pi Pico.
 */
void init_Distance(int TRIG_PIN, int ECHO_PIN ){
    stdio_init_all();
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

/**
 * @brief Mide la distancia utilizando un sensor ultrasonido.
 * @param comienzo Tiempo en microsegundos cuando el pulso ECHO comienza.
 * @param final Tiempo en microsegundos cuando el pulso ECHO termina.
 * @param duracion Duración del pulso ECHO en microsegundos.
 * @param distancia Distancia medida en centímetros.
 * @param TRIG_PIN El pin de TRIG en la Raspberry Pi Pico.
 * @param ECHO_PIN El pin de ECHO en la Raspberry Pi Pico.
 * @return Verdadero si la medición fue exitosa, falso en caso contrario.
 */
bool Distancia(uint32_t comienzo, uint32_t final, uint32_t duracion,float distancia,int TRIG_PIN, int ECHO_PIN) {

    bool Choque = false;

    ///<Enviar pulso al TRIG
    gpio_put(TRIG_PIN, 1);
    busy_wait_us_accurate(10);
    gpio_put(TRIG_PIN, 0);

    ///<Esperar a que el ECHO comience
    while (gpio_get(ECHO_PIN) == 0);
    comienzo = time_us_32();

    ///< Esperar a que el ECHO termine
    while (gpio_get(ECHO_PIN) == 1);
    final = time_us_32();

    ///< Calcular la duración y la distancia
    duracion = final - comienzo;
    distancia = (duracion * 0.0343) / 2;

    printf("La distancia es: %.2f cm\n", distancia);

    if(distancia<=60){
        Choque=true;
    }
    return Choque;

}