#include <pic14/pic12f683.h>

// Se definen los pines en variables para facil manipulacion
#define one 0b00000001
#define two 0b00000010
#define three 0b00000011
#define four 0b00000110
#define five 0b00000111
#define six 0b00010110

// Preambulo de funciones
void delay (unsigned int tiempo);
unsigned int rand();

// Variables globales
unsigned int seed = 22032; // Necesito encontrar la manera de obtener un valor de seed diferente cada vez que se llame la función
unsigned int led_matrix[] = {one, two, three, four, five, six};
unsigned int rand_counter = 0; // Counter for updating seed

// Funcion Main
void main(void)
{
    TRISIO = 0b00100000; //Poner todos los pines como salidas
	GPIO = 0x00; //Poner pines en bajo
    ANSEL = 0x00; // Todas digitales
    // unsigned int random;
    unsigned int time = 100;

    while (1)
    {   
        if (GP5 == 1)
        {   
            unsigned int random = rand();
            random = (random % 6);

            GPIO = led_matrix[random];
            delay(time);
            GPIO = 0x00;
        }
    }
}


void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}


unsigned int rand(){
    unsigned int p = 9802;
    unsigned int q = 2348;
    unsigned int random_num;

    seed += rand_counter++;
    random_num = (seed*seed) % p*q;

    return random_num;
}
