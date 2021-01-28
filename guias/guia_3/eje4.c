// Guía 3 (Con sistema base v4)

//	4. Crear un programa en C que tome el estado de los switches y lo refleje en los leds (switch = 1
//implica led encendido, switch = 0 implica led apagado).


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

#define switchsChannel 1
#define ledsChannel 2

int main()
{
	XGpio switchs, leds;
	int checkSW;


	// Tact switchs y Dip switchs
	XGpio_Initialize(&switchs, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&switchs, switchsChannel, 0xFFFFFFFF); // Determino que sea entrada

	// LEDs
	XGpio_Initialize(&leds, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&leds, ledsChannel, 0x00000000);
	XGpio_DiscreteClear(&leds, ledsChannel, 0xFFFFFFFF);

	while (1) {
		checkSW = XGpio_DiscreteRead(&switchs, switchsChannel); // Leo estado de switchs
		XGpio_DiscreteWrite(&leds, ledsChannel, checkSW); // Muestro estado de switchs en los leds
	}

	return 0;
}
