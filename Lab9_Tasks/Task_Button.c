//*****************************************************************************
//	Task_Button.c - Blink the LED.
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

#include "driverlib/uart.h"
#include "Drivers/uartstdio.h"
#include "Drivers/UARTStdio_Initialization.h"

//*****************************************************************************
//	External variables.
//	(1) SysTick
//*****************************************************************************
extern uint32_t SysTickFrequency_Nbr;
extern uint32_t SysTickCount_Nbr;

//*****************************************************************************
//	Task local static variables.
//*****************************************************************************
static uint32_t Task_Button_NextExecute = 0;		// Next execution time
static uint32_t Task_speakerbuzz_NextExecute = 0;		// Next execution time
static uint32_t halfPeriod;			// Number of SysTicks between execution
static uint32_t Task_Button_Delay; //10ms delay
static uint32_t buttonstate = 0; //0 idle, 1 debounce, 2 steady
static uint32_t Task_button_RingTime = 0;
static bool Task_Button_Initialized = false;	// State of task initialization
static uint32_t button_value = 0;

bool sound = false;

extern void Task_Button() {
	uint32_t LED_Data1 = 0;
	uint32_t button = 0;

	UARTStdio_Initialization();
	if (!Task_Button_Initialized) {
		// Enable the GPIO Port H and G
		SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOH);
		SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOG);
		//Configure GPIO_G to drive the buttons
		GPIOPinTypeGPIOInput( GPIO_PORTG_BASE,
				GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		GPIOPadConfigSet( GPIO_PORTG_BASE,
		GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
				GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		//Configure GPIO_H to drive the speaker
		GPIOPinTypeGPIOOutput( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0);
		GPIOPadConfigSet( GPIO_PORTH_BASE,
		GPIO_PIN_1 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x01);
		//set variables
		halfPeriod = (SysTickFrequency_Nbr / (2 * 500));
		Task_Button_NextExecute = SysTickCount_Nbr
				+ SysTickFrequency_Nbr / 1000;
		Task_speakerbuzz_NextExecute = SysTickCount_Nbr + halfPeriod;
		Task_Button_DeltaTimeout = (750 * SysTickFrequency_Nbr) / 1000;
		Task_Button_Delay = (10 * SysTickFrequency_Nbr) / 1000;
		Task_button_RingTime = 0;
		//	Task is initialized.
		Task_Button_Initialized = true;
	}

	if (SysTickCount_Nbr >= Task_Button_NextExecute) //updates every 1ms
			{
		button = GPIOPinRead( GPIO_PORTG_BASE,
				GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		if (buttonstate == 0) //idle state
				{
			if (button != 0xF8) // if button pressed
					{
				buttonstate = 1; //sets button to debounce
				Task_Button_NextExecute = SysTickCount_Nbr + Task_Button_Delay; //adds 10ms delay before button state is checked again
			}
		} else if (buttonstate == 1) // debounce state
				{
			if (button != 0xF8) // if button pressed
					{
				Task_button_RingTime = SysTickFrequency_Nbr / 5
						+ SysTickCount_Nbr; //sets ringtime to be 200ms
				Task_Button_NextExecute = SysTickCount_Nbr + Task_Button_Delay; //adds 10ms delay before button state is checked again
				buttonstate = 2;

				if (button == 0x78) {
					UARTprintf("Select Button pressed\n");
					button_value = 1;
				} else if (button == 0xB8) {
					UARTprintf("Right Button pressed\n");
					button_value = 2;
				}
				else if (button == 0xD8) {
									UARTprintf("Left Button pressed\n");
									button_value = 3;
								}
				else if (button == 0xF0) {
									UARTprintf("Up Button pressed\n");
									button_value = 4;
								}
				else if (button == 0xE8) {
									UARTprintf("Down Button pressed\n");
									button_value = 5;
								}
			}
		} else if (buttonstate == 2) //steady state
				{
			Task_Button_NextExecute = SysTickCount_Nbr
					+ SysTickFrequency_Nbr / 1000; //adds a 1ms delay before checking button state again
			if (button == 0xf8) //when button is released
					{
				Task_button_RingTime = SysTickFrequency_Nbr / 2
						+ SysTickCount_Nbr; //rings for 500ms
				buttonstate = 0; //sets button to idle state

				if (button_value == 1){
					UARTprintf("Select Button Released\n");
				} else if (button_value == 2) {
					UARTprintf("Right Button Released\n");
					button_value = 2;
				}
				else if (button_value == 3) {
									UARTprintf("Left Button Released\n");
									button_value = 3;
								}
				else if (button_value == 4) {
									UARTprintf("Up Button Released\n");
									button_value = 4;
								}
				else if (button_value == 5) {
									UARTprintf("Down Button Released\n");
									button_value = 5;
								}
			}
		}
	}

//Tone
	if (SysTickCount_Nbr <= Task_button_RingTime) //rings until time catches up (200 ms)
			{
		if (SysTickCount_Nbr >= Task_speakerbuzz_NextExecute)      //<frequency>
				{
			//read
			LED_Data1 = GPIOPinRead( GPIO_PORTH_BASE, GPIO_PIN_1 | GPIO_PIN_0);
			LED_Data1 ^= 0b11; //makes more sense when written in this way. flipping 11 bits.
			//write
			GPIOPinWrite( GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, LED_Data1);
			Task_speakerbuzz_NextExecute = SysTickCount_Nbr + halfPeriod;//</frequency>
		}
	}
}
