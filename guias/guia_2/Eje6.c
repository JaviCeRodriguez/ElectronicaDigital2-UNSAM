/* Se tiene la siguiente declaración:
        int x[5];
        int *ptrx;
a) ¿Cómo haría para que el puntero ptrx apunte a la primera posición del vector x?
b) ¿Recorrer el vector completo utilizando incrementos en el puntero ptrx?
c) Realizar un printf del puntero ptrx para cada incremento del punto b. Indicar por qué entre
valor y valor existe un salto en la secuencia.
d) Si en lugar de tener un vector de enteros (int) en el ejemplo utilizáramos un vector de chars,
el salto entre valores consecutivos del punto c cambiaría? Corroborarlo. 
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Programa Principal ----*/
int main (){
    /* Variables */
    int x[5] = {0, 1, 2, 4, 8};
    char y[5] = {'a', 'b', 'c', 'd', 'e'};
    int* ptrx;
    char* ptry;

    /* Inciso a */
    printf("\nDireccion del puntero antes: %p", ptrx);
    ptrx = &x[0]; //Equivalentemente: ptrx = x
    printf("\nDireccion del puntero despues: %p", ptrx);

    /* Inciso b y c */
    printf("\n");
    for (int i=0; i<5; i++){
        printf("\nEl puntero esta en x[%d] con direccion %p de memoria", i, (ptrx++));
    } // Hay saltos de 4 bytes de memoria porque el vector es entero

    /* Inciso d */
    // Cambia a saltos de 1 byte si se usa vector de chars
    ptry = &y[0];
    printf("\n");
    for (int d=0; d<5; d++){
        printf("\nEl puntero esta en y[%d] con direccion %p de memoria", d, (ptry++));
    }

    return 0;
}