/*La siguiente función calcula el largo de una cadena de caracteres. Reformularla para que la
funcionalidad sea resuelta por medio del uso de un puntero auxiliar en lugar de la variable n. 
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>

/*---- Medidor de cadenas ----*/
int strlen(char *s){
    char* aux; // Puntero auxiliar

    for (aux = &s[0]; *aux != '\0'; aux++){
    }
        
    return (aux-s); // Hago la resta de las direcciones de 'aux' y 's',
                    //dandome la longitud del array y retorno a main
}

/*---- Programa Principal ----*/
int main(){
    /* Variables */
    char nombre[] = "Javier Ceferino Rodriguez";

    /* Prints */
    printf("\nNombre: %s", nombre);
    printf("\nLongitud: %u", strlen(nombre)); // Uso %u para ver un valor entero
}