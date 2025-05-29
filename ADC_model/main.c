#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "328p_gpio.h"
#include "328p_adc.h"
#include <util/delay.h>

int main(void) {
	ADC_standardInit();

	DDRB_UNION->port = PORT_ALL_OUTPUT;
	PORTB_UNION->port = PORT_ALL_OFF;

	unsigned int adc_value;

	while (1) {
		adc_value = ADC_read();

		uint8_t num_pins_on = (adc_value / 256) + 1;

		PORTB_UNION->port = (1 << num_pins_on) - 1;

		_delay_ms(100);
	}
}


