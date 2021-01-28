/*Indicar los valores de x e y (y sus direcciones de memoria) en cada sentencia del siguiente
fragmento de código (mencionar cómo se llega a la obtención de los mismos).
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Programa Principal ----*/
int main(){
    /* Variables */
    int x = 1, y = 2;
    int *ptr;

    /* Operaciones */
    ptr = &x;   // Puntero 'ptr' se posiciona en 'x'
    y = *ptr;   // Variable 'y' toma el valor del puntero ('x')
    *ptr = 0;   // Puntero ahora vale 0

    /* Prints */
    printf("x vale %d y direcciona a %p\n", x, &x); // 'x' es 0 y se direcciona en x bytes
    printf("y vale %d y direcciona a %p\n", y, &y); // 'y' es 1 y se direcciona en x-4 bytes
    printf("Direccion del puntero: %p\n", ptr); // Puntero en 'x'

    return 0;
}
