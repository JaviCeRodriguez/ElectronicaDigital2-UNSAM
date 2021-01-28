/* Salida del programa:
Valores iniciales de la estructura
    id1:            B
    id2:            C
    id3:            Sensible
    Nombre:         Pedro
    Direccion:      Av. Carlos Calvo 1234
    Edad:           23
    Varios:         68
-
Direccion de la estructura:         0x0022FEF4
-
Direccion del miembro id1:          0x0022FEF4 (offset: 0 bytes)
Direccion del miembro id2:          0x0022FEF5 (offset: 1 bytes)
Direccion del miembro id3:          0x0022FEF6 (offset: 2 bytes)
Direccion del miembro nombre:       0x0022FF00 (offset: 12 bytes)
Direccion del miembro domicilio:    0x0022FF04 (offset: 16 bytes)
Direccion del miembro edad:         0x0022FF08 (offset: 20 bytes)
Direccion del miembro varios:       0x0022FF0C (offset: 24 bytes)
-
Dirección de la primera posición de memoria después de la estructura: 0x0022FF10 
-
    a) Analizar los distintos valores presentes en la misma indicando claramente el significado de
cada uno.
    b) Completar la codificación presentada (implementar el procedimiento showinfo) y realizar una
corrida del programa compilado, verificando los resultados obtenidos con los presentados en
este punto. 
-------------------------------------------------------------------------------------------------
Alumno: Javier Ceferino Rodriguez */

/*---- Includes ----*/
#include <stdio.h>
#include <stddef.h>

struct pru_struct {
    char id1;
    char id2;
    char id3[10];
    char *nombre;
    char *domicilio;
    int edad;
    int varios;
};

void showinfo(struct pru_struct* p){
    printf("\nValores iniciales de la estructura\n");
    printf("\tid1:\t\t %c\n", p->id1);
    printf("\tid2:\t\t %c\n", p->id2);
    printf("\tid3:\t\t %s\n", p->id3);
    printf("\tNombre:\t\t %s\n", p->nombre);
    printf("\tEdad:\t\t %d\n", p->edad);
    printf("\tVarios:\t\t %d\n", p->varios);

    printf("\nDireccion de la estructura:  0x%p\n", p);

    printf("\nDireccion del miembro id1:\t\t 0x%p (offset: %d bytes)\n", &p->id1, (int)offsetof(struct pru_struct, id1));
    printf("Direccion del miembro id2:\t\t 0x%p (offset: %d bytes)\n", &p->id2, (int)offsetof(struct pru_struct, id2));
    printf("Direccion del miembro id3:\t\t 0x%p (offset: %d bytes)\n", &p->id3, (int)offsetof(struct pru_struct, id3));
    printf("Direccion del miembro nombre:\t\t 0x%p (offset: %d bytes)\n", &p->nombre, (int)offsetof(struct pru_struct, domicilio));
    printf("Direccion del miembro edad:\t\t 0x%p (offset: %d bytes)\n", &p->edad, (int)offsetof(struct pru_struct, edad));
    printf("Direccion del miembro varios:\t\t 0x%p (offset: %d bytes)\n", &p->varios, (int)offsetof(struct pru_struct, varios));

    printf("\nDireccion de la primera posicion de memoria despues de la estructura: 0x%p\n\n", ++p);
}

int main(){
    int i;
    int tmp;
    struct pru_struct empleados = {
        'B',
        'C',
        "Sensible",
        "Pedro",
        "Av. Carlos Calvo 1234",
        23,
        68,
    };

    showinfo(&empleados);

    printf("Tamano = %ld\n\n", sizeof(struct pru_struct));

    return 0;
}