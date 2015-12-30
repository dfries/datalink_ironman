/* Copyright (c) 2015 David Fries <David@Fries.net>  all rights reserved
 */

// arm_math.h sets macros and includes core_cmInstr.h
#include "cores/teensy3/arm_math.h"

#include "cores/teensy3/kinetis.h"
#include "cores/teensy3//usb_serial.h"
#include <string.h>
#include <stdio.h>

#include "teensy_helper/kinetis_extra.h"
#include "teensy_helper/bit_extra.h"
#include "teensy_helper/kinetis_power.h"

// TODO remove extras
extern "C" void yield() {}

// built in LED PTC5
#define LED_CONFIG {PORTC_PCR5 = PORT_PCR_MUX(PORT::MUX_ALT_GPIO) | \
	PORT_PCR_DSE | PORT_PCR_SRE; \
	GPIOC_PDDR |= _BV(5);}
#define LED_ON     GPIOC_PSOR = _BV(5)
#define LED_OFF    GPIOC_PCOR = _BV(5)
#define LED_TOGGLE GPIOC_PTOR = _BV(5)

extern "C" {
void usb_rx_cb(void)
{
	int bindex = 0;
	char buf[256];
	int r = usb_serial_read(buf + bindex, sizeof(buf) -1 - bindex);
	if(r)
	{
		char out[12];
		int bytes = sprintf(out, "%u\n", r);
		usb_serial_write(out, bytes);
		LED_TOGGLE;
	}
}
}

int main(void)
{
	// indicate starting to run
	LED_CONFIG;
	LED_ON;
	// TODO: call from an interrupt timer, or put it to sleep
	//delay(30);
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_ON;
	LED_OFF;
	//delay(30);
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_ON;
	//delay(90);
	for(int i = 0; i<F_CPU / 22; ++i)
		LED_ON;
	LED_OFF;

	// power management, disable some parts
	Disable_RTC();
	Disable_FTM1();
	Disable_Analog();
	Disable_Serial();
	Disable_SysTick();

	// enable the FTM specific registers, which is the second set,
	// except I'm not figuring out where the first set ends and
	// the second begins...
	//FTM0_MODE |= FTM_MODE_FTMEN;

	FTM0_SC = 0;
	// 0xffff disables Modulo (top) value and center PWM mode causing
	// the countet revert to a free running up only counter.
	//FTM0_MOD = 0xffff - 1;
	// seems to not match for values 32768 and up, as in a value less
	// than FTM0_MOD but more than 32768 gives fully on when it should
	// be almost off (using PWM_Clear mode)
	FTM0_MOD = F_CPU / (115200 / 9);
	// CPWMS center aligned (count up/down)
	// use FTM::prescale_1 which is also 0
	FTM0_SC = FTM_SC_CLKS(FTM::clk_system);

	// slower initially for debugging
	FTM0_SC |= FTM_SC_PS(FTM::prescale_128);

	// PORT_PCR_DSE enable high drive strength, data sheet gives values
	// PORT_PCR_SRE enable slow slew rate ?  slower, no
	// PORT_PCR_ODE Open Drain Enabled

	for(;;)
	{
#if 0
		LED_OFF;
		// Wait for Interrupt, sleep mode
		__WFI();
		LED_ON;
#else
#warning sleep disabled
#endif
	}
}
