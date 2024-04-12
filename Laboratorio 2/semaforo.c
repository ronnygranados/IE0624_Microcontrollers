#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Pines del semáforo peatonal 1
#define RED_PEATON1 PB7
#define GREEN_PEATON1 PB6

// Pines del semáforo peatonal 2
#define RED_PEATON2 PB5
#define GREEN_PEATON2 PB4

// Pines del semáforo vehicular
#define RED_VEHICULAR PB3
#define GREEN_VEHICULAR PB2

// Definición de los estados para la FSM
#define esperando_boton 0
#define blink_vehicular 1
#define detener_trafico 2
#define blink_peatonal 3

// Variables globales del programa
int boton;

// Preambulo de funciones
void external_interrupt();
void cruzando();
void detenido();
// void blinking_vehicular();

// Interrupciones 

ISR(INT0_vect){ // Subrutina que alterna B1 entre 1 y 0
    boton = !boton;
}

// Inicio de la función main

int main(void)
{
    external_interrupt(); // Voy a detectar flancos negativos en el registro INT0, mediante el pin D2

    DDRB |= (1 << RED_PEATON1) | (1 << GREEN_PEATON1) | (1 << RED_PEATON2) | (1 << GREEN_PEATON2); // Pines de semaforos peatonales como salidas
    DDRB |= (1 << RED_VEHICULAR) | (1 << GREEN_VEHICULAR); // Pines del semáforo vehicular como salidas
    DDRB |= (1 << PB1);
    
    boton = 0; // Inicializo la variable B1 en 0

    sei(); // habilito interrupciones globales

    while (1)
    {
        // blinking_vehicular();
        if (boton == 1)
        {
            detenido();
        } else
        {
            cruzando();
        }
    }
}

// Final de la función main


// Funciones del programa

void external_interrupt() { // Se establece para detectar flancos positivos en el pin PD2
    GIMSK |= (1 << INT0); // Habilitar la INT0 (interrupción externa)
    MCUCR |= (1 << ISC01); // Configurar como flanco descendente
}

void cruzando(){
    PORTB |= (1 << GREEN_PEATON1);
    PORTB |= (1 << GREEN_PEATON2);
    PORTB |= (1 << GREEN_VEHICULAR);

    PORTB &= ~(1 << RED_PEATON1);
    PORTB &= ~(1 << RED_PEATON2);
    PORTB &= ~(1 << RED_VEHICULAR);
}

void detenido(){
    PORTB &= ~(1 << GREEN_PEATON1);
    PORTB &= ~(1 << GREEN_PEATON2);
    PORTB &= ~(1 << GREEN_VEHICULAR);
    
    PORTB |= (1 << RED_PEATON1);
    PORTB |= (1 << RED_PEATON2);
    PORTB |= (1 << RED_VEHICULAR);
}

// void blinking_vehicular(){
//     // PORTB ^= (1 << PB1);
// }