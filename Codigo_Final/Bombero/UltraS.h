/**
 * @file UltraS.h
 * @brief Archivo de cabecera para funciones de medición de distancia con un sensor ultrasonido.
 */

#ifndef ULTRAS_H
#define ULTRAS_H

    /**
     * @brief Inicializa los pines del sensor ultrasonido.
     * @param TRIG_PIN El pin de TRIG en la Raspberry Pi Pico.
     * @param ECHO_PIN El pin de ECHO en la Raspberry Pi Pico.
     */
    void init_Distance(int TRIG_PIN, int ECHO_PIN );

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
    bool Distancia(uint32_t comienzo, uint32_t final, uint32_t duracion,float distancia,int TRIG_PIN, int ECHO_PIN);

    /**
     * @brief Espera un número específico de microsegundos.
     * @param us El número de microsegundos a esperar.
     */
    void busy_wait_us_accurate(uint32_t us);

#endif