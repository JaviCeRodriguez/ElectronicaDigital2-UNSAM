//Practica 3

//7. Crear un programa en C que reciba bytes (caracteres presionados en el teclado) a través de una
//conexión serie (UART) y los muestre a través de la terminal y de los leds.


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

//#include "xstatus.h"
//#include "xuartlite_l.h"


#define RGB_LEDS_CHANNEL 2

int main(void)
{

	XGpio leds;

	//led RGB
	XGpio_Initialize(&leds, XPAR_AXI_GPIO_1_DEVICE_ID);
	XGpio_SetDataDirection(&leds, RGB_LEDS_CHANNEL, 0x00000000);
	XGpio_DiscreteClear(&leds, RGB_LEDS_CHANNEL, 0xFFFFFFFF);

	int aux;
	int *RXD = (int *) (XPAR_AXI_UARTLITE_0_BASEADDR);
	int *status = (int *) (XPAR_AXI_UARTLITE_0_BASEADDR + 0x00000008);

	while (1){


		if (*status & 0x00000001){
			aux = *RXD;
			xil_printf("Caracter recibido %c \r\n", aux);
			XGpio_DiscreteWrite(&leds, RGB_LEDS_CHANNEL, aux); //valores entre 0 y F, 4 bits
		}


	}


	return 0;
}
