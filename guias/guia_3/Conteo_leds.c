//Practica 3

//6. Crear un programa en C que realice un conteo desde 0 a 15 indefinidamente mostrando el
//resultado en los leds RGB (se deberá reducir la velocidad de conteo para que se puedan observar
//los cambios). Utilizar uno de los switches para habilitar/deshabilitar la cuenta y uno de los botones
//para reiniciar la cuenta.

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"

#define RGB_LEDS_CHANNEL 2
#define BUTTONS_CHANNEL 1
#define SWITCHS_CHANNEL 2
#define COMMON_LEDS_CHANNEL 1

int main()
{
	XGpio leds_RGB, dip, push, leds;
	int push_check, dip_check;
	int aux_RGB = 0x0;

	//Led (blanqueo)
	XGpio_Initialize(&leds, XPAR_AXI_GPIO_1_DEVICE_ID);
	XGpio_SetDataDirection(&leds, COMMON_LEDS_CHANNEL, 0x00000000); //si es salida todo 000 si es entrada todo FFF
	XGpio_DiscreteClear(&leds, COMMON_LEDS_CHANNEL, 0xFFFFFFFF);


	//Led RGB
	XGpio_Initialize(&leds_RGB, XPAR_AXI_GPIO_1_DEVICE_ID);
	XGpio_SetDataDirection(&leds_RGB, RGB_LEDS_CHANNEL, 0x00000000);
	XGpio_DiscreteClear(&leds_RGB, RGB_LEDS_CHANNEL, 0xFFFFFFFF);

	XGpio_DiscreteWrite(&leds_RGB, RGB_LEDS_CHANNEL, aux_RGB);

	//Buttoms
	XGpio_Initialize(&push, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&push, BUTTONS_CHANNEL, 0xFFFFFFFF);

	//Switchs
	XGpio_Initialize(&dip, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&dip, SWITCHS_CHANNEL, 0xFFFFFFFF);


	while (1) {
		aux_RGB = 0x0;
		XGpio_DiscreteWrite(&leds_RGB, RGB_LEDS_CHANNEL, aux_RGB);

		for (int i=1;i<200000;i++) {
			}

		while (aux_RGB<0xF){
			push_check = XGpio_DiscreteRead(&push, BUTTONS_CHANNEL);

			if (push_check == 0x1){
				aux_RGB = 0x0;
				XGpio_DiscreteWrite(&leds_RGB, RGB_LEDS_CHANNEL, 0x0);
			}
			dip_check = XGpio_DiscreteRead(&dip, SWITCHS_CHANNEL);

			if (dip_check == 0x1 && push_check != 0x1){ //se podria poner para que si estan todos en 1 tambien funque
				aux_RGB = aux_RGB + 0x1;
				XGpio_DiscreteWrite(&leds_RGB, RGB_LEDS_CHANNEL, aux_RGB);
			}

			for (int i=1;i<200000;i++) {
			}

		}
	}

	return 0;
}
