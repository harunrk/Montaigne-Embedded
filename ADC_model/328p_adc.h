#ifndef _328P_ADC_H
#define _328P_ADC_H

#define REG_ADDR_ADMUX					0x7C
#define REG_ADDR_ADCSRA					0x7A
#define REG_ADDR_DIDR0					0x7E

#define PORT_ALL_OFF					0x00
#define PORT_ALL_ON					0xFF

#define ENABLE						1
#define DISABLE						0

/*
	REFS1 REFS0		Voltage Reference Selection
	0     0			AREF, internal VREF turned off
	0     1			AV CC with external capacitor at AREF pin
	1     0			Reserved
	1     1			Internal 1.1V voltage reference with external capacitor at AREF pin
*/
typedef struct {
	uint8_t mux0 : 1;
	uint8_t mux1 : 1;
	uint8_t mux2 : 1;
	uint8_t mux3 : 1;
	uint8_t reserved : 1;
	uint8_t adlar : 1;
	uint8_t refs0 : 1;
	uint8_t refs1 : 1;
} _adcmsr_t;

typedef union {
	_adcmsr_t select;
	volatile uint8_t admux;
} adcmsr_t, *adcmsr_ptr_t;

/*
	ADPS2 ADPS1 ADPS0	Division Factor
	0     0		0	2
	0     0		1	2
	0     1		0	4
	0     1		1	8
	1     0		0	16
	1     0		1	32
	1     1		0	64
	1     1		1	128
*/
typedef struct {
	uint8_t adps0 : 1;
	uint8_t adps1 : 1;
	uint8_t adps2 : 1;
	uint8_t adie3 : 1;
	uint8_t adif : 1;
	uint8_t adate : 1;
	uint8_t adsc : 1;
	uint8_t aden : 1;
} _adcsra_t;

typedef union {
	_adcsra_t select;
	volatile uint8_t adcsra;
} adcsra_t, *adcsra_ptr_t;

#define ADMUX_UNION					((adcmsr_ptr_t) REG_ADDR_ADMUX)
#define ADCSRA_UNION					((adcsra_ptr_t) REG_ADDR_ADCSRA)

void ADC_standardInit() {
	ADMUX_UNION->admux = PORT_ALL_OFF;
	ADMUX_UNION->select.refs0 = ENABLE;  

	ADCSRA_UNION->adcsra = PORT_ALL_OFF;
	ADCSRA_UNION->select.aden = ENABLE;  
	// Prescaler 64
	ADCSRA_UNION->select.adps1 = ENABLE;
	ADCSRA_UNION->select.adps2 = ENABLE;
}

unsigned int ADC_read() {
	// Start the ADC conversion
	*(volatile uint8_t*) REG_ADDR_ADCSRA |= (1 << 6);

	// Wait until the conversion is complete
	while (*(volatile uint8_t*) REG_ADDR_ADCSRA & (1 << 6));

	// Read and combine the ADCL and ADCH values.
	unsigned int adc_value = *(volatile uint8_t*) 0x78;		// ADCL register
	adc_value |= (*(volatile uint8_t*) 0x79) << 8;			// ADCH register

	return adc_value;
}

#endif
