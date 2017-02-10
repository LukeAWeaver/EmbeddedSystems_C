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
int pulse = 1;
void Task_PWMSignal(void *pvParameters) {
	if (!Task_Initialized) {

		SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM); //enable PWM prpheral
		GPIOPinTypeGPIOOutput( GPIO_PORTD_BASE, GPIO_PIN_1);
		GPIOPadConfigSet( GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
		GPIO_PIN_TYPE_OD);

		//ADC INITIALIZE/CONFIG
		SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0); //ENABLE ADC0
		ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
		ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
				ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
		ADCSequenceEnable(ADC0_BASE, 0);

		Task_Initialized = true;
		PWMGenEnable(PWM1, PWM_GEN_0) //enable pwm
		PWMGenConfigure(PWM1, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN) //configure pwm
		// 16/0.00002
		PWMGenPeriodSet(PWM1, PWM_GEN_0, 800000) //set pwm period

	}
//end initialization

	while (1) {
		if (isPulse1 = true)
		{
			PWMPulseWidthSet(PWM1, PWM_GEN_0,
					PWMGenPeriodGet(PWM_BASE, PWM_OUT_0) / 20) //set width to 1ms
			ispulse1 = false;
		} else if (isPulse1 = false)
		{
			PWMPulseWidthSet(PWM1, PWM_GEN_0, 2
					PWMGenPeriodGet(PWM_BASE, PWM_OUT_0) / 10) //set width to 2ms
			isPulse1 = true;
		}

	}
}
