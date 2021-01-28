/*Compilar el siguiente código y explicar lo obtenido en la corrida.
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Programa Principal ----*/
int main(void){
    int a = 0x12345678;
    short int b = 0xABCD;
    char c = 'a';

    int * ptr_a = &a;
    short int * ptr_b = &b;
    char * ptr_c = &c;
    printf("\nValor de ptr_a:\t\t %p\n", ptr_a);
    printf("Valor de ptr_a + 1:\t %p\n", ++ptr_a);
    printf("\nValor de ptr_b:\t\t %p\n", ptr_b);
    printf("Valor de ptr_b + 1:\t %p\n", ++ptr_b);
    printf("\nValor de ptr_c:\t\t %p\n", ptr_c);
    printf("Valor de ptr_c + 1:\t %p\n", ++ptr_c);
}