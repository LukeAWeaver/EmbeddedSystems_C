/*
 * Task_ReportSysTick.c
 *
 *  Created on: Oct 6, 2016
 *      Author: lweaver
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

void Task_Display( void *pvParameters ) {

char TimeString[24];
char ADCString[24];
int Task1_Status;
extern volatile int long xPortSysTickCount;
uint32_t TaskStartTime;
extern unsigned long ulValue;
 //
 //Initialize the OLED display and write status.
 //
 RIT128x96x4Init( 1000000 );
 RIT128x96x4StringDraw( "Blinky", 36, 85, 8 );
 //
 //Set up periodic execution
 //
 TaskStartTime = xTaskGetTickCount();

 while ( 1 ) {
 sprintf( ADCString, "ADC: %04d", ulValue );
 Task1_Status = sprintf( TimeString, "Time: %08d", xPortSysTickCount );
 Task1_Status = Task1_Status;
 RIT128x96x4StringDraw( TimeString, 0, 74, 15);
 RIT128x96x4StringDraw( ADCString, 0, 58, 15);

 vTaskDelayUntil( &TaskStartTime, 500 ); // Delay 500 ticks from TaskStartTime;
 // reset TaskStartTime
 }
}
