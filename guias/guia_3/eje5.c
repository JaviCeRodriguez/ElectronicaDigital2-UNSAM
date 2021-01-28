// Guía 3 (Con sistema base v4)

//	5. Crear un programa en C que realice las operaciones de suma, resta, multiplicación y división de
//dos números enteros especificados en el código. La operación debe ser elegida a través de dos de
//los switches del kit y el resultado debe ser mostrado por la terminal.


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

#define switchsChannel 1

void retardo(void){
    for( int i=0;i<99999;i++);
}

int main()
{
	XGpio switchs;
	int checkSW;
	int a = 5, b = 2, res;


	// Tact switchs y Dip switchs
	XGpio_Initialize(&switchs, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&switchs, switchsChannel, 0xFFFFFFFF); // Determino que sea entrada

	for(;;){
		checkSW = XGpio_DiscreteRead(&switchs, switchsChannel); // Leo estado de switchs
		if (checkSW == 0b00000000) // Suma
			res = a + b;
		else
			if (checkSW == 0b00010000) // Resta
				res = a - b;
			else
				if (checkSW == 0b00100000) // Producto
					res = a * b;
				else
					if (checkSW == 0b00110000) // Division entera
						res = a / b;
					else // Este lo agrego para que indique que la posicion de switchs es invalida
						res = 0xffff;

		xil_printf("%d\r\n", res);
		retardo();
	}

	return 0;
}
