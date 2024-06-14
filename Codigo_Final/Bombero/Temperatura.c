#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"
#include <stdbool.h>

/**
 * @brief Realiza una medición de temperatura.
 * 
 * Esta función se encarga de realizar una medición de temperatura utilizando un sensor de temperatura conectado a la Raspberry Pi Pico.
 * La función devuelve un valor booleano que indica si la temperatura medida supera un umbral predefinido.
 * 
 * @return Verdadero si la temperatura medida supera el umbral, falso en caso contrario.
 */
bool Temp() {
    
    bool Tempt=false;
    uint16_t raw_value = adc_read(); ///<Lee el valor crudo del ADC (0-4095)
        
    ///<El LM35 da 10mV por cada grado Celsius, y el ADC tiene una resolución de 12 bits (0-4095)
    ///<con una referencia de 3.3V, por lo que la conversión es:
    ///<Temp (°C) = (Valor ADC * 3.3V / 4095) / 0.01V/°C
    float voltage = raw_value * 3.3 / (1 << 12); ///<1 << 12 es igual a 4096
    float temperature = voltage / 0.01; ///<Conversión a grados Celsius

    printf("Temperature: %.2f °C\n", temperature); ///<Imprime la temperatura

    if(temperature>=60){
        Tempt=true;
    }
    return Tempt;

}