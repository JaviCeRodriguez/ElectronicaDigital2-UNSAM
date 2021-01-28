/* Armar un código en C para mostrar por pantalla los diferentes tamaños de los tipos: char, short,
int, long, float y double.
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Programa Principal ----*/
int main (){
    /* Variables */

    /* Prints */
    printf("\nTamaño de un char: %i bytes", sizeof(char));
    printf("\nTamaño de un short: %i bytes", sizeof(short int));
    printf("\nTamaño de un int: %i bytes", sizeof(int));
    printf("\nTamaño de un long: %i bytes", sizeof(long int));
    printf("\nTamaño de un float: %i bytes", sizeof(float));
    printf("\nTamaño de un double: %i bytes", sizeof(double));

    return 0;
}