// Ejercicio 2

#define leds 0x40000008


void retardo(void){
    for( int i=0;i<99999;i++);
}


int main(void)
{
	for(;;){
		*(int *)(leds) = 0x0;
		retardo();
		*(int *)(leds) = 0x1;
		retardo();
	}
}
