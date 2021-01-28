// Ejercicio 4

int main(void)
{
	int switchs = 0x40000000;
	int leds = 0x40000008;

	for(;;){
		*(int *)(leds) = *(int *)(switchs);
	}
}
