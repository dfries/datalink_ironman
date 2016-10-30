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

#include "kinetis_helper/kinetis_extra.h"
#include "kinetis_helper/bit_extra.h"
#include "kinetis_helper/kinetis_power.h"

// TODO remove extras
extern "C" void yield() {}

#define IRQ_CB
//#define EXTERNAL_LED

#ifndef USB_RX_CB_AVAILABLE
#warning usb_rx_cb not available, will poll USB
#undef IRQ_CB
#endif

// built in LED PTC5
#define LED_CONFIG {PORTC_PCR5 = PORT_PCR_MUX(PORT::MUX_ALT_GPIO) | \
	PORT_PCR_DSE | PORT_PCR_SRE; \
	GPIOC_PDDR |= _BV(5);}
#define LED_ON     GPIOC_PSOR = _BV(5)
#define LED_OFF    GPIOC_PCOR = _BV(5)
#define LED_TOGGLE GPIOC_PTOR = _BV(5)

#ifdef EXTERNAL_LED
// LEDs are common anode and inverted, switch set/clear
#define LED_RED0_ON		GPIOC_PCOR = _BV(4)
#define LED_RED0_OFF		GPIOC_PSOR = _BV(4)
#define LED_RED0_TOGGLE		GPIOC_PTOR = _BV(4)

#define LED_GREEN0_ON		GPIOD_PCOR = _BV(4)
#define LED_GREEN0_OFF		GPIOD_PSOR = _BV(4)
#define LED_GREEN0_TOGGLE	GPIOD_PTOR = _BV(4)

#define LED_BLUE0_ON		GPIOD_PCOR = _BV(7)
#define LED_BLUE0_OFF		GPIOD_PSOR = _BV(7)
#define LED_BLUE0_TOGGLE	GPIOD_PTOR = _BV(7)

#define LED_RED1_ON		GPIOD_PCOR = _BV(5)
#define LED_RED1_OFF		GPIOD_PSOR = _BV(5)
#define LED_RED1_TOGGLE		GPIOD_PTOR = _BV(5)

#define LED_GREEN1_ON		GPIOD_PCOR = _BV(6)
#define LED_GREEN1_OFF		GPIOD_PSOR = _BV(6)
#define LED_GREEN1_TOGGLE	GPIOD_PTOR = _BV(6)

#define LED_BLUE1_ON		GPIOC_PCOR = _BV(1)
#define LED_BLUE1_OFF		GPIOC_PSOR = _BV(1)
#define LED_BLUE1_TOGGLE	GPIOC_PTOR = _BV(1)

static uint8_t g_color;
#endif

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

/* If accepted upstream this is an extern "C" call in usb_dev.h which is
 * called each time a non-zero data packet is received.
 */
#ifdef IRQ_CB
void usb_rx_cb(void)
#else
void rx_poll(void)
#endif
{
	#ifdef RX_IRQ_DISABLE
	int irq_disabled;
	__irq_status(irq_disabled);
	__disable_irq();
	#endif
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
		usb_serial_printf("p %u g %u size %u\n", serial_put, serial_get,
			size);
			*/
		if(!size)
		{
			break;
		}

		int r = usb_serial_read(serial_data + serial_put, size);
		if(!r)
		{
			break;
		}

		// for interactive use, only looks at the first character
		// The datalink rs-232 protocol only sends 0xff and 0x00
		// bytes, having a few debug characters doesn't interfere
		if(serial_data[serial_put] == 'm')
			usb_serial_printf("FTM0_MOD %ld\n", FTM0_MOD);
		if(serial_data[serial_put] == 's')
			usb_serial_printf("FTM0_SC 0x%lx\n", FTM0_SC);
#ifdef EXTERNAL_LED
		uint8_t old = g_color;
		if(serial_data[serial_put] == 'a')
			g_color = 0;
		if(serial_data[serial_put] == 'r')
			g_color = 1;
		if(serial_data[serial_put] == 'g')
			g_color = 2;
		if(serial_data[serial_put] == 'b')
			g_color = 3;
		if(serial_data[serial_put] == 'R')
			g_color = 4;
		if(serial_data[serial_put] == 'G')
			g_color = 5;
		if(serial_data[serial_put] == 'B')
			g_color = 6;
		if(g_color != old)
		{
			LED_OFF;
			LED_RED0_OFF;
			LED_GREEN0_OFF;
			LED_BLUE0_OFF;
			LED_RED1_OFF;
			LED_GREEN1_OFF;
			LED_BLUE1_OFF;
		}
#endif
		if(serial_data[serial_put] == 'i' && r == 1)
		{
			LED_ON;
			--r;
		}
		if(serial_data[serial_put] == 'o' && r == 1)
		{
			LED_OFF;
			--r;
		}

		serial_put = (serial_put + r) % sizeof(serial_data);
	}
	if(serial_get != serial_put)
	{
		NVIC_ENABLE_IRQ(IRQ_FTM0);
	}

	// must restore because it's called from ftm0_isr
	#ifdef RX_IRQ_DISABLE
	if(!irq_disabled)
	{
		__enable_irq();
	}
	#endif
}

