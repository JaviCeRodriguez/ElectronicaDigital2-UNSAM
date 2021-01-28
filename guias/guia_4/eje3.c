// Ejercicio 3
// btn0 => 1
// btn1 => 2
// btn2 => 4
// btn3 => 8
// sw0 => 16
// sw1 => 32
// sw2 => 64
// sw3 => 128

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#define switchs 0x40000000


void retardo(void){
    for( int i=0;i<99999;i++);
}


int main(void)
{
	init_platform();
	printf("Probando...\n");

	int estado = 0;

	for(;;){
		estado = *(int *)(switchs);
		xil_printf("%d\r\n", estado);
		retardo();
	}

    cleanup_platform();
}

