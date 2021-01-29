#include "xparameters.h"
#include "xgpio.h"
#include "xil_exception.h"
#include "xintc.h"
#include <stdio.h>

#define GPIO_DEVICE_ID			XPAR_GPIO_0_DEVICE_ID
#define GPIO_CHANNEL1			1

#define INTC_GPIO_INTERRUPT_ID	XPAR_INTC_0_GPIO_0_VEC_ID
#define INTC_DEVICE_ID			XPAR_INTC_0_DEVICE_ID
#define INTC					XIntc
#define INTC_HANDLER			XIntc_InterruptHandler

/************************** Function Prototypes ******************************/
void GpioHandler(void *CallBackRef);

void GpioSetupIntrSystem(INTC *IntcInstancePtr, XGpio *InstancePtr, u16 DeviceId, u16 IntrId, u16 IntrMask);

void GpioDisableIntr(INTC *IntcInstancePtr, XGpio *InstancePtr, u16 IntrId, u16 IntrMask);

/************************** Variable Definitions *****************************/

XGpio Gpio; 					/* The Instance of the GPIO Driver */

INTC Intc; 						/* The Instance of the Interrupt Controller Driver */

static u16 GlobalIntrMask;		/* GPIO channel mask that is needed by the Interrupt Handler */

static volatile u32 IntrFlag; 	/* Interrupt Handler Flag */

int main(void){

	print(" Press button to Generate Interrupt\r\n");

	XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
	GpioSetupIntrSystem(&Intc, &Gpio, GPIO_DEVICE_ID, INTC_GPIO_INTERRUPT_ID, GPIO_CHANNEL1);
//	  GpioDisableIntr(&Intc, &Gpio, INTC_GPIO_INTERRUPT_ID, GPIO_CHANNEL1);

	while(1){
		if (IntrFlag == 1){
			printf("Interrupcion activada ...\n\r");
			IntrFlag = 0;
		}
	};

//	return 0;
}

/******************************************************************************
*
* This function performs the GPIO set up for Interrupts
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller driver Instance
* @param	InstancePtr is a reference to the GPIO driver Instance
* @param	DeviceId is the XPAR_<GPIO_instance>_DEVICE_ID value from xparameters.h
* @param	IntrId is XPAR_<INTC_instance>_<GPIO_instance>_IP2INTC_IRPT_INTR value from xparameters.h
* @param	IntrMask is the GPIO channel mask
*
* @return	XST_SUCCESS if the Test is successful, otherwise XST_FAILURE
******************************************************************************/
void GpioSetupIntrSystem(INTC *IntcInstancePtr, XGpio *InstancePtr,
			u16 DeviceId, u16 IntrId, u16 IntrMask)
{
	GlobalIntrMask = IntrMask;

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 * specify the device ID that was generated in xparameters.h
	 */
	XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);

	/* Hook up interrupt service routine */
	XIntc_Connect(IntcInstancePtr, IntrId,
		      (Xil_ExceptionHandler)GpioHandler, InstancePtr);

	/* Enable the interrupt vector at the interrupt controller */
	XIntc_Enable(IntcInstancePtr, IntrId);

	/*
	 * Start the interrupt controller such that interrupts are recognized
	 * and handled by the processor
	 */
	XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);

	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_InterruptEnable(InstancePtr, IntrMask);
	XGpio_InterruptGlobalEnable(InstancePtr);

	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 (Xil_ExceptionHandler)INTC_HANDLER, IntcInstancePtr);

	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();

}

/*******************************************************************************
* This is the interrupt handler routine for the GPIO for this example.
*
* @param	CallbackRef is the Callback reference for the handler.
*
******************************************************************************/
void GpioHandler(void *CallbackRef)
{
	XGpio *GpioPtr = (XGpio *)CallbackRef;

	IntrFlag = 1;
//	xil_printf("Hola mundo!\n\r");

	/* Clear the Interrupt */
	XGpio_InterruptClear(GpioPtr, GlobalIntrMask);

}

/******************************************************************************
* This function disables the interrupts for the GPIO
*
* @param	IntcInstancePtr is a pointer to the Interrupt Controller driver Instance
* @param	InstancePtr is a pointer to the GPIO driver Instance
* @param	IntrId is XPAR_<INTC_instance>_<GPIO_instance>_VEC value from xparameters.h
* @param	IntrMask is the GPIO channel mask
*
******************************************************************************/
void GpioDisableIntr(INTC *IntcInstancePtr, XGpio *InstancePtr,
			u16 IntrId, u16 IntrMask)
{
	XGpio_InterruptDisable(InstancePtr, IntrMask);
	XIntc_Disable(IntcInstancePtr, IntrId);
	return;
}
