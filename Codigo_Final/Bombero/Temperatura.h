/**
 * @file Temperatura.h
 * @brief Archivo de cabecera para la función de medición de temperatura.
 */

#ifndef TEMPERATURA_H
#define TEMPERATURA_H

    /**
     * @brief Realiza una medición de temperatura.
     * 
     * Esta función se encarga de realizar una medición de temperatura utilizando un sensor de temperatura conectado a la Raspberry Pi Pico.
     * La función devuelve un valor booleano que indica si la temperatura medida supera un umbral predefinido.
     * 
     * @return Verdadero si la temperatura medida supera el umbral, falso en caso contrario.
     */
    bool Temp();

#endif