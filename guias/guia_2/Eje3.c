/*  Crear una función que realice la suma de dos enteros. Utilizarla para imprimir por pantalla la
suma de dos enteros predefinidos.
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Funcion suma ----*/
int suma (int a, int b){
    return (a+b); // Retorno con la suma de los argumentos de entrada
}

/*---- Programa Principal ----*/
int main(){
    int x=1, y=2;
    printf("Introduzca un valor entero: ");
    scanf("%d", &x); // Pido un entero para x
    printf("Introduzca otro valor entero: ");
    scanf("%d", &y); // Pido un entero para y

    printf("La suma de %d y %d es %d", x, y, suma(x,y));

    return 0;
}

