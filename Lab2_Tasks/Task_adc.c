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

#include	"FreeRTOS.h"
#include	"task.h"
//*****************************************************************************
//
//	External variables.
//
//*****************************************************************************
extern unsigned long ulValue = 0;

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************

extern void Task_adc( void *pvParameters ) {

	uint32_t LED_Data = 0;
//youtu.be/h6FhV693seo
	    //
	    // Enable the GPIO Port G.
	    //
	    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOG );

	    //
	    // Configure GPIO_G to drive the Status LED.
	    //
	    GPIOPinTypeGPIOOutput( GPIO_PORTG_BASE, GPIO_PIN_2 );
	    GPIOPadConfigSet( GPIO_PORTG_BASE,
	    					GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );

	//new code
	//ADC INITIALIZE/CONFIG

SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 ); //ENABLE ADC0
ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 1);
ADCSequenceStepConfigure(ADC0_BASE,0,0,ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
ADCSequenceEnable(ADC0_BASE,0);

ADCIntClear(ADC0_BASE,0);




//end new code

//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************

	while (1) {
		ADCProcessorTrigger(ADC0_BASE,0); //START
		while(!ADCIntStatus(ADC0_BASE,0,false)); //wait

		ADCSequenceDataGet( ADC0_BASE, 0, &ulValue);//read
		ADCIntClear(ADC0_BASE,0);

        //
        // Toggle the LED.
        //
//		LED_Data = GPIOPinRead( GPIO_PORTG_BASE, GPIO_PIN_2 );
//		LED_Data = LED_Data ^ 0x04;
//        GPIOPinWrite( GPIO_PORTG_BASE, GPIO_PIN_2, LED_Data );

        //
        //	Delay 1000 mS.
        //
		vTaskDelay( ( configTICK_RATE_HZ * 1000 ) / 10000 );

	}
}



