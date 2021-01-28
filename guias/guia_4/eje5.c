// Ejercicio 5

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#define switchs 0x40000000
#define leds 	0x40000008


void retardo(void){
    for( int i=0;i<99999;i++);
}


int main(void)
{
	init_platform();

	int estado, a = 5, b = 2;

	for(;;){
		estado = *(int *)(switchs);
		if (estado == 0b00000000) // Suma
			*(int *)(leds) = a + b;
		else
			if (estado == 0b00010000) // Resta
				*(int *)(leds) = a - b;
			else
				if (estado == 0b00100000) // Producto
					*(int *)(leds) = a * b;
				else
					if (estado == 0b00110000) // Division entera
						*(int *)(leds) = a / b;
					else // Este lo agrego para que indique que no hace operaciones
						*(int *)(leds) = 0xffff;
		xil_printf("%d\r\n", estado);
		retardo();
	}

    cleanup_platform();
}
