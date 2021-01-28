// Guía 3 (Con sistema base v4)

//	2. Tomando de base el programa anterior escribir uno nuevo que prenda
//y apague un led indefinidamente.

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

#define ledCommonCH 2
#define ledRGBCH 2

void retardo(){
	for (int i=1;i<250000;i++){}
}

int main()
{
	XGpio leds;

	// Leds comunes
	XGpio_Initialize(&leds, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, ledCommonCH, 0x00000000); // Determino que es salida con ceros
	while(1){
		XGpio_DiscreteWrite(&leds, ledCommonCH, 0x00000000); // Ingreso todo 0 hexadecimal
		retardo();
		XGpio_DiscreteWrite(&leds, ledCommonCH, 0x0000FFFF); // Ingreso FFFF para prender todos los leds
		retardo();
	}

	return 0;
}
