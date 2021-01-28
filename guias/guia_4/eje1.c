// Ejercicio 1

int main(void)
{
	int leds = 0x40000008;

	for(;;){
		*(int *)(leds) = 0x1;
	}
}
