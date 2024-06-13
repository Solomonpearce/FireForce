#include "pico/stdlib.h"

#define IN1 5
#define IN2 6
#define EN 4

void setup() {
    // Inicializa el subsistema de entrada/salida estándar
    stdio_init_all();
    
    // Configura los pines GPIO como salidas
    gpio_init(IN1);
    gpio_set_dir(IN1, GPIO_OUT);
    
    gpio_init(IN2);
    gpio_set_dir(IN2, GPIO_OUT);

    gpio_init(EN);
    gpio_set_dir(EN, GPIO_OUT);
    
    // Habilita el motor
    gpio_put(EN, 1);
}

void change_direction(bool direction) {
    if (direction) {
        gpio_put(IN1, 1);
        gpio_put(IN2, 0);
    } else {
        gpio_put(IN1, 0);
        gpio_put(IN2, 1);
    }
}

int main() {
    setup();
    
    bool direction = true;
    
    while (true) {
        change_direction(direction);
        direction = !direction;
        sleep_ms(5000);  // Espera 5 segundos
    }
    
    return 0;
}