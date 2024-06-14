# FireForce

FIreforce es un proyecto que busca adecuar un sistema de sensores a un vehiculo de control autonomo, para la respuesta contra incendios. 

El vehiculo Fireforce está equipado con una serie de sensores y actuadores que le permiten navegar de forma autónoma , detectar obstaculos y fuego, para posteriormente activar un sistema de extinción de incendios. El vehículo se controla mediante el sistema de la Raspberry Pi Pico. 

El codigo fuente presente se divide en dos etapas, una modular, en donde están las funcionalidades por separado del sistema, como lo es el funcionamiento de los actuadores y la sensorica del mismo. Así cómo un modulo más complejo en donde se integran todas las funcionalidades, mediante el uso de flujo de programa conocido como Polling + Interrupciones.
