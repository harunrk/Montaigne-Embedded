#define USE_GPIO_D
#include "328p_gpio.h"

#define REG_ADDR_TCCR0						0x44	// TCCR0A - TCCR0B 16 bit
#define REG_ADDR_TCNT0						0x46		// Timer Counter
#define REG_ADDR_TIFR0						0x35		// Timer Interrupt Flag Register
#define _REG_TCCR0							(*(volatile uint16_t*) REG_ADDR_TCCR0)
#define _REG_TCNT0							(*(volatile uint8_t*) REG_ADDR_TCNT0)
#define	_REG_TIFR0							(*(volatile uint8_t*) REG_ADDR_TIFR0)

typedef struct{
	uint8_t wgmA			:2;
	uint8_t reservedA		:2;
	uint8_t com0B			:2;
	uint8_t com0A			:2;
	uint8_t clock_select	:3;
	uint8_t wgmB			:1;
	uint8_t reservedB		:2;
	uint8_t foc0B			:1;
	uint8_t foc0A			:1;	
}_tccr0_t, *_tccr0_ptr_t;					// Timer Counter Control Register

typedef union{
	_tccr0_t bits;
	volatile uint16_t value;
}tccr0_t, *tccr0_ptr_t;	

#define REG_TCCR0_UNION						(*(tccr0_ptr_t) REG_ADDR_TCCR0)
#define TIMER0_MODE_SELECTION				REG_TCCR0_UNION.bits.wgmA
#define TIMER0_mode_NORMAL					0
#define TIMER0_mode_CTC						2
#define TIMER0_CLOCK_SELECTION				REG_TCCR0_UNION.bits.clock_select
#define TIMER0_no_clock_source				0
#define TIMER0_ON_no_prescaling				1
#define	TIMER0_prescaling_8					2
#define	TIMER0_prescaling_64				3
#define	TIMER0_prescaling_256				4
#define	TIMER0_prescaling_1024				5
#define	TIMER0_ext_clock_falling_edge		6
#define	TIMER0_ext_clock_rising_edge		7

typedef struct{
	uint8_t value:8;	
}tcnt0_t, *tcnt_ptr_t;

#define	REG_TIMER0_COUNTER					(*(tccr0_ptr_t) REG_ADDR_TCNT0)
#define TIMER0_COUNTER						REG_TIMER0_COUNTER.value




uint16_t overflow;
uint8_t led_no;

// Timer ile 125ms geciktirmeli olarak led yakmak.
int main(void){
	DDRD_UNION.port = PORT_ALL_OUTPUT;
	TIMER0_MODE_SELECTION = TIMER0_mode_NORMAL;
	TIMER0_CLOCK_SELECTION = TIMER0_ON_no_prescaling;
	TIMER0_COUNTER = 0;
	
	overflow = 0;
	led_no = 0;
	
	while(1){
		if(TIMER0_COUNTER>=249){
			overflow++;
			TIMER0_COUNTER = 0;
			
			if(overflow >= 8000){
				overflow = 0;
				PORTD_UNION.port = (1<<led_no);
				led_no++;
			}
			
			led_no = (led_no>7)? 0: led_no;
		}
	}
}

