// Guía 3 (Con sistema base v4)

//	3. Crear un programa en C que tome el estado de los switches y lo muestre por una terminal.
//En este caso se deberá agregar el uso de la función XGpio_DiscreteRead


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

#define switchsChannel 1

int main()
{
	XGpio switchs;
	int checkSW, stateSW;


	// Tact switchs y Dip switchs
	XGpio_Initialize(&switchs, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&switchs, switchsChannel, 0xFFFFFFFF); // Determino que sea entrada

	checkSW = XGpio_DiscreteRead(&switchs, switchsChannel); // Leo estado de switchs
	xil_printf("Push buttons status: %x \r\n", checkSW);

	stateSW = checkSW;

	while (1) {
		if (stateSW != checkSW){
			xil_printf("Push buttons status: %d \r\n", checkSW); // Imprime el valor del sw 2^pos de bit
			stateSW = checkSW;
		}
		checkSW = XGpio_DiscreteRead(&switchs, switchsChannel); // Leo estado de switchs
	}

	return 0;
}