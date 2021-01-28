// Guía 3 (Con sistema base v4)

//	1. Crear un programa en C que prenda un solo led del kit utilizando las funciones XGpio_Initialize,
//XGpio_SetDataDirection y XGpio_DiscreteWrite.

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

#define ledCommonCH 2
#define ledRGBCH 2

int main()
{
	XGpio leds;

	// Leds comunes
	XGpio_Initialize(&leds, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, ledCommonCH, 0x00000000); // Determino que es salida con ceros
	XGpio_DiscreteWrite(&leds, ledCommonCH, 0x00000020); // Ingreso 20 hexadecimal a los leds (LD0 green)

	return 0;
}
