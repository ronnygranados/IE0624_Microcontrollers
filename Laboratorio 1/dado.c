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
int rand();
void roll_dice(unsigned int number);

// Variables globales
unsigned int seed = 22042; // Necesito encontrar la manera de obtener un valor de seed diferente cada vez que se llame la función
unsigned int led_matrix[] = {one, two, three, four, five, six};

// Funcion Main
void main(void)
{

    TRISIO = 0b00100000; //Poner todos los pines como salidas
	GPIO = 0x00; //Poner pines en bajo
    ANSEL = 0x00; // Todas digitales

    unsigned int time = 100;

    while (1)
    {
        if (GP5 == 1)
        {   
            unsigned int random = rand();
            if (random > 6)
            {
                random = (random % 6) + 1;
                roll_dice(random);
            }else{
                roll_dice(random);
            }
            delay(time);
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

int rand(){
    int p = 20;
    int q = 24;

    seed = (seed*seed) % p*q;

    return seed;
}

void roll_dice(unsigned int number){ // Recibe el número del dado que se obtuvo de forma aleatoria y lo muestra en los LEDs
    switch (number)
    {
    case 1:
        GPIO = led_matrix[0];
    case 2:
        GPIO = led_matrix[1];
    case 3:
        GPIO = led_matrix[2];
    case 4:
        GPIO = led_matrix[3];
    case 5:
        GPIO = led_matrix[4];
    case 6:
        GPIO = led_matrix[5];
    }
}
