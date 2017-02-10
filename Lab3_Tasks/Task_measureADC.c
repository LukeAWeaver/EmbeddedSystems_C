/*--Task_adc.c
 *
 *  Author:			Gary J. Minden
 *	Organization:	KU/EECS/EECS 388
 *  Date:			2016-09-26 (B60926)
 *
 *  Description:	Blinks Status LED on Stellaris LM3S1968
 *					Evaluation board.
 *
 */

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

#include "Drivers/uartstdio.h"

#include	"FreeRTOS.h"
#include	"task.h"
//*****************************************************************************
//
//	External variables.
//
//*****************************************************************************
//extern unsigned long ulValue = 0;

static uint32_t buttonstate = 0; //0 idle, 1 debounce, 2 steady
static bool Task_Button_Initialized = false;	// State of task initialization
static uint32_t voltage[100];
static uint32_t button = 0;
unsigned long ulValue = 0;
int vol=0;
//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************

extern void Task_measureADC(void *pvParameters) {

	if (!Task_Button_Initialized) {

		SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOG); // Enable the GPIO Port G.
		GPIOPinTypeGPIOInput( GPIO_PORTG_BASE, GPIO_PIN_7);	//Configure GPIO_G to drive the buttons
		GPIOPadConfigSet( GPIO_PORTG_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIOPinWrite( GPIO_PORTD_BASE, GPIO_PIN_2, 0x04 );
		//end button configs

		//port for voltage
		SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD);
		GPIOPinTypeGPIOOutput( GPIO_PORTD_BASE, GPIO_PIN_2);
		GPIOPadConfigSet( GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_OD);

		//ADC INITIALIZE/CONFIG
		SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0); //ENABLE ADC0
		ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
				ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
		ADCSequenceEnable(ADC0_BASE, 0);

		Task_Button_Initialized = true;
	}
//end initialization
//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************

	while (1) {
		button = GPIOPinRead( GPIO_PORTG_BASE, GPIO_PIN_7);
		if (buttonstate == 0) //idle state
				{
			if (button == 0x00) // if button pressed
					{
				buttonstate = 1; //sets button to debounce
				SysCtlDelay(166666); //166,666
			}
		} else if (buttonstate == 1) // debounce state
				{
			//	Delay 10 mS.
			if (button == 0x00) // if button pressed
					{
				buttonstate = 2;
				if (button == 0x00) {
					UARTprintf("Select Button pressed\n");
				}
			}
		} else if (buttonstate == 2) //steady state
				{
			if (button == 0x80) //when button is released
					{
				buttonstate = 0; //sets button to idle state
				GPIOPinWrite( GPIO_PORTD_BASE, GPIO_PIN_2, 0b00);
				vTaskDelay(( configTICK_RATE_HZ * 500*10) / 10000);//500ms
				GPIOPinWrite( GPIO_PORTD_BASE, GPIO_PIN_2, 0x04);
				uint32_t i = 0;
				for (i = 0; i < 100; i++) {
					vol = 0;
					ADCProcessorTrigger(ADC0_BASE, 0); //START
					while (!ADCIntStatus(ADC0_BASE, 0, false))
						; //wait
					ADCSequenceDataGet( ADC0_BASE, 0, &ulValue); //read
					//UARTprintf("Voltage: %d\n", i);
					vol = ulValue*3300/1023;
					voltage[i] = vol;
					ADCIntClear(ADC0_BASE, 0);
					//sample every 1 ms = 1,000,000 nanoseconds
					//x*60 (nanoseconds) = 1,000,000, x=16666
					SysCtlDelay(16666); //16,000
				}
				for (i = 0; i < 100; i++) {
					UARTprintf("Time: %04d ms; Voltage: %d mv\n", i, voltage[i]); //over span of 100ms, 1ms -> 2ms -> .. -> 100ms
				}
			}
		}
		vTaskDelay(( configTICK_RATE_HZ * 10) / 10000);

	} //end while loop
} //end class

