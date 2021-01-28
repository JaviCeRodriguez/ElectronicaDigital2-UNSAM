/* Codificar un procedimiento que intercambie dos enteros, por medio de la utilización de
punteros. Verificarlo mediante el llamado del mismo desde un código externo con impresión del
resultado (valores antes y después del intercambio). 
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Intercambio de enteros ----*/
void swap(int *a, int *b){
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

/*---- Programa Principal ----*/
int main(){
    /* Variables */
    int x = 1, y = 2;

    /* Operaciones y prints */
    printf("Valores originales:\tx = %d, y = %d\n", x, y);
    swap(&x, &y);
    printf("Valores nuevos:\t\tx = %d, y = %d\n", x, y);
}