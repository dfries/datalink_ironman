/* Copyright (c) 2015 David Fries <David@Fries.net>  all rights reserved
 */

/* microcontroller blinker program for the Teensy 3.0 ARM based kinetis
 * based microcontroller.  Emulates a USB serial port and the blinks the
 * built in LED at the correct rate for the datalink serial protocol
 */

// arm_math.h sets macros and includes core_cmInstr.h
#include "cores/teensy3/arm_math.h"

#include "cores/teensy3/kinetis.h"
#include "cores/teensy3/usb_serial.h"
#include "cores/teensy3/usb_dev.h"
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

void usb_serial_printf(const char *__restrict format, ...)
	__attribute__ ((__format__ (__printf__, 1, 2)));

void usb_serial_printf(const char *__restrict format, ...)
{
	// better be enough
	char buf[320];
	va_list ap;
	va_start(ap, format);
	int size = vsnprintf(buf, sizeof(buf)-1, format, ap);
	buf[size] = 0;
	va_end(ap);
	usb_serial_write(buf, size);
}

// 1 byte to distinguish full/empty leaves 128 to match USB packet size of 64
static char serial_data[129];
// indices into serial_data, next index to put (from USB) or get data, equal is
// empty
static uint8_t serial_get, serial_put;

#ifndef USB_RX_CB_AVAILABLE
#error usb_rx_cb not available, this program will not function
#endif
/* If accepted upstream this is an extern "C" call in usb_dev.h which is
 * called each time a non-zero data packet is received.
 */
void usb_rx_cb(void)
{
	// run until full or no more data
	while(usb_serial_available())
	{
		uint8_t size;
		if(serial_get > serial_put)
		{
			size = serial_get - serial_put - 1;
		}
		else
		{
			size = sizeof(serial_data) - serial_put;
			// if get is at the start, stop early
			if(size && serial_get == 0)
				--size;
		}

		/*
		while(!usb_serial_write_buffer_free())
			;
		usb_serial_printf("p %u g %u size %u\n", serial_put, serial_get,
			size);
			*/
		if(!size)
			break;

		int r = usb_serial_read(serial_data + serial_put, size);
		if(!r)
			break;

		//LED_TOGGLE;
		serial_put = (serial_put + r) % sizeof(serial_data);
	}
	if(serial_get != serial_put)
		NVIC_ENABLE_IRQ(IRQ_FTM0);
}

void ftm0_isr()
{
	if(FTM0_SC & FTM_SC_TOF)
	{
		// clear interrupt flag (write 0) Do you need to?
		//FTM0_SC &= ~FTM_SC_TOF;

		if(serial_get == serial_put)
		{
			NVIC_DISABLE_IRQ(IRQ_FTM0);
			// if the buffer filled up there could be data buffered
			// and no interrupts move it to the local buffer
			usb_rx_cb();
			if(serial_get == serial_put)
			{
				LED_OFF;
				return;
			}
		}
		char b = serial_data[serial_get];
		serial_get = (serial_get + 1) % sizeof(serial_data);
		if(b & 1)
		{
			LED_ON;
			usb_serial_write("*", 1);
		}
		else
		{
			LED_OFF;
			usb_serial_write("-", 1);
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

	// 0xffff disables Modulo (top) value and center PWM mode causing
	// the countet revert to a free running up only counter.
	//FTM0_MOD = 0xffff - 1;
	// seems to not match for values 32768 and up, as in a value less
	// than FTM0_MOD but more than 32768 gives fully on when it should
	// be almost off (using PWM_Clear mode)
	FTM0_MOD = F_CPU / (115200 / 9);
	// CPWMS center aligned (count up/down)
	// use FTM::prescale_1 which is also 0
	// TOIE Timer Overflow Interrupt Enable
	FTM0_SC = FTM_SC_TOIE | FTM_SC_CLKS(FTM::clk_system);
	// also enable irq on controller (but no data, initially disable)
	//NVIC_ENABLE_IRQ(IRQ_FTM0);

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
