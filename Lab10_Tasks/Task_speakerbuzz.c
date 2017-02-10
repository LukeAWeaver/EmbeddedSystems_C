//*****************************************************************************
//
//	Task_speakerbuzz.c - Blink the LED.
//
//		Author: 		Gary J. Minden
//		Organization:	KU/EECS/EECS 388
//		Date:			2016-08-16 (B60816)
//		Version:		1.0
//
//		Purpose:		Example task that demonstrates:
//							(1) setting up a GPIO pin (LEC)
//							(2) blinking (toggling) a LED
//
//		Notes:			Updated at KU and adapted from the TI LM3S1968 blinky
//						and other examples.
//
//		Date:			2016-08-16 (B60816)
//						Moved task code to separate files.
//						Changed structure of task code. See individual tasks.
//						Updated basic types.
//
//*****************************************************************************
//
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

//*****************************************************************************
//
//	External variables.
//	(1) SysTick
//
//*****************************************************************************
extern uint32_t		SysTickFrequency_Nbr;
extern uint32_t 	SysTickCount_Nbr;

//*****************************************************************************
//
//	Task local static variables.
//
//*****************************************************************************



static uint32_t Task_speakerbuzz_NextExecute = 0;		// Next execution time
static uint32_t halfPeriod;			// Number of SysTicks between execution
static uint32_t Task_speakerbuzz_NextTimeout = 0;
static uint32_t Task_speakerbuzz_DeltaTimeout;

static bool which_tone = true; //true for tone 1, false for tone 2

static bool		Task_speakerbuzz_Initialized = false;	// State of task initialization

extern void Task_speakerbuzz() {

	uint32_t LED_Data1 = 0;



	if ( !Task_speakerbuzz_Initialized ) {
	    //
	    // Enable the GPIO Port H.
	    //
	    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOH );
	     //
	     // Configure GPIO_H to drive the Speaker.
	     //

	    //
	    // Configure GPIO_H to drive the Status LED.
	    //
	    GPIOPinTypeGPIOOutput( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0 );
	    GPIOPadConfigSet( GPIO_PORTH_BASE,
	    					GPIO_PIN_1 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );

	    //
		//	The delta between executions is 50 mS.
		//	The initial time to execute is Delta from now.
	    //
        GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x01 );

	    halfPeriod  = (SysTickFrequency_Nbr  / (2 * 500) );
		Task_speakerbuzz_NextExecute = SysTickCount_Nbr + halfPeriod;


	    Task_speakerbuzz_DeltaTimeout  = (750 * SysTickFrequency_Nbr) / 1000 ;
		Task_speakerbuzz_NextTimeout = SysTickCount_Nbr + Task_speakerbuzz_DeltaTimeout;
		//
		//	Task is initialized.
		//
		Task_speakerbuzz_Initialized = true;

	}

	if (SysTickCount_Nbr >= Task_speakerbuzz_NextTimeout) {
		if (which_tone) //if tone 1 is playing
			halfPeriod  = (SysTickFrequency_Nbr  / (2 * 200) );
		else //if tone 2 is playing
			halfPeriod  = (SysTickFrequency_Nbr  / (2 * 500) );
		which_tone ^= 1;

		Task_speakerbuzz_NextTimeout = SysTickCount_Nbr + Task_speakerbuzz_DeltaTimeout;
	}
//*****************************************************************************
//
//	Task execution.
//
//*****************************************************************************

	if ( SysTickCount_Nbr >= Task_speakerbuzz_NextExecute) {

        //
        // Toggle the LED.
        //
		//read
		LED_Data1 = GPIOPinRead( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0);
		LED_Data1 ^= 0b11; //makes more sense when written in this way. flipping 11 bits.

        //write

        GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, LED_Data1 );
        //
        //	Advance Task_speakerbuzz_NextExecute time.
        //

        Task_speakerbuzz_NextExecute  = SysTickCount_Nbr + halfPeriod;
	}


}
