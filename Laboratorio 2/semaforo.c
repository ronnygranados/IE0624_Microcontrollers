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
volatile int boton;
int estado_actual = esperando_boton;

volatile int segs = 0;
// int un_seg = 0; // Un acumulador 
// int tres_segs = 0;
// int diez_segs = 0;

// Preambulo de funciones
void boton_init();
void timer_init();
void cruzando();
void detenido();
void delay(float seconds);
void fsm();

// Interrupciones 

ISR(INT0_vect){ // Subrutina que alterna B1 entre 1 y 0
    boton = !boton;
}

ISR(TIMER1_COMPA_vect){
    // segs = segs + 10;
    segs++;
    // if (segs >= 3*188) // Un segundo
    // {
    //     /* code */
    //     PORTB |= (1 << RED_PEATON1); // Sí funciona
    //     // segs = 0;
    //     // PORTB &= ~(1 << RED_PEATON1);
    // }
    
}

// Inicio de la función main
// Disclaimer: Voy a implementar todo usando _delay_ms(), la lógica de la máquina de estados,
//             saber que funciona bien y ya luego le meto la función de delay()
int main(void)
{
    boton_init();
    sei();
    // timer_init();

    TCCR1B = 0x00;
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Esto me activa el prescaler de 1024 y el modo CTC
    TIMSK |= (1 << OCIE1A);
    OCR1A = 38; // Esto va a hacer que se genere una interrupción cada ~10ms
    
    DDRB |= (1 << RED_PEATON1) | (1 << GREEN_PEATON1) | (1 << RED_PEATON2) | (1 << GREEN_PEATON2); // Pines de semaforos peatonales como salidas
    DDRB |= (1 << RED_VEHICULAR) | (1 << GREEN_VEHICULAR); // Pines del semáforo vehicular como salidas
    
    boton = 0; // Inicializo la variable B1 en 0

    while (1)
    {
        // delay(3);
        // PORTB |= (1 << GREEN_VEHICULAR);
        // delay(1000);
        // PORTB &= ~(1 << RED_PEATON1);
        // delay(1000);
        fsm();
        // PORTB |= (1 << GREEN_VEHICULAR);
        // delay(0.01);
        // PORTB |= (1 << RED_PEATON1);
        // PORTB |= (1 << RED_PEATON2);

        // // _nop();
        // // PORTB |= (1 << RED_PEATON1) | (1 << RED_PEATON2);
        // PORTB &= ~(1 << RED_VEHICULAR) & ~(GREEN_PEATON1) & ~(GREEN_PEATON2);

        // if (boton == 1)
        // {
        //     for (int i = 0; i < 6; i++)
        //     {
        //         PORTB ^= (1 << GREEN_VEHICULAR);
        //         delay(0.5);
        //     }
        //     PORTB = 0x00;
        // }
        
        // if (boton == 1)
        // {
        //     for (int i = 0; i < 6; i++)
        //     {
        //         PORTB ^= (1 << RED_PEATON1);
        //         // PORTB ^= (1 << RED_PEATON2) ^ (1 << RED_VEHICULAR);
        //         // _delay_ms(2000);
        //         delay(0.5);
        //     }
        //     boton = 0;
        // } else
        // {
        //     PORTB &= ~(1 << RED_PEATON1);
        // }
    }
}

// Final de la función main

// Funciones del programa
void boton_init() { // Se establece para detectar flancos positivos en el pin PD2
    GIMSK |= (1 << INT0); // Habilitar la INT0 (interrupción externa)
    MCUCR |= (1 << ISC01); // Configurar como flanco descendente
}

void timer_init(){
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Esto me activa el prescaler de 1024 y el modo CTC
    TIMSK |= (1 << OCIE1A);
    OCR1A = 38; // Esto va a hacer que se genere una interrupción cada ~10ms
}

void cruzando(){
    PORTB |= (1 << GREEN_PEATON1)| (1 << GREEN_PEATON2) | (1 << GREEN_VEHICULAR);
    PORTB &= ~(1 << RED_PEATON1) & ~(1 << RED_PEATON2) & ~(1 << RED_VEHICULAR);
}

void detenido(){
    PORTB &= ~(1 << GREEN_PEATON1) & ~(1 << GREEN_PEATON2) & ~(1 << GREEN_VEHICULAR);
    PORTB |= (1 << RED_PEATON1) | (1 << RED_PEATON2) | (1 << RED_VEHICULAR);
}

void delay(float seconds) { // Esta función sirve
    segs = 0; // Reseteo a la variable segs
    while (segs < 188*seconds) { 
        // No haga nada
    }
}

void fsm(){
    switch (estado_actual)
    {
    case esperando_boton:
        // Carros pasan
        PORTB |= (1 << GREEN_VEHICULAR);
        delay(0.01);
        PORTB |= (1 << RED_PEATON1) | (1 << RED_PEATON2);
        PORTB &= ~(1 << RED_VEHICULAR) & ~(GREEN_PEATON1) & ~(GREEN_PEATON2);

        if (boton == 1)
        {
            estado_actual = blink_vehicular;
        }
        break;

    case blink_vehicular:
        for (int i = 0; i < 6; i++)
        {
            PORTB ^= (1 << GREEN_VEHICULAR);
            delay(0.5);
        }
        estado_actual = detener_trafico;
        break;

    case detener_trafico:
        delay(1);
        PORTB &= ~(1 << GREEN_VEHICULAR);
        PORTB |= (1 << RED_VEHICULAR);

        delay(0.01);

        PORTB |= (1 << GREEN_PEATON1) | (1 << GREEN_PEATON2);
        PORTB &= ~(1 << RED_PEATON1) & ~(1 << RED_PEATON2);
        delay(10);
        estado_actual = blink_peatonal;
        break;

    case blink_peatonal:
        for (int i = 0; i < 6; i++)
        {
            PORTB ^= (1 << GREEN_PEATON1) ^ (1 << GREEN_PEATON2);
            delay(0.5);
        }
        PORTB &= ~(1 << GREEN_PEATON1) & ~(1 << GREEN_PEATON2);
        PORTB |= (RED_PEATON1);
        delay(1);
        boton = 0;
        estado_actual = esperando_boton;
        break;
    }
}