void ftm0_isr()
{
	int irq_disabled;
	__irq_status(irq_disabled);
	__disable_irq();
	do {
	if(FTM0_SC & FTM_SC_TOF)
	{
		// clear interrupt flag, it will keep triggering until you do
		FTM0_SC &= ~FTM_SC_TOF;

		if(serial_get == serial_put)
		{
			NVIC_DISABLE_IRQ(IRQ_FTM0);
#ifdef EXTERNAL_LED
			switch(g_color)
			{
			case 0:
				LED_OFF;
				break;
			case 1:
				LED_RED0_OFF;
				break;
			case 2:
				LED_GREEN0_OFF;
				break;
			case 3:
				LED_BLUE0_OFF;
				break;
			case 4:
				LED_RED1_OFF;
				break;
			case 5:
				LED_GREEN1_OFF;
				break;
			case 6:
				LED_BLUE1_OFF;
				break;
			}
#else
			LED_OFF;
#endif

			// if the buffer filled up there could be data buffered
			// and no interrupts move it to the local buffer
			#ifdef IRQ_CB
			// only check when not polling
			usb_rx_cb();
			#endif
			if(serial_get == serial_put)
			{
				break;
			}
		}
		/*
		usb_serial_printf("%u %u %c\n", serial_put, serial_get,
			serial_data[serial_get] & 1 ? '*' : '-');
		*/
		char b = serial_data[serial_get];
		serial_get = (serial_get + 1) % sizeof(serial_data);
		if(b & 1)
		{
#ifdef EXTERNAL_LED
			switch(g_color)
			{
			case 0:
				LED_ON;
				break;
			case 1:
				LED_RED0_ON;
				break;
			case 2:
				LED_GREEN0_ON;
				break;
			case 3:
				LED_BLUE0_ON;
				break;
			case 4:
				LED_RED1_ON;
				break;
			case 5:
				LED_GREEN1_ON;
				break;
			case 6:
				LED_BLUE1_ON;
				break;
			}
#else
			LED_ON;
#endif
		}
		else
		{
#ifdef EXTERNAL_LED
			switch(g_color)
			{
			case 0:
				LED_OFF;
				break;
			case 1:
				LED_RED0_OFF;
				break;
			case 2:
				LED_GREEN0_OFF;
				break;
			case 3:
				LED_BLUE0_OFF;
				break;
			case 4:
				LED_RED1_OFF;
				break;
			case 5:
				LED_GREEN1_OFF;
				break;
			case 6:
				LED_BLUE1_OFF;
				break;
			}
#else
			LED_OFF;
#endif
		}
	}
	} while(0);
	if(!irq_disabled)
	{
		__enable_irq();
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

#ifdef EXTERNAL_LED
	const uint32_t gpio_value = PORT_PCR_MUX(PORT::MUX_ALT_GPIO) |
		PORT_PCR_DSE | PORT_PCR_SRE;
	// RGB set 0
	PORTC_PCR4 = gpio_value;
	PORTD_PCR4 = gpio_value;
	PORTD_PCR7 = gpio_value;
	// RGB set 1
	PORTD_PCR5 = gpio_value;
	PORTD_PCR6 = gpio_value;
	PORTC_PCR1 = gpio_value;

	// common anode, they start on, so turn them off
	LED_RED0_OFF;
	LED_GREEN0_OFF;
	LED_BLUE0_OFF;
	LED_RED1_OFF;
	LED_GREEN1_OFF;
	LED_BLUE1_OFF;

	// output for all
	GPIOC_PDDR |= _BV(4) | _BV(1);
	GPIOD_PDDR |= _BV(4) | _BV(7) | _BV(5) | _BV(6);

	LED_RED0_ON;
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_RED0_OFF;

	LED_GREEN0_ON;
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_GREEN0_OFF;

	LED_BLUE0_ON;
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_BLUE0_OFF;

	LED_RED1_ON;
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_RED1_OFF;

	LED_GREEN1_ON;
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_GREEN1_OFF;

	LED_BLUE1_ON;
	for(int i = 0; i<F_CPU / 66; ++i)
		LED_OFF;
	LED_BLUE1_OFF;
#endif

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
	FTM0_SC &= ~FTM_SC_CLKS_MASK; // disable clock to update FTM0_MOD
	// emulate the timing of a 115200 baud serial port,
	// (only set to turn the LED on/off once per sent byte)

	/* Timing, serblink sets the serial port for
	 * 115200 1 start bit 8 data bits 1 stop bit
	 * one frame is (data sent not serial bits)
	 *
	 * start bit, 6 bytes
	 * 8x data bits, 8*6 - 4 = 44, (alternate 6 and 5 bytes for timing)
	 * 4*6 = 24 (blank between the two lines)
	 * start bit, 6 bytes
	 * 8x data bits, 8*6 - 4 = 44, (alternate 6 and 5 bytes for timing)
	 * 12*6 = 72 (blank between frames)
	 * = 196
	 * (115200 / 9) / 196 = 65 Hz watch ignores
	 * (115200 / 10) / 196 = 58.78 Hz watch works
	 * 48000000 / (115200 / 10) = 4166
	 * 48000000 / (4166 * 196) = 58.78
	 * 48000000 / (M * 196) = 60
	 * 48000000 / (60 * 196) = 4081.63
	 * 48000000 / (4081 * 196) = 60.0093
	 * 4081 is closer to 60 Hz, but going with F_CPU / (115200 / 10)
	 * because it is less of a mystery as to where the numbers come
	 * from and if the CPU speed changes it should still work.
	 */

	FTM0_MOD = F_CPU / (115200 / 10);
	// CPWMS center aligned (count up/down)
	// use FTM::prescale_1 which is also 0
	// TOIE Timer Overflow Interrupt Enable
	FTM0_SC = FTM_SC_TOIE | FTM_SC_CLKS(FTM::clk_system);
	// also enable irq on controller (but no data, initially disable)
	//NVIC_ENABLE_IRQ(IRQ_FTM0);

	// slower initially for debugging
	//FTM0_SC |= FTM_SC_PS(FTM::prescale_128);

	// PORT_PCR_DSE enable high drive strength, data sheet gives values
	// PORT_PCR_SRE enable slow slew rate ?  slower, no
	// PORT_PCR_ODE Open Drain Enabled

	#ifdef IRQ_CB
	// if it isn't polling, the main loop is empty, so go back to sleep
	SCB_SCR |= SCB_SCR_SLEEPONEXIT;
	#endif
	for(;;)
	{
		// Wait for Interrupt, sleep mode
		__WFI();

		#ifndef IRQ_CB
		rx_poll();
		#endif
	}
}
