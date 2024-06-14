/**
 * @file access_sys.c
 * @brief Archivo principal del sistema de acceso.
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "Temperatura.h"
#include "UltraS.h"
#include "hardware/adc.h"
#include "hardware/irq.h"
#include "hardware/sync.h "
#include "hardware/gpio.h"
#include <stdbool.h>
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include <stdbool.h>
#include "pico/time.h"



///<Ultrasonido
#define TRIG_PIN 14
#define ECHO_PIN 15 ///< Asumí que el ECHO está en un pin diferente
///<Motores
#define IN1 5
#define IN2 6
#define EN 4
#define IN3 7
#define IN4 8
#define EN2 9
///<Bomba
#define CAUDAL_PIN 10
///<Temperatura
#define ADC_PIN 26  ///<El pin GP26 corresponde a ADC0
uint32_t comienzo=0;
uint32_t final=0;
uint32_t duracion=0;
float distancia=0;
/**
 * @brief Estructura para almacenar las banderas de interrupción.
 */
typedef union{
    uint8_t W;///<Variable de 8 bits que representa las interrupciones con las banderas.
    struct{
        bool Muro :1;///<Se activa si el sistema detecta un obstaculo
        bool Fuego :1;///<se activa si el sensor detecta temperaturas muy altas
        uint8_t      :2;///<Bits reservados no utilizados (padding)
    }B;
}myFlags_t;
volatile myFlags_t gFlags;///<Variable global volátil del tipo 'myFlags_t' para almacenar banderas

/**
 * @brief Callback para las interrupciones GPIO.
 * 
 * Esta función se llama cuando se produce una interrupción GPIO. Comprueba si se ha detectado un obstáculo o un incendio.
 * 
 * @param gpio El número de pin GPIO que causó la interrupción.
 * @param events El tipo de evento que causó la interrupción.
 */
void gpio_callback(uint gpio, uint32_t events) {
    bool T = Temp();
    if(T){
        gFlags.B.Muro = false;
        gFlags.B.Fuego = true;

    }else {
        gFlags.B.Fuego = false;
        bool D = Distancia(comienzo, final, duracion, distancia, TRIG_PIN, ECHO_PIN);
        if(D){
            gFlags.B.Muro = true;
            
        } else{
             gFlags.B.Muro = false;
        }
    }
    
   
}
/**
 * @brief Configura los pines GPIO y el ADC.
 */
void setup() {
    ///< Inicializa el subsistema de entrada/salida estándar
    stdio_init_all();
    gpio_set_function(CAUDAL_PIN,GPIO_FUNC_SIO);
    gpio_set_function(TRIG_PIN,GPIO_FUNC_SIO);
    
    ///< Configura los pines GPIO como salidas
    gpio_init(IN1);
    gpio_set_dir(IN1, GPIO_OUT);
    
    gpio_init(IN2);
    gpio_set_dir(IN2, GPIO_OUT);

    gpio_init(EN);
    gpio_set_dir(EN, GPIO_OUT);
    
    gpio_init(CAUDAL_PIN);
    gpio_set_dir(CAUDAL_PIN, GPIO_OUT);
    gpio_put(CAUDAL_PIN,0);
    ///< Habilita el motor
    gpio_put(EN, 1);
    printf("MOTOR ENCENDIDO");
     ///< Configura los pines GPIO como salidas
    gpio_init(IN3);
    gpio_set_dir(IN3, GPIO_OUT);
    
    gpio_init(IN4);
    gpio_set_dir(IN4, GPIO_OUT);

    gpio_init(EN2);
    gpio_set_dir(EN2, GPIO_OUT);
    
    ///< Habilita el motor
    gpio_put(EN2, 1);
    ///<Inicializa los pines del medidor ultrasonnico
    init_Distance( TRIG_PIN, ECHO_PIN );
    gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
    ///<inicializa ADC para la temperatura
    ///< Inicializa el ADC
    adc_init();

    ///< Habilita el pin ADC correspondiente
    adc_gpio_init(ADC_PIN);

    ///< Selecciona el canal de entrada, en este caso el canal 0 (ADC0)
    adc_select_input(0);
    gpio_set_irq_enabled_with_callback(26, GPIO_IRQ_EDGE_RISE, true, gpio_callback);
}
/**
 * @brief Cambia la dirección de los motores.
 * 
 * @param direction La dirección a la que se deben mover los motores.
 */
void change_direction(bool direction) {
    if (direction) {
        gpio_put(EN, 1);
        gpio_put(IN1, 1);
        gpio_put(IN2, 0);
        gpio_put(EN2, 1);
        gpio_put(IN3, 1);
        gpio_put(IN4, 0);
    } else {
        gpio_put(EN, 1);
        gpio_put(IN1, 0);
        gpio_put(IN2, 1);
        gpio_put(EN2, 1);
        gpio_put(IN3, 1);
        gpio_put(IN4, 0);
    }
}
/**
 * @brief Detiene los motores.
 */
void Stop(){
    gpio_put(EN, 0);
    gpio_put(EN2, 0);
}
/**
 * @brief Función principal del programa.
 * 
 * Configura los pines GPIO y el ADC, luego entra en un bucle infinito donde comprueba las banderas de interrupción y actúa en consecuencia.
 * 
 * @return 0 si el programa se ejecuta correctamente.
 */
int main() {
    setup();
    bool direction = true;
    change_direction(direction);
    while (true) {
        if(!gFlags.W){
         change_direction(direction);
        }
        while(gFlags.W){///</<Se rectifican las banderas de las interrupciones

            if(gFlags.B.Muro){///</<Rectifica si se activo la bandera de la detección de un obstaculo
                change_direction(!direction);
            }
            if(gFlags.B.Fuego){
                printf("Estoy vivo,fuego");
                Stop();
                gpio_put(CAUDAL_PIN, 1);

            } else {
                gpio_put(CAUDAL_PIN, 0);
            }
        
        }
        __wfi();
    }
return 0;
}