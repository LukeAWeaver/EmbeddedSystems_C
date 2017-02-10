/*
 * main.c
 */

#include	"inc/hw_ints.h"
#include	"inc/hw_memmap.h"
#include	"inc/hw_types.h"
#include	"inc/hw_uart.h"

#include	<stddef.h>
#include	<stdbool.h>
#include	<stdint.h>
#include	<stdarg.h>
#include "driverlib/systick.h"
#include	"driverlib/sysctl.h"
#include	"driverlib/pin_map.h"
#include	"driverlib/gpio.h"
#include "Drivers/rit128x96x4.h"
#include	"Drivers/UARTStdio_Initialization.h"
#include	"Drivers/uartstdio.h"

#include	"FreeRTOS.h"
#include	"task.h"

#include	<stdio.h>

extern void Task_Blinky( void *pvParameters );
extern void Task_adc( void *pvParameters );
extern void Task_Display( void *pvParameters );
extern volatile int long xPortSysTickCount;
//*****************************************************************************
//
// The speed of the processor clock in Hertz, which is therefore the speed of the
// clock that is fed to the peripherals.
//
//*****************************************************************************
unsigned long g_ulSystemClock;

//*****************************************************************************
//
//	The number of clock cycles between SysTick interrupts. The SysTick interrupt
//	period is 0.1 mS (defined in FreeRTOSConfig.h.
//	All events in the application occur at some fraction of
//	this clock rate.
//
//*****************************************************************************
#define SysTickFrequency		configTICK_RATE_HZ

extern volatile int long xPortSysTickCount;


int main( void ) {
	char TimeString[24];
	int Task1_Status;
	int LoopCount = 0;
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ );

	//
	// Get the system clock speed.
	//
	g_ulSystemClock = SysCtlClockGet();

	UARTStdio_Initialization();

	//
	//	Create a task to blink LED
	//
	xTaskCreate( Task_Blinky, "Blinky", 128, NULL, 1, NULL );
	xTaskCreate( Task_Display, "Display", 512, NULL, 1, NULL );
	xTaskCreate( Task_adc, "ADC", 128, NULL, 1, NULL );
	UARTprintf( "FreeRTOS Starting!\n" );

	//
	//	Start FreeRTOS Task Scheduler
	//
	vTaskStartScheduler();

	while ( 1 ) {


	}

}
