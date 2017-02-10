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
//include pwm
#include "driverlib/pwm.c"
#include "driverlib/pwm.h"

#include "Drivers/rit128x96x4.h"
#include	"Drivers/UARTStdio_Initialization.h"
#include	"Drivers/uartstdio.h"

#include	"FreeRTOS.h"
#include	"task.h"

#include	<stdio.h>

static bool isPulse1 = true;
static bool Task_Initialized = false;

int pulse = 1;
extern unsigned long ulValue;


void Task_PWMSignal(void *pvParameters) {
	if (!Task_Initialized) {

		SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM0); //enable PWM prpheral
	   // GPIOPinConfigure(GPIO_PD0_PWM1);
		GPIOPinTypeGPIOOutput( GPIO_PORTD_BASE, GPIO_PIN_1);
		GPIOPadConfigSet( GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA,
		GPIO_PIN_TYPE_STD);
		GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);


//gen, pintype,
		Task_Initialized = true;

	    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN |
	                    PWM_GEN_MODE_NO_SYNC); //configure pwm
		// 50/0.00005
		//clock divider,
	    SysCtlPWMClockSet(SYSCTL_PWMDIV_64); //50,000,000 -> 781250/50
		PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 15625); //set pwm period
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1,
				PWMGenPeriodGet(PWM0_BASE, PWM_OUT_1) / 20); //set width to 1ms

	    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
		PWMGenEnable(PWM0_BASE, PWM_GEN_0); //enable pwm

	}
//end initialization

	while (1) {
		if (isPulse1 == true)
		{
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 781); //set width to 1ms
			isPulse1 = false;

		}

		else if (isPulse1 == false)
		{
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ulValue); //set width to 2ms

		}
		vTaskDelay( ( configTICK_RATE_HZ * 10000 ) / 10000 );

	}
}
