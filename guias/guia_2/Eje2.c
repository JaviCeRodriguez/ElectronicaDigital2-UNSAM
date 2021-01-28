/* Crear un vector de enteros de 10 posiciones, recorrerlo e imprimir por pantalla cada uno de sus
valores. ¿Qué tamaño ocupa en memoria? 
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Programa Principal ----*/
int main(){
    /* Variables */
    int vec[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    /* Imprimiendo las posiciones del for */
    for (int i=0; i<10; i++){
        printf("Ciclo %d\t\t", vec[i]+1);
    }
    /* Tamaño del vector */
    printf("\nEl tamaño del vector 'vec' es de %i bytes", sizeof(vec));

    return 0;
